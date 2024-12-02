package com.luxoft.helloapp;

import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.luxoft.hello.IHello;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {

    private Button btnGpio;
    private TextView txtGetRPMValue;
    private TextView txt;

    private IHello hello;
    private Handler handler; // To update the UI from background thread
    private Thread updateThread; // The background thread that runs the loop

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        txtGetRPMValue = findViewById(R.id.txtGetRPMValue);
        txt = findViewById(R.id.txvalue);

        btnGpio = findViewById(R.id.btnGpio);

        // Initialize Handler to update the UI
        handler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                // Update the UI with the RPM value from the background thread
                if (msg.what == 1) {
                    int rpmValue = msg.arg1;
                    txtGetRPMValue.setText(Integer.toString(rpmValue));
                }
            }
        };

        // Bind the service to access IHello
        bindServiceToGetIHello();

        // Start the thread to continuously call getUpdate()
        startUpdateThread();

        // Stop the thread when the button is clicked
        btnGpio.setOnClickListener(v -> {
            stopUpdateThread();
            Toast.makeText(this, "Update Stopped", Toast.LENGTH_SHORT).show();
        });
    }

    private void bindServiceToGetIHello() {
        try {
            Class<?> localClass = Class.forName("android.os.ServiceManager");
            Method getService = localClass.getMethod("getService", String.class);
            if (getService != null) {
                Object result = getService.invoke(localClass, "com.luxoft.hello.IHello/default");
                if (result != null) {
                    IBinder binder = (IBinder) result;
                    hello = IHello.Stub.asInterface(binder);
                }
            }
        } catch (ClassNotFoundException | NoSuchMethodException | IllegalAccessException | InvocationTargetException e) {
            throw new RuntimeException(e);
        }
    }

    private void startUpdateThread() {
        updateThread = new Thread(() -> {
            try {
                while (true) {
                    int result = hello.getUpdate(); // Get the update
                    Boolean pressed = hello.get_button_state();
                    runOnUiThread(() -> {
                        // Update UI elements
                        txtGetRPMValue.setText(Integer.toString(result));
                        txt.setText(Boolean.toString(pressed));
                    });
                }
            } catch (Exception e) {
                Log.e("ITI", "failed to update rotary", e);
            }
        });
        updateThread.start(); // Start the background thread
    }
    
    

    private void stopUpdateThread() {
        if (updateThread != null && !updateThread.isInterrupted()) {
            updateThread.interrupt(); // Stop the thread
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        stopUpdateThread(); // Ensure the thread is stopped when the activity is destroyed
    }
}