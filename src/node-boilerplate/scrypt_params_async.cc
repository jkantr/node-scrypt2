#include <napi.h>

#include "scrypt_params_async.h"

//Scrypt is a C library and there needs c linkings
extern "C" {
  #include "pickparams.h"
}

using namespace Napi;

void ScryptParamsAsyncWorker::Execute() {
  // Scrypt: calculate input parameters
  result = pickparams(&logN, &r, &p, maxtime, maxmem, maxmemfrac, osfreemem);
}

void ScryptParamsAsyncWorker::OnOK() {
  Napi::HandleScope scope(Env());

  // Returned params in JSON object
  Napi::Object obj = Napi::Object::New(Env());
  obj.Set(Napi::String::New(Env(), "N"), Napi::Number::New(Env(), logN));
  obj.Set(Napi::String::New(Env(), "r"), Napi::Number::New(Env(), r));
  obj.Set(Napi::String::New(Env(), "p"), Napi::Number::New(Env(), p));

  Callback().Call({Env().Null(),obj});
}

// Asynchronous access to scrypt params
void params(const Napi::CallbackInfo& info) {
  ScryptParamsAsyncWorker* paramsWorker = new ScryptParamsAsyncWorker(info);
  paramsWorker->Queue();
}
