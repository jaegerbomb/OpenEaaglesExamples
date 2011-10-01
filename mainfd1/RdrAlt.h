//------------------------------------------------------------------------------
// Class:	RdrAlt
// Base class: BasicGL::Graphic -> RdrAlt
// 
// Description: radar altitude display and minimums.  It take the min value 
// of the radar, and will compare it to our current Ralt, and determine the color
// and visibility based on that value
// Inputs:
//      UPDATE_VALUE  -> actual radar alt
//      UPDATE_VALUE2 -> minimum radar alt
//------------------------------------------------------------------------------
#ifndef	__RdrAlt_CF2CCCBC_88C5_4df9_BBC3_C4C1DF910728
#define __RdrAlt_CF2CCCBC_88C5_4df9_BBC3_C4C1DF910728

#include "openeaagles/basicGL/Graphic.h"

namespace Eaagles {
namespace MainFlightDisplay1 {

class RdrAlt : public BasicGL::Graphic {
    DECLARE_SUBCLASS(RdrAlt,BasicGL::Graphic)
    
public:
    RdrAlt();
    
    // set functions
    virtual bool setRAlt(const LCreal newRA);       // Sets RADAR altitude (ft)
    virtual bool setRAltMin(const LCreal newRM);    // Sets RADAR altitude minimum (ft)
    
    // get functions
    LCreal getRadarAltFt()  { return rAlt; }
    LCreal getRdrMinAltFt() { return rAltMin; }
        
    // Component interface
    virtual void updateData(const LCreal dt = 0.0);
    virtual bool event(const int event, Basic::Object* const obj = 0);
    
private:
    // event functions
    bool onEventSetRAltRdrAlt(const Basic::Number* const x);
    bool onEventSetRAltMinRdrAlt(const Basic::Number* const x);
    
    LCreal rAlt;        // ft -- our actual radar alt
    SendData rAltSD;
    
    LCreal rAltMin;     // ft -- our minimum radar altitude setting
    SendData rAltMinSD;
};

}; // end of MainFlightDisplay1 namespace
}; // end of Eaagles namespace

#endif	/* __RdrAlt_CF2CCCBC_88C5_4df9_BBC3_C4C1DF910728 */
