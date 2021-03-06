//------------------------------------------------------------------------------
// Class: Random
// Description: Generates Random Numbers
//------------------------------------------------------------------------------

#ifndef __Eaagles_Tutorial_Random_H__
#define __Eaagles_Tutorial_Random_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Tutorial {

class Random : public Basic::Object
{
   DECLARE_SUBCLASS(Random, Basic::Object)

public:
   Random();

   int getNum() const;
   void setSeed(const unsigned int);
};

} // namespace Tutorial
} // namespace Eaagles


#endif
