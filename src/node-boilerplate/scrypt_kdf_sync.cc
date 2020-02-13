#include <napi.h>

#include "scrypt_common.h"

//
// Scrypt is a C library and there needs c linkings
//
extern "C" {
	#include "keyderivation.h"
}

using namespace Napi;

//
// Synchronous Scrypt params
//
Napi::Value kdfSync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    //
    // Variable Declaration
    //
    Napi::Value kdfResult = Napi::Buffer<char>::New(env, 96);

    //
    // Arguments from JavaScript
    //
    const uint8_t* key_ptr = reinterpret_cast<uint8_t*>(info[0].As<Napi::Buffer<char>>().Data()); //assume info[0] is a buffer (checked in JS land)
    const size_t keySize = info[0].As<Napi::Buffer<char>>().Length();
    const NodeScrypt::Params params(env, info[1].ToObject());
    const uint8_t* salt_ptr = reinterpret_cast<uint8_t*>(info[2].As<Napi::Buffer<char>>().Data());

    //
    // Scrypt key derivation function
    //
    const unsigned int result = KDF(key_ptr, keySize, reinterpret_cast<uint8_t*>(kdfResult.As<Napi::Buffer<char>>().Data()), params.N, params.r, params.p, salt_ptr);

    //
    // Error handling
    //
    if (result) {
        Napi::Error::New(env, NodeScrypt::ScryptError(env, result)).ThrowAsJavaScriptException();

    }

    return kdfResult;
}
