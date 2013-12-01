
#include "./TdElevPtr.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace TestOpenGL {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TdElevPtr,"TdElevPtr")
EMPTY_SERIALIZER(TdElevPtr)


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
TdElevPtr::TdElevPtr()
{
    STANDARD_CONSTRUCTOR()
    elev = 0.0;
    // we move a lot
    setDrawType(GL_DYNAMIC_DRAW);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void TdElevPtr::copyData(const TdElevPtr& org, const bool)
{
    BaseClass::copyData(org);
    elev = 0.0;
}

EMPTY_DELETEDATA(TdElevPtr)


//------------------------------------------------------------------------------
// event() -- default event handler
//------------------------------------------------------------------------------
bool TdElevPtr::event(const int event, Basic::Object* const obj)
{
    bool used = false;

    if (event == UPDATE_VALUE)
    {
        const Basic::Number* num = dynamic_cast<const Basic::Number*>(obj);
        if (num != 0) {
           // calclulate our delta
           double delta = 0.0f;
           if (elev != 0) {
               delta = elev - num->getReal();
           }
            elev = num->getReal();
            translateModelMatrix(0, delta, 0);
            used = true;
        }
    }
    
    if (!used)
    {
        // Otherwise, pass key events up to our base class
        used = BaseClass::event(event,obj);
    }

    return used;
}


//------------------------------------------------------------------------------
// draw() -- draw the pointer
//------------------------------------------------------------------------------
void TdElevPtr::draw()
{
   if (isVisible())  {
      // get our current model matrix, and translate it
      // determine how far we have translates
      // now draw
      BaseClass::draw();
   }
}

}
}
