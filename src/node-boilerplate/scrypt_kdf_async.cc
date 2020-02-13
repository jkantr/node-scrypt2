#include <napi.h>

#include "scrypt_kdf_async.h"

//Scrypt is a C library and there needs c linkings
extern "C" {
	#include "keyderivation.h"
}

using namespace Napi;

void ScryptKDFAsyncWorker::Execute() {
    //
    // Scrypt key derivation function
    //
    result = KDF(key_ptr, key_size, KDFResult_ptr, params.N, params.r, params.p, salt_ptr);
}

void ScryptKDFAsyncWorker::OnOK() {
    Napi::HandleScope scope(Env());

    Callback().Call({
        Env().Null(),
        ScryptPeristentObject.Get(Napi::String::New(Env(), "KDFResult"))
    });
}

// Asynchronous access to scrypt params
void kdf(const Napi::CallbackInfo& info) {
    ScryptKDFAsyncWorker* kdfWorker = new ScryptKDFAsyncWorker(info);
    kdfWorker->Queue();
}
