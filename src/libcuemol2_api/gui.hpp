#pragma once

#include "api.hpp"

// namespace qlib {
//   class LString;
//   class LScriptable;
//   class LVariant;
//   class LVarArgs;
//   class LDynamic;
// }
namespace gfx { class TextRenderImpl; }
namespace sysdep { class MouseEventHandler; }

namespace cuemol2 {

#ifdef BUILD_OPENGL_SYSDEP
  LIBCUEMOL_API gfx::TextRenderImpl *initTextRender();
  LIBCUEMOL_API void finiTextRender(gfx::TextRenderImpl *pTR);
  LIBCUEMOL_API sysdep::MouseEventHandler *createMouseEventHander();
#endif

}