//------------------------------------------------------------------------------
// Class:	TestOne
// Base class:	Object -> Component -> BasicGL::Graphic -> TestOne
//
// Description: Move within a user defined box displaying a trail
//
//
//  ( def-form TestOne
//  )
//
//
//------------------------------------------------------------------------------
#ifndef __EAAGLES_TESTOPENGL_TESTONE_H__
#define __EAAGLES_TESTOPENGL_TESTONE_H__

#include "openeaagles/basicGL2.0/core/Graphic.h"

namespace Eaagles {
   namespace Basic {
      class Angle;
   }
namespace TestOpenGL {


class TestOne : public BasicGL2_0::Graphic
{
    DECLARE_SUBCLASS(TestOne,BasicGL2_0::Graphic)

public:
   TestOne();

   void getPosition(LCreal& xx, LCreal& yy) const      { xx = xPos; yy = yPos; }
   void setPosition(const LCreal xx, const LCreal yy)  { xPos = xx; yPos = yy; }
   virtual bool realSpeed(const Basic::Number* const rsobj);
   virtual bool setAngle(Basic::Angle* saobj);
   virtual bool setAngle(const Basic::Number* const saobj);


   LCreal leftLimit() const                { return left; }
   void leftLimit(const LCreal ll)         { left = ll; }

   LCreal rightLimit() const               { return right; }
   void rightLimit(const LCreal rr)        { right = rr; }

   LCreal bottomLimit() const              { return bottom; }
   void bottomLimit(const LCreal bb)       { bottom = bb; }

   LCreal topLimit() const                 { return top; }
   void topLimit(const LCreal tt)          { top = tt; }

   void setStartAngle(const LCreal deg);
   void setSpeed(const LCreal xx);

   virtual void drawFunc();

   virtual void updateTC(const LCreal dt = 0.0);
   virtual void updateData(const LCreal dt = 0.0);
   virtual bool event(const int event, Basic::Object* const obj = 0);
   virtual void reset();

private:
    
    enum { maxHist = 10 };
    LCreal left, right;             // X limits
    LCreal bottom, top;             // Y limits
    LCreal xPos, yPos;              // Position
    LCreal xOld, yOld;              // Previous Position
    LCreal dx, dy;                  // Delta position
    LCreal speed;                   // Speed
    LCreal sangle;                  // Starting angle (radians)
    glm::vec3 trail[maxHist];       // Display trail
    int    nTrails;                 // Trail size
    int    index;                   // Trail index
    Basic::Angle* iangle;           // Input angle
    bool bufferCreated;             // Has our vertex buffer been created
};

} // End MainW namespace
} // End Eaagles namespace

#endif // __TestOne_H__
