#include <napi.h>

#include "scrypt_common.h"

//
// Scrypt is a C library and there needs c linkings
//
extern "C" {
  #include "hash.h"
}

using namespace Napi;

//
// Synchronous Scrypt params
//
Napi::Value hashSync(const Napi::CallbackInfo& info) {
  //
  // Arguments from JavaScript
  //
  Napi::Env env = info.Env();
  const uint8_t* key_ptr = reinterpret_cast<uint8_t*>(info[0].As<Napi::Buffer<char>>().Data());
  const size_t key_size = info[0].As<Napi::Buffer<char>>().Length();
  const NodeScrypt::Params params(env, info[1].ToObject());
  const size_t hash_size = info[2].As<Napi::Number>().Int64Value();
  const uint8_t* salt_ptr = reinterpret_cast<uint8_t*>(info[3].As<Napi::Buffer<char>>().Data());
  const size_t salt_size = info[3].As<Napi::Buffer<char>>().Length();

  //
  // Variable Declaration
  //
  Napi::Value hash_result = Napi::Buffer<char>::New(env, static_cast<uint32_t>(hash_size));
  uint8_t* hash_ptr = reinterpret_cast<uint8_t*>(hash_result.As<Napi::Buffer<char>>().Data());

  //
  // Scrypt key derivation function
  //
  const unsigned int result = Hash(key_ptr, key_size, salt_ptr, salt_size, params.N, params.r, params.p, hash_ptr, hash_size);

  //
  // Error handling
  //
  if (result) {
    Napi::Error::New(env, NodeScrypt::ScryptError(env, result)).ThrowAsJavaScriptException();
  }

  return hash_result;
}
