#pragma once

#include "api.hpp"

namespace qlib {
  class LString;
  class LScriptable;
  class LVariant;
  class LVarArgs;
  class LDynamic;
}

namespace cuemol2 {

  LIBCUEMOL_API bool getService(const qlib::LString &svcname,
                                qlib::LDynamic **prval,
                                qlib::LString &errmsg) noexcept;

  LIBCUEMOL_API bool createObj(const qlib::LString &svcname,
                               const qlib::LString &strval,
                               qlib::LDynamic **prval,
                               qlib::LString &errmsg) noexcept;

  LIBCUEMOL_API bool getProp(qlib::LScriptable *pthis,
                             const qlib::LString &propnm,
                             qlib::LVariant &val,
                             qlib::LString &errmsg) noexcept;
  
  LIBCUEMOL_API bool setProp(qlib::LScriptable *pthis,
                             const qlib::LString &propnm,
                             const qlib::LVariant &val,
                             qlib::LString &errmsg) noexcept;

  LIBCUEMOL_API bool invokeMethod(qlib::LScriptable *pthis,
                                  const qlib::LString &mthnm,
                                  qlib::LVarArgs &args,
                                  qlib::LString &errmsg) noexcept;
} // namespace cuemol2
