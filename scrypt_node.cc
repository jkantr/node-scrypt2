/*
  scrypt_node.cc

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
#include <napi.h>


using namespace Napi;

//
// Forward declarations
//
Napi::Value paramsSync(const Napi::CallbackInfo& info);
Napi::Value params(const Napi::CallbackInfo& info);
Napi::Value kdfSync(const Napi::CallbackInfo& info);
Napi::Value kdf(const Napi::CallbackInfo& info);
Napi::Value kdfVerifySync(const Napi::CallbackInfo& info);
Napi::Value kdfVerify(const Napi::CallbackInfo& info);
Napi::Value hashSync(const Napi::CallbackInfo& info);
Napi::Value hash(const Napi::CallbackInfo& info);

//
// Module initialisation
//
Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "paramsSync"), Napi::Function::New(env, paramsSync));

  exports.Set(Napi::String::New(env, "params"), Napi::Function::New(env, params));

  exports.Set(Napi::String::New(env, "kdfSync"), Napi::Function::New(env, kdfSync));

  exports.Set(Napi::String::New(env, "kdf"), Napi::Function::New(env, kdf));

  exports.Set(Napi::String::New(env, "verifySync"), Napi::Function::New(env, kdfVerifySync));

  exports.Set(Napi::String::New(env, "verify"), Napi::Function::New(env, kdfVerify));

  exports.Set(Napi::String::New(env, "hashSync"), Napi::Function::New(env, hashSync));

  exports.Set(Napi::String::New(env, "hash"), Napi::Function::New(env, hash));

  return exports;
}

NODE_API_MODULE(scrypt, InitAll)
