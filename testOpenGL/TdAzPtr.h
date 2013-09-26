//------------------------------------------------------------------------------
// Class:	TdAzPtr
// Base class:	Object -> Component -> BasicGL::Graphic -> TdAzPtr
//
// Description: TEST Display Azimuth Pointer
//
//
//  ( def-form TdAzPtr
//  )
//
//
//------------------------------------------------------------------------------
#ifndef __EAAGLES_TESTOPENGL_TDAZPTR_H__
#define __EAAGLES_TESTOPENGL_TDAZPTR_H__

#include "openeaagles/basicGL2.0/core/Graphic.h"

namespace Eaagles {
namespace TestOpenGL {

class TdAzPtr : public BasicGL2_0::Graphic
{
    DECLARE_SUBCLASS(TdAzPtr, BasicGL2_0::Graphic)

public:
    TdAzPtr();

    virtual bool event(const int event, Basic::Object* const obj = 0);
    virtual void draw();

private:
    LCreal      azimuth;
};

}
}

#endif
