/*
scrypt_kdf_async.h

Copyright (C) 2013 Barry Steyn (http://doctrina.org/Scrypt-Authentication-For-Node.html)

This source code is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this source code must not be misrepresented; you must not
   claim that you wrote the original source code. If you use this source code
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original source code.
3. This notice may not be removed or altered from any source distribution.

Barry Steyn barry.steyn@gmail.com
*/

#ifndef _SCRYPT_KDF_ASYNC_H
#define _SCRYPT_KDF_ASYNC_H

#include "scrypt_async.h"

class ScryptKDFAsyncWorker : public ScryptAsyncWorker {
  public:
    ScryptKDFAsyncWorker(const Napi::CallbackInfo& args) :
      ScryptAsyncWorker(args[3].As<Napi::Function>()),
      key_ptr(reinterpret_cast<uint8_t*>(args[0].As<Napi::Buffer<char>>().Data())),
      key_size(args[0].As<Napi::Buffer<char>>().Length()),
      params(args.Env(), args[1].ToObject()),
      salt_ptr(reinterpret_cast<uint8_t*>(args[2].As<Napi::Buffer<char>>().Data()))
    {
      Napi::Env env = args.Env();
      ScryptPeristentObject = Napi::Persistent(Napi::Object::New(env));
      ScryptPeristentObject.Set(Napi::String::New(env, "keyBuffer"), args[0]);
      ScryptPeristentObject.Set(Napi::String::New(env, "KDFResult"), Napi::Buffer<char>::New(env, 96));
      ScryptPeristentObject.Set(Napi::String::New(env, "salt"), args[2]);

      KDFResult_ptr = reinterpret_cast<uint8_t*>(ScryptPeristentObject.Get(Napi::String::New(env, "KDFResult")).As<Napi::Buffer<char>>().Data());
    };

    void Execute();
    void OnOK();

  private:
    uint8_t* KDFResult_ptr;
    const uint8_t* key_ptr;
    const size_t key_size;
    const NodeScrypt::Params params;
    const uint8_t* salt_ptr;
};

#endif /* _SCRYPT_KDF_ASYNC_H */
