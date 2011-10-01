//------------------------------------------------------------------------------
// Class: TestStation
//------------------------------------------------------------------------------
#ifndef __TestStation_H__
#define __TestStation_H__

#include "openeaagles/simulation/Station.h"

namespace Eaagles {
   namespace Glut  { class GlutDisplay; }

namespace MainIR {

//------------------------------------------------------------------------------
// Class: TestStation
// Description: Test station class that adds our graphics, which is managed by
//              a GlutDisplay, adds processing of the Hands On Throttle And Stick
//              (HOTAS) device, which is usually just a joystick, and adds stepping
//              of our ownship to the next local, air vehicle player.
//
// Form Name: TestStation
// Slots:
//   glutDisplay  <BasicGL::GlutDisplay>  ! All of our display components
//------------------------------------------------------------------------------
class TestStation : public Simulation::Station
{
   DECLARE_SUBCLASS(TestStation, Simulation::Station)

public:
   TestStation();

   // Step our "ownship" to the next local air vehicle
   void stepOwnshipPlayer();

   // Basic::Component functions
   virtual void updateTC(const LCreal dt = 0.0f);
   virtual void updateData(const LCreal dt = 0.0f);
   virtual void reset();

private:
   bool setSlotGlutDisplay(Glut::GlutDisplay* const msg);

   SPtr<Glut::GlutDisplay> glutDisplay;
   bool glutDisplayInit;
};

} // End MainIR namespace
} // End Eaagles namespace

#endif