#include <napi.h>

#include "scrypt_kdf-verify_async.h"

//Scrypt is a C library and there needs c linkings
extern "C" {
	#include "keyderivation.h"
}

using namespace Napi;

void ScryptKDFVerifyAsyncWorker::Execute() {
  //
  // Scrypt KDF Verification function
  //
  result = Verify(kdf_ptr, key_ptr, key_size);
  match = (result == 0);
  result = (result == 11) ? 0 : result; // Set result to 0 if 11 so error not thrown
}

void ScryptKDFVerifyAsyncWorker::OnOK() {
  Napi::HandleScope scope(Env());

  Callback().Call({
    Env().Null(),
    (match) ? Napi::Boolean::New(Env(), true) : Napi::Boolean::New(Env(), false)
  });
}

void kdfVerify(const Napi::CallbackInfo& info) {
  ScryptKDFVerifyAsyncWorker* kdfVerifyWorker = new ScryptKDFVerifyAsyncWorker(info);
  kdfVerifyWorker->Queue();
}
