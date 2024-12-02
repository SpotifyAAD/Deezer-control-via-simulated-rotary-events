#pragma once

#include <aidl/com/luxoft/hello/BnHello.h>
#include <mutex>


namespace aidl {
namespace com {
namespace luxoft {
namespace hello {


class HelloImpl : public BnHello {

    virtual ndk::ScopedAStatus getUpdate(int32_t* _aidl_return) override;


    virtual ndk::ScopedAStatus get_button_state(bool* _aidl_return) override;

    


protected:

    ::ndk::ScopedAIBinder_DeathRecipient death_recipient_;
    static void binderDiedCallbackAidl(void* cookie_ptr);

};

}  // namespace hello
}  // namespace hardware
}  // namespace aospinsight
}  // namespace aidl
