//*****************************************************************************
// Example routine for the basicGL and basic framework
//*****************************************************************************



#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/Timers.h"
#include "openeaagles/basic/Parser.h"
#include "openeaagles/basic/basicFF.h"
#include "openeaagles/basicGL/basicGLFF.h"
#include "openeaagles/basicGL2.0/formFunc.h"

#include "Display.h"
#include "GlutDisplay.h"
#include "TestOne.h"
#include <GL/glut.h>
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

namespace Eaagles {
namespace TestOpenGL {

// Frame Rate
const double frameRate = 50.0f;

// System descriptions
static class Display* sys = 0;
static class GlutDisplay* otherSys = 0;

double lastTime = 0.0f;


//=============================================================================
// Main test functions
//=============================================================================

static void error_callback(int error, const char* description)
{
	std::cout << "Main::error_callback, error # " << error << ", description = " << description << std::endl;
}


// timerFunc() -- called to update the graphics
static void timerFunc(int)
{
   double dt = 1.0/double(frameRate);

   unsigned int millis = (unsigned int)(dt * 1000);
   glutTimerFunc(millis, timerFunc, 1);

   Basic::Timer::updateTimers((LCreal)dt);
   //BasicGL2_0::Graphic::flashTimer((LCreal)dt);
   otherSys->tcFrame((LCreal)dt);
}

// Test Form Function
static Basic::Object* testFormFunc(const char* formname)
{
   Basic::Object* newform = 0;

   // This test ...
   if ( strcmp(formname, Display::getFormName()) == 0 ) {
      newform = new Display;
   }
   else if ( strcmp(formname, GlutDisplay::getFormName()) == 0 ) {
      newform = new GlutDisplay;
   }
   else if ( strcmp(formname, TestOne::getFormName()) == 0 ) {
      newform = new TestOne;
   }
   else {
      if (newform == 0) newform = BasicGL2_0::formFunc(formname);
      if (newform == 0) newform = Basic::basicFormFunc(formname);
   }
   return newform;
}

// readTest() -- function to the read description files
static void readTest(const char* const testFileName)
{
    // Read the description file
    int errors = 0;
    Basic::Object* q1 = Basic::lcParser(testFileName, testFormFunc, &errors);
    if (errors > 0) {
        std::cerr << "Errors in reading file: " << errors << std::endl;
        exit(1);
    }

    // Set 'sys' to our basic description object.
    sys = 0;
    if (q1 != 0) {

        // When we were given a Pair, get the pointer to its object.
        Basic::Pair* pp = dynamic_cast<Basic::Pair*>(q1);
        if (pp != 0) {
           std::cout << "Form: " << *pp->slot() << std::endl;
            q1 = pp->object();
        }

        // What we should have here is the description object and
        // it should be of type 'Display'.
        sys = dynamic_cast<Display*>(q1);
    }

   // Make sure we did get a valid object (we must have one!)
   if (sys == 0) {
      // let's check to see if we are using glut displays
      otherSys = dynamic_cast<GlutDisplay*>(q1);
      if (otherSys == 0) {
         std::cout << "Invalid description file!" << std::endl;
         exit(EXIT_FAILURE);
      }
    }

    //sys->serialize(std::cout);
}

// ---
// CALLBACKS
// ---
static void reshapeCB(GLFWwindow* win, const int w, const int h)
{
   // Single window for now.
   sys->reshapeIt(w, h);   
}
// handles when the cursor moves... the user can check the status
// of the mouse buttons to determine passive or active motion
static void cursorMoveCB(GLFWwindow* win, const double x, const double y)
{
   sys->cursorMove(x, y);
}
// mouse button callbacks - wheel is not called here
static void mouseButtonCB(GLFWwindow* win, int button, int state, int mods)
{
   sys->mouseButtonEvent(button, state, mods);
}
// keyboard callback, with action (GL_PRESS, RELEASE) and any modification keys
// (CTRL, SHIFT)
static void keyCB(GLFWwindow* win, int key, int scanCode, int action, int mods)
{
   sys->keyboardEvent(key, action, mods);
}
// called when the given window closes.
static void closeEventCB(GLFWwindow* win)
{
   sys->closeEvent();
}
// called when a window gains or loses focus (GL_TRUE for gain focus)
static void focusCB(GLFWwindow* win, int state)
{
   sys->focus(state);
}

typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

void setVSync(int interval=1)
{
  const char *extensions = (const char*)glGetString( GL_EXTENSIONS );

  if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
    return; // Error: WGL_EXT_swap_control extension not supported on your computer.\n");
  else
  {
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

    if( wglSwapIntervalEXT )
      wglSwapIntervalEXT(interval);
  }
}

// timerFunc() -- Time critical stuff)
static void timerFunc(double curTime)
{
   if (lastTime == 0) {
      lastTime = curTime;
   }
   else {
      if ((curTime - lastTime) > double(1.0f/frameRate)) {
         if (sys != 0) sys->updateTC(double(1.0f/frameRate));
         else otherSys->updateTC(double(1.0f/frameRate));
         lastTime = curTime;
      }
   }
}

//-----------------------------------------------------------------------------
// main() -- Main routine
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{



   // Config file file
   const char* configFile = "test.edl";

   // Parse arguments
   for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i],"-f") == 0) {
         configFile = argv[++i];
      }
   }

// ---
// Read in the description files
// ---
   readTest(configFile);

   // ---
   // GLFW windows
   // ---
   if (sys != 0) {
      // set our error callback
      glfwSetErrorCallback(error_callback);


      // initialize glfw
      if (!glfwInit()) exit(EXIT_FAILURE);
      sys->createWindow();

      //setVSync(0);
      GLFWwindow* win = sys->getWindowInstance();
      if (win != 0) {
         // the cool part about this is we set up the callbacks right here, so
         glfwSetWindowSizeCallback(win, reshapeCB);
         glfwSetCursorPosCallback(win, cursorMoveCB);
         glfwSetMouseButtonCallback(win, mouseButtonCB);
         glfwSetKeyCallback(win, keyCB);
         glfwSetWindowCloseCallback(win, closeEventCB);
         glfwSetWindowFocusCallback(win, focusCB);
      }

      // let's remove vertical sync, to be able to print out true performance characteristics
      // WINDOW

      // call our draw routine - I put the update data here as well, but
      // users can update at whatever rate they wish.  I also have no
      // sleep functions here, which could be useful if optimization
      // is implemented.
      double lastTime = glfwGetTime();
      int nbFrames = 0;
      while (!sys->isWindowShutdown()) {
         // Measure speed
         double currentTime = glfwGetTime();
         nbFrames++;
         if ( currentTime - lastTime >= 1.0 ){ // If last printf() was more than 1 sec ago
            // printf and reset timer
            printf("%f ms/frame\n", 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
         }
         // we can also update data from here, as the callbacks are done in sequence
         sys->updateData(0.025);
         //timerFunc(glfwGetTime());
         // SLS - these are made up numbers
         sys->updateTC(0.02);

         // draw all of our schtuff
         sys->drawIt();

         // poll for any new key, refresh, or other callback that we have done
         glfwPollEvents();

         // now sleep for just a bit
         Eaagles::lcSleep(10);
      }

      glfwTerminate();
   }
   // GLUT
   else {
      glutInit(&argc, argv);
      // ---
      // Create a display window
      // ---
      // initialize glfw
      otherSys->createWindow();

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
   }
   return 0;
}

} 
} 

//-----------------------------------------------------------------------------
// main() -- Main routine
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
   return Eaagles::TestOpenGL::main(argc,argv);
}
