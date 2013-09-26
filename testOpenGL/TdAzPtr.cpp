
#include "./TdAzPtr.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace TestOpenGL {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TdAzPtr,"TdAzPtr")
EMPTY_SERIALIZER(TdAzPtr)


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
TdAzPtr::TdAzPtr()
{
    STANDARD_CONSTRUCTOR()
    azimuth = 0.0;
    // we move a lot
    setDrawType(GL_DYNAMIC_DRAW);
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void TdAzPtr::copyData(const TdAzPtr& org, const bool)
{
    BaseClass::copyData(org);
    azimuth = 0.0;
}

EMPTY_DELETEDATA(TdAzPtr)


//------------------------------------------------------------------------------
// event() -- default event handler
//------------------------------------------------------------------------------
bool TdAzPtr::event(const int event, Basic::Object* const obj)
{
    bool used = false;

    if (event == UPDATE_VALUE)
    {
        const Basic::Number* num = dynamic_cast<const Basic::Number*>(obj);
        if (num != 0) {
            azimuth = num->getReal();
            glm::mat4 myMatrix = getModelMatrix();
            //std::cout << "my Matrix = " << myMatrix[3].x << std::endl;
            double delta = (azimuth - myMatrix[3].x);
            //std::cout << "DELTA = " << delta << std::endl;
            translateModelMatrix(delta, 0, 0);
            //std::cout << "AZIMUTH = " << azimuth << std::endl;
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
void TdAzPtr::draw()
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
