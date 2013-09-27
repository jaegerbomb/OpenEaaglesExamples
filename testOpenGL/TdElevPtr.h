//------------------------------------------------------------------------------
// Class:	TdElevPtr
// Base class:	Object -> Component -> BasicGL::Graphic -> TdElevPtr
//
// Description: TEST Display Elevation Pointer
//
//
//  ( def-form TdElevPtr
//  )
//
//
//------------------------------------------------------------------------------
#ifndef __EAAGLES_TESTOPENGL_TDELEVPTR_H__
#define __EAAGLES_TESTOPENGL_TDELEVPTR_H__

#include "openeaagles/basicGL2.0/core/Graphic.h"

namespace Eaagles {
namespace TestOpenGL {

class TdElevPtr : public BasicGL2_0::Graphic
{
    DECLARE_SUBCLASS(TdElevPtr, BasicGL2_0::Graphic)

public:
    TdElevPtr();

    virtual bool event(const int event, Basic::Object* const obj = 0);
    virtual void draw();

private:
    LCreal      elev;
};

}
}

#endif
