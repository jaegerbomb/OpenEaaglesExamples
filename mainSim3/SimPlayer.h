//------------------------------------------------------------------------------
// Class:	SimPlayer
// Base class:	Basic::Object -> Basic::Component -> Player -> AirVehicle -> xxx -> SimPlayer
//
// Description: Sim Player
//
//------------------------------------------------------------------------------
#ifndef __SimPlayer__
#define __SimPlayer__

#include "openeaagles/simulation/AirVehicle.h"

namespace Eaagles {
namespace Sim3 {

class SimPlayer : public Simulation::Aircraft
{
    DECLARE_SUBCLASS(SimPlayer,Simulation::Aircraft)

public:
    SimPlayer();
    
    // Basic::Component interface
    virtual void reset();

private:
};

} // End Sim3 namespace
} // End Eaagles namespace

#endif // __SimPlayer__