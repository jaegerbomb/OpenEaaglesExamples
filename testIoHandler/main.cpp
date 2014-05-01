//*****************************************************************************
// I/O Device Test program
//*****************************************************************************

#include "Display.h"
#include "Factory.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/Timers.h"
#include "openeaagles/basic/Parser.h"

#include <GL/glut.h>

#include <cstring>
#include <cstdlib>

namespace Eaagles {
namespace Test {

// Frame Rate
const int frameRate = 20;

// The main station
Display* display = 0;

//=============================================================================
// Main MFD functions
//=============================================================================

// timerFunc() -- Time critical stuff)
static void timerFunc(int)
{
   double dt = 1.0/double(frameRate);

   unsigned int millis = (unsigned int)(dt * 1000);
   glutTimerFunc(millis, timerFunc, 1);

   Basic::Timer::updateTimers(dt);
   BasicGL::Graphic::flashTimer(dt);
   display->updateTC(dt);
}

// build a display as specified by configuration file
static Display* builder(const char* const filename)
{
   Display* sys = 0;

   // Read the description file
   int errors = 0;
   Basic::Object* q1 = lcParser(filename, Factory::createObj, &errors);
   if (errors > 0) {
      std::cerr << "Errors in reading file: " << errors << std::endl;
      std::exit(1);
   }

   // Set 'sys' to our basic description object.
   if (q1 != 0) {
      // When we were given a Pair, get the pointer to its object.
      Basic::Pair* pp = dynamic_cast<Basic::Pair*>(q1);
      if (pp != 0) {
         q1 = pp->object();
      }
      sys = dynamic_cast<Display*>(q1);
   }

   return sys;
}

//-----------------------------------------------------------------------------
// main() -- Main routine
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
   glutInit(&argc, argv);

   // Description (input) File -- After being processed by the C preprocessor
   const char* fileName = "test1.edl";

   for (int i = 1; i < argc; i++) {
      if (std::strcmp(argv[i],"-f") == 0) {
         fileName = argv[++i];
      }
   }

   // ---
   // Build a display
   // ---
   display = builder(fileName);

   // Make sure we did get a valid object (we must have one!)
   if (display == 0) {
      std::cout << "main(): invalid description file!" << std::endl;
      std::exit(EXIT_FAILURE);
   }
   //display->serialize(std::cout);

   // ---
   // Create a display window
   // ---
   display->createWindow();

   // ---
   // reset the system
   // ---
   display->reset();

   // ---
   // Set timer
   // ---
   double dt = 1.0/double(frameRate);
   unsigned int millis = (unsigned int)(dt * 1000);
   glutTimerFunc(millis, timerFunc, 1);

   // ---
   // Main loop
   // ---
   glutMainLoop();
   return 0;
}

} // End of Test namespace
} // End of Eaagles namespace

//-----------------------------------------------------------------------------
// main() -- Main routine
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
   Eaagles::Test::main(argc,argv);
}

