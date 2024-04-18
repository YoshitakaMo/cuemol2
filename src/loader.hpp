#pragma once

#ifdef WIN32
// for Windows
#  ifdef LIBCUEMOL_EXPORTS
#    define LIBCUEMOL_API __declspec(dllexport)
#  elif defined(LINK_SHARED)
#    define LIBCUEMOL_API __declspec(dllimport)
#  else
#    define LIBCUEMOL_API
#  endif
#else
// other platforms
#  define LIBCUEMOL_API
#endif

#include <common.h>

namespace qlib { class LString; }
namespace gfx { class TextRenderImpl; }
namespace sysdep { class MouseEventHandler; }

namespace cuemol2 {

  LIBCUEMOL_API int init_qlib() noexcept;
  LIBCUEMOL_API int init(const qlib::LString &confpath, bool reg_view) noexcept;

  LIBCUEMOL_API int fini();
  
#ifdef BUILD_OPENGL_SYSDEP
  LIBCUEMOL_API gfx::TextRenderImpl *initTextRender();
  LIBCUEMOL_API void finiTextRender(gfx::TextRenderImpl *pTR);
  LIBCUEMOL_API sysdep::MouseEventHandler *createMouseEventHander();
#endif
    
} // namespace cuemol2
