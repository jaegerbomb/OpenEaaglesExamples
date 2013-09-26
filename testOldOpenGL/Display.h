//------------------------------------------------------------------------------
// Class: OpenGL GLFW3 Display
//------------------------------------------------------------------------------
#ifndef __Eaagles_TestOldOpenGL_Display_H__
#define __Eaagles_TestOldOpenGL_Display_H__

#include "openeaagles/basicGL/Display.h"

struct GLFWwindow;

namespace Eaagles {
namespace TestOldOpenGL {

class Display : public BasicGL::Display {
   DECLARE_SUBCLASS(Display, BasicGL::Display)

public:
   Display();

   // Creates the window 
   virtual int createWindow();

   bool isWindowShutdown() const;
   
   // GLFW specific functions
   GLFWwindow* getWindowInstance();
   void cursorMove(const double x, const double y);
   // mouse event is slightly different for GLFW
   void mouseButtonEvent(const int button, const int state, const int mods);
   // keyboard events
   void keyboardEvent(const int key, const int action, const int mods);
   // window close event
   void closeEvent();
   // window level focus events
   void focus(const int state);

   // Basic::Component functions
   virtual void reset();

protected:
   // slot functions
   bool setSlotMajorVersion(const Basic::Number* const x);
   bool setSlotMinorVersion(const Basic::Number* const x);
   bool setSlotCompatibility(const Basic::Number* const x);
   bool setSlotBenchmark(const Basic::Number* const x);

   // BasicGL2.0::Display functions
   virtual void select();                       // Selects this display.
   virtual void swapBuffers();
   

   virtual void specialEvent(const int key);

   // Handle passive mouse motion events
   virtual void passiveMotionEvent(const int x, const int y);

   // Handle mouse motion events with either mouse key pressed
   virtual void mouseMotionEvent(const int x, const int y);



private:
   void initData();
   
   // find our display by the GLFWwindow handle
   //Display* findDisplay(GLFWwindow* win);

   //// callback functions
   //static void reshapeCB(GLFWwindow* win, const int w, const int h);


   GLFWwindow* myWin;				// holds the pointer to our GLFW 3 window
   int glMajorVersion;           // openGL major version we would like to use
   int glMinorVersion;           // openGL minor version we would like to use
   bool compatibility;           // are we using the compatibility extensions?
   bool benchmark;               // create a ton of graphics?
};

inline GLFWwindow* Display::getWindowInstance()     { return myWin; }


} 
} 
#endif