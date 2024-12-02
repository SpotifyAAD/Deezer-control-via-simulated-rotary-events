package com.volvocars.mediasample.dataaccess

import android.content.Context
import android.util.Log
import com.volvocars.mediasample.common.util.ResourceUriUtil
import com.volvocars.mediasample.domain.Playlist
import com.volvocars.mediasample.domain.Song
import okhttp3.OkHttpClient
import okhttp3.Request
import org.json.JSONObject
import java.io.File
import java.util.concurrent.Executors
import java.util.concurrent.CountDownLatch
import java.util.concurrent.ExecutorService
import kotlin.random.Random

private const val PLAYLIST_ID = "deezer-playlist-id"
private const val DEEZER_API_URL = "https://api.deezer.com/playlist/908622995"

class PlaylistFactory(
    private val context: Context,
    private val resourceUriUtil: ResourceUriUtil
) {

    private val songTitles = mutableListOf<String>()
    private val lock = Any() // For thread-safe access to songTitles

    fun createPlaylist(): Playlist {
        val executor = Executors.newFixedThreadPool(8)
        val latch = CountDownLatch(8) // Wait for 8 downloads to complete

        Thread {
            downloadRandomTracks(executor, latch)
        }.start()

        latch.await() // Ensure all tracks are downloaded before creating the playlist

        return Playlist(
            id = PLAYLIST_ID,
            songs = getSongs()
        )
    }

    private fun downloadRandomTracks(executor: ExecutorService, latch: CountDownLatch) {
        val client = OkHttpClient()
        val request = Request.Builder().url(DEEZER_API_URL).build()

        try {
            val response = client.newCall(request).execute()
            if (response.isSuccessful) {
                val jsonData = response.body?.string() ?: return
                val playlistJson = JSONObject(jsonData)
                val tracksArray = playlistJson.getJSONObject("tracks").getJSONArray("data")

                val trackCount = tracksArray.length()
                val randomIndices = mutableSetOf<Int>()

                // Generate up to 8 unique random indices
                while (randomIndices.size < 8 && randomIndices.size < trackCount) {
                    randomIndices.add(Random.nextInt(trackCount))
                }

                randomIndices.forEachIndexed { index, i ->
                    val track = tracksArray.getJSONObject(i)
                    val trackUrl = track.getString("preview")
                    val trackTitle = track.getString("title")
                    val fileName = "track_${index + 1}.mp3"

                    synchronized(lock) {
                        songTitles.add(trackTitle)
                    }

                    executor.submit {
                        try {
                            downloadTrack(trackUrl, fileName, trackTitle)
                        } finally {
                            latch.countDown() // Signal task completion
                        }
                    }
                }

                executor.shutdown()
            } else {
                Log.e("APIError", "Failed to fetch playlist data")
            }
        } catch (e: Exception) {
            Log.e("DownloadError", "Error occurred: ${e.message}")
            latch.countDown()
        }
    }

    private fun downloadTrack(url: String, fileName: String, trackTitle: String) {
        val client = OkHttpClient()
        val request = Request.Builder().url(url).build()
        val file = File(context.filesDir, fileName)

        if (file.exists()) {
            Log.d("DownloadStatus", "File already exists: $fileName")
            return
        }

        try {
            val response = client.newCall(request).execute()
            if (response.isSuccessful) {
                file.outputStream().use { output ->
                    response.body?.byteStream()?.copyTo(output)
                }
                Log.d("DownloadStatus", "Download completed: $trackTitle as $fileName")
            } else {
                Log.e("DownloadStatus", "Failed to download $fileName")
            }
        } catch (e: Exception) {
            Log.e("DownloadError", "Error downloading $fileName: ${e.message}")
        }
    }

    private fun getSongs(): List<Song> {
        synchronized(lock) {
            if (songTitles.isEmpty()) {
                Log.e("PlaylistError", "No songs downloaded")
                return emptyList()
            }

            return songTitles.mapIndexed { index, title ->
                val fileName = "track_${index + 1}.mp3"
                Song(
                    id = "track_${index + 5}",
                    title = title,
                    subtitle = "Deezer Track",
                    description = "Downloaded from Deezer Playlist",
                    mediaArtUri = "",
                    mediaUri = File(context.filesDir, fileName).absolutePath,
                    artist = "Unknown Artist"
                )
            }
        }
    }
}
