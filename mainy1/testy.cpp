
#include "Factory.h"

#include "openeaagles/simulation/Station.h"
#include "openeaagles/basicGL/Graphic.h"
#include "openeaagles/basic/Parser.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/Timers.h"

#include <cstdlib>
#include <cstring>

#include <GL/glut.h>

// Default configuration file
static const char* const DEFAULT_CONFIG_FILE = "test1.edl";

// default background frame rate
static const int BG_RATE = 10;

// Top level Station
static Eaagles::Simulation::Station* station = 0;

// build a station
static Eaagles::Simulation::Station* builder(const char* const fileName)
{
   Eaagles::Simulation::Station* p = 0;

   // Read the description file
   int errors = 0;
   Eaagles::Basic::Object* q1 =
         Eaagles::Basic::lcParser(fileName, Eaagles::Example::Factory::createObj, &errors);
   if (errors > 0) {
      std::cerr << "File: " << fileName << ", errors: " << errors << std::endl;
      return 0;
   }

   if (q1 != 0) {
      // When we were given a Basic::Pair, get the pointer to its object.
      Eaagles::Basic::Pair* pp = dynamic_cast<Eaagles::Basic::Pair*>(q1);
      if (pp != 0) {
         q1 = pp->object();
      }

      // What we should have here is the Station object
      p = dynamic_cast<Eaagles::Simulation::Station*>(q1);
   }
    
   return p;
}

//-----------------------------------------------------------------------------
// Station's background tasks -- a callback from a GLUT timer, it's basicly the
// top level of our background thread.  (Note: GlutDisplay will handle the
// background thread, updateData, for all of its graphics components)
//-----------------------------------------------------------------------------
static void updateDataCB(int msecs)
{
   glutTimerFunc(msecs, updateDataCB, msecs);

   // Current time
   double time = Eaagles::getComputerTime();

   // Compute delta time
   static double time0 = time;   // N-1 Time
   Eaagles::LCreal dt = Eaagles::LCreal(time - time0);
   time0 = time;

   station->updateData(dt);
}

//-----------------------------------------------------------------------------
// main() -- Main routine
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
   glutInit(&argc, argv);

   // configuration file
   const char* configFile = DEFAULT_CONFIG_FILE;

   // Parse arguments
   for (int i = 1; i < argc; i++) {
      if (std::strcmp(argv[i],"-f") == 0) {
         configFile = argv[++i];
      }
   }

   // ---
   // Build a station
   // ---
   station = builder(configFile);
   if (station == 0) {
      std::cerr << "Invalid configuration file!" << std::endl;
      std::exit(EXIT_FAILURE);
   }

   // ---
   // Reset the Simulation
   // ---
   station->event(Eaagles::Basic::Component::RESET_EVENT);

   // ---
   // Set timer for the background tasks
   // ---
   double dt = 1.0/double(BG_RATE);
   int msecs = (int) (dt * 1000);

   // ensure everything is reset
   station->updateData(dt);
   station->updateTC(dt);
   station->event(Eaagles::Basic::Component::RESET_EVENT);

   glutTimerFunc(msecs, updateDataCB, msecs);

   // ---
   // Create the Time Critical Thread (updateTC())
   // ---
   station->createTimeCriticalProcess();

   // ---
   // Main loop
   // ---
   glutMainLoop();
   return EXIT_SUCCESS;
}
