/*
scrypt_hash_async.h

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

#ifndef _SCRYPTHASHASYNC_
#define _SCRYPTHASHASYNC_

#include "scrypt_async.h"

class ScryptHashAsyncWorker : public ScryptAsyncWorker {
  public:
    ScryptHashAsyncWorker(const Napi::CallbackInfo& info) :
      ScryptAsyncWorker(info[4].As<Napi::Function>()),
      key_ptr(reinterpret_cast<uint8_t*>(info[0].As<Napi::Buffer<char>>().Data())),
      key_size(info[0].As<Napi::Buffer<char>>().Length()),
      params(info.Env(), info[1].ToObject()),
      hash_size(info[2].As<Napi::Number>().Int64Value()),
      salt_ptr(reinterpret_cast<uint8_t*>(info[3].As<Napi::Buffer<char>>().Data())),
      salt_size(static_cast<size_t>(info[3].As<Napi::Buffer<char>>().Length()))
    {
      ScryptPeristentObject = Napi::Persistent(Napi::Object::New(info.Env()));
      ScryptPeristentObject.Set(Napi::String::New(info.Env(), "KeyBuffer"), info[0]);
      ScryptPeristentObject.Set(Napi::String::New(info.Env(), "HashBuffer"), Napi::Buffer<char>::New(info.Env(), static_cast<uint32_t>(hash_size)));
      ScryptPeristentObject.Set(Napi::String::New(info.Env(), "SaltBuffer"), info[3]);

      hash_ptr = reinterpret_cast<uint8_t*>(ScryptPeristentObject.Get(Napi::String::New(info.Env(), "HashBuffer")).As<Napi::Buffer<char>>().Data());
    };

    void Execute();
    void OnOK();

  private:
    const uint8_t* key_ptr;
    const size_t key_size;
    const NodeScrypt::Params params;
    const size_t hash_size;
    const uint8_t* salt_ptr;
    const size_t salt_size;
    uint8_t* hash_ptr;
};

#endif /* _SCRYPTHASHASYNC_ */
