#include <napi.h>

#include "scrypt_common.h"

//Scrypt is a C library and there needs c linkings
extern "C" {
  #include "keyderivation.h"
}

using namespace Napi;

//Synchronous access to scrypt params
Napi::Value kdfVerifySync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  //
  // Variable Declaration
  //
  const uint8_t* kdf_ptr = reinterpret_cast<uint8_t*>(info[0].As<Napi::Buffer<char>>().Data());
  const uint8_t* key_ptr = reinterpret_cast<uint8_t*>(info[1].As<Napi::Buffer<char>>().Data());
  const size_t key_size = info[1].As<Napi::Buffer<char>>().Length();

  //
  // Scrypt KDF Verification
  //
  const unsigned int result = Verify(kdf_ptr, key_ptr, key_size);

  //
  // Return result (or error)
  //
  if (result && result != 11) { // 11 is the "error" code for an incorrect match
    Napi::Error::New(env, NodeScrypt::ScryptError(env, result)).ThrowAsJavaScriptException();

  }

  return (!result) ? Napi::Boolean::New(env, true) : Napi::Boolean::New(env, false);
}
