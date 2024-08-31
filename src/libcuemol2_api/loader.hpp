#pragma once

#include "api.hpp"

namespace qlib {
  class LString;
  class TimerImpl;
}

namespace cuemol2 {
  LIBCUEMOL_API int init_qlib() noexcept;
  LIBCUEMOL_API int init(const qlib::LString &confpath, bool reg_view) noexcept;
  LIBCUEMOL_API int init_timer(qlib::TimerImpl *pTimer) noexcept;
  LIBCUEMOL_API int fini() noexcept;
  LIBCUEMOL_API int fini_qlib() noexcept;
} // namespace cuemol2
