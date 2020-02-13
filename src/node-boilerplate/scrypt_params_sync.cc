#include <napi.h>

#include "scrypt_common.h"

//Scrypt is a C library and there needs c linkings
extern "C" {
  #include "pickparams.h"
}

using namespace Napi;

//Synchronous access to scrypt params
Napi::Value paramsSync(const Napi::CallbackInfo& info) {
  //
  // Variable Declaration
  //
  int logN = 0;
  uint32_t r = 0;
  uint32_t p = 0;

  //
  // Arguments from JavaScript
  //
  Napi::Env env = info.Env();
  const double maxtime = info[0].As<Napi::Number>().DoubleValue();
  const size_t maxmem = info[2].As<Napi::Number>().Int64Value();
  const double maxmemfrac = info[1].As<Napi::Number>().DoubleValue();
  const size_t osfreemem = info[3].As<Napi::Number>().Int64Value();

  //
  // Scrypt: calculate input parameters
  //
  const unsigned int result = pickparams(&logN, &r, &p, maxtime, maxmem, maxmemfrac, osfreemem);

  //
  // Error handling
  //
  if (result) {
    Napi::Error::New(env, NodeScrypt::ScryptError(env, result)).ThrowAsJavaScriptException();
  }

  //
  // Return values in JSON object
  //
  Napi::Object obj = Napi::Object::New(env);
  obj.Set(Napi::String::New(env, "N"), Napi::Number::New(env, logN));
  obj.Set(Napi::String::New(env, "r"), Napi::Number::New(env, r));
  obj.Set(Napi::String::New(env, "p"), Napi::Number::New(env, p));

  return obj;
}
