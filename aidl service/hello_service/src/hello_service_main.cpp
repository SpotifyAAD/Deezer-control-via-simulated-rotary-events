#include "HelloImpl.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using aidl::com::luxoft::hello::HelloImpl;

int main() {
    LOG(INFO) << "Hello daemon started!";

    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<HelloImpl> hello = ndk::SharedRefBase::make<HelloImpl>();

    const std::string instance = std::string() + HelloImpl::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(hello->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reached
}