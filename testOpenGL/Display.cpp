// Include GLEW FIRST
#include "Display.h"

#include "TestOne.h"

#include "openeaagles/basic/Color.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
// Shader includes glew
#include "openeaagles/basicGL2.0/core/Shader.h"
#include <GLFW/glfw3.h>

// LEE TEST
#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
namespace TestOpenGL {

IMPLEMENT_SUBCLASS(Display,"GLFW3Display")
EMPTY_SERIALIZER(Display)

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Display)
   "glMajorVersion",       // 1) OpenGL major version we want to use (not guaranteed)
   "glMinorVersion",       // 2) OpenGL minor version we want to use (not guaranteed)
   "glCompatibility",       // 3) Do we load the openGL compatibility extensions (for backwards compatibility)
   "benchmark"             // 4) benchmark
END_SLOTTABLE(Display)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Display)
   ON_SLOT(1,setSlotMajorVersion,Basic::Number)
   ON_SLOT(2,setSlotMinorVersion, Basic::Number)
   ON_SLOT(3,setSlotCompatibility, Basic::Number)
   ON_SLOT(4, setSlotBenchmark, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Display::Display()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// InitData
//------------------------------------------------------------------------------
void Display::initData()
{
	myWin = 0;
   // default to no version hints (which means we will get the latest OpenGL WITH the
   // compatibility loaded)
   glMajorVersion = 0;
   glMinorVersion = 0;
   compatibility = true;
   benchmark = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy our objects data
//------------------------------------------------------------------------------
void Display::copyData(const Display& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   myWin = org.myWin;
   glMajorVersion = org.glMajorVersion;
   glMinorVersion = org.glMinorVersion;
   compatibility = org.compatibility;
   benchmark = org.benchmark;
}

//------------------------------------------------------------------------------
// deleteData() -- copy our objects data
//------------------------------------------------------------------------------
void Display::deleteData()
{
	myWin = 0;
}

// ---
// isWindowShutdown() - is our window closing (or closed)
// ---
bool Display::isWindowShutdown() const
{
	if (myWin != 0) return (glfwWindowShouldClose(myWin) != 0);
	else return true;
}

//-----------------------------------------------------------------------------
// Reset the display
//-----------------------------------------------------------------------------
void Display::reset()
{
   BaseClass::reset();
}

// ---
// cursorMove() - called when the mouse move callback is instantiated on this object
// ---
void Display::cursorMove(const double x, const double y)
{
   // get the last reported mouse button from the window, to see if it is a   
   // passive or active motion
   int mouseButton = glfwGetMouseButton(myWin, GLFW_MOUSE_BUTTON_LEFT);
   if (mouseButton == GLFW_PRESS) {
      mouseMotionEvent((int)x, (int)y);
      //std::cout << "MOUSE MOTION EVENT = " << x << ", " << y << std::endl;
   }
   else {
      passiveMotionEvent((int)x, (int)y);
      //std::cout << "PASSIVE MOTION EVENT = " << x << ", " << y << std::endl;
   }

}

//-----------------------------------------------------------------------------
// createWindow() -- create the main window
//-----------------------------------------------------------------------------
int Display::createWindow()
{
   GLint  vpX(0), vpY(0);                          // our initial viewport position
   GLsizei vpWidth(0), vpHeight(0);                // our initial viewport size
   getViewport(&vpX, &vpY, &vpWidth, &vpHeight);
   
   // Let's try to use OpenGL 3.2 or later, which will remove all deprecated functions, and
   // force us to use the 3.2+ core API.
   // check our gl major and minor version

   // antialiasing!
   if (isAntialiasing()) glfwWindowHint(GLFW_SAMPLES, 4);
   if (glMajorVersion > 0) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajorVersion);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinorVersion);
      if (glMajorVersion >= 3 && glMinorVersion >= 2) {
         if (compatibility) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
         }
         // load core only
         else glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      }
   }

	myWin = glfwCreateWindow(vpWidth, vpHeight, getName(), NULL, NULL);

   if (myWin == 0) {
      std::cout << "Display::createWindow() - gflwCreateWIndow FAILED!" << std::endl;
   }
   else {
      glfwSetWindowPos(myWin, vpX, vpY);
   
      // set our context quickly, then initialize glew
      glfwMakeContextCurrent(myWin);
      GLenum glewInitResult;
      glewExperimental = GL_TRUE;
	   glewInitResult = glewInit();

	   if (GLEW_OK != glewInitResult) {
		   std::cerr << "ERROR: " << glewGetErrorString(glewInitResult) << std::endl;
		   exit(EXIT_FAILURE);
	   }
      else {
         if (glMajorVersion > 0) {
            std::cout << "INFO: OpenGL version requested = " << glMajorVersion << "." << glMinorVersion << std::endl;
         }
         std::cout << "INFO: Actual OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
         if (compatibility) std::cout << "INFO: compatibility extensions supported." << std::endl;



      }

      // generate and create our vertex array 
      GLuint myArrayID = 0;
      glGenVertexArrays(1, &myArrayID);
      setVertexArrayID(myArrayID);
	   glBindVertexArray(myArrayID);

      // create our shaders - this HAS to be done after our context is setup.
      Eaagles::Basic::PairStream* myShaders = getShaders();
      if (myShaders != 0) {
         Eaagles::Basic::List::Item* item = myShaders->getFirstItem();
         while (item != 0) {
            Eaagles::Basic::Pair* pair = (Eaagles::Basic::Pair*)item->getValue();
            if (pair != 0) {
               BasicGL2_0::Shader* shdr = dynamic_cast<BasicGL2_0::Shader*>(pair->object());
               if (shdr != 0) shdr->createAndLinkShaders();
            }
            item = item->getNext();
         }
      }
   }

   // load our textures
   loadTextures();

   // are we benchmarking?
   if (benchmark) {
      double startAngle = 0;
      double startSpeed = 0;
      Eaagles::Basic::Color* color = new Eaagles::Basic::Color();
      color->setRed(1);
      color->setBlue(1);
      color->setGreen(1);
      for (int i = 0; i < 1000; i++) {
         TestOne* one = new TestOne();
         one->setStartAngle(startAngle++);
         one->setSpeed(4);
         one->setColor(color);
         Basic::Pair* pair = new Basic::Pair("t", one);
         addComponent(pair);
         pair->unref();
         one->unref();
      }
      color->unref();
   }

	return 0;
}

//------------------------------------------------------------------------------
// select() -- select this display
//------------------------------------------------------------------------------
void Display::select()
{
   if (myWin != 0) glfwMakeContextCurrent(myWin);
}

//------------------------------------------------------------------------------
// swapbuffers() --
//------------------------------------------------------------------------------
void Display::swapBuffers()
{
	glfwSwapBuffers(myWin);
}

// SLOT FUNCTIONS
// ---
bool Display::setSlotMajorVersion(const Eaagles::Basic::Number* const x)
{
   bool ok = false;
   if (x != 0) {
      glMajorVersion = x->getInt();
      ok = true;
   }

   return ok;
}
bool Display::setSlotMinorVersion(const Eaagles::Basic::Number* const x)
{
   bool ok = false;
   if (x != 0) {
      glMinorVersion = x->getInt();
      ok = true;
   }

   return ok;
}
bool Display::setSlotCompatibility(const Eaagles::Basic::Number* const x)
{
   bool ok = false;
   if (x != 0) {
      compatibility = x->getBoolean();
      ok = true;
   }

   return ok;
}
bool Display::setSlotBenchmark(const Basic::Number* const x)
{
   if (x != 0) benchmark = x->getBoolean();
   return true;
}


//-----------------------------------------------------------------------------
// specialEvent()
//-----------------------------------------------------------------------------
void Display::specialEvent(const int key)
{
   //if (key == GLUT_KEY_LEFT)           keyboardEvent(BACK_SPACE);
   //else if (key == GLUT_KEY_RIGHT)     keyboardEvent(FORWARD_SPACE);
   //else if (key == GLUT_KEY_UP)        keyboardEvent(UP_ARROW_KEY);
   //else if (key == GLUT_KEY_DOWN)      keyboardEvent(DOWN_ARROW_KEY);
   //else if (key == GLUT_KEY_PAGE_UP)   keyboardEvent(PAGE_UP_KEY);
   //else if (key == GLUT_KEY_PAGE_DOWN) keyboardEvent(PAGE_DOWN_KEY);
   //else if (key == GLUT_KEY_HOME)      keyboardEvent(HOME_KEY);
   //else if (key == GLUT_KEY_END)       keyboardEvent(END_KEY);
   //else if (key == GLUT_KEY_INSERT)    keyboardEvent(INSERT_KEY);
   //else if (key == GLUT_KEY_F1)        keyboardEvent(F1_KEY);
   //else if (key == GLUT_KEY_F2)        keyboardEvent(F2_KEY);
   //else if (key == GLUT_KEY_F3)        keyboardEvent(F3_KEY);
   //else if (key == GLUT_KEY_F4)        keyboardEvent(F4_KEY);
   //else if (key == GLUT_KEY_F5)        keyboardEvent(F5_KEY);
   //else if (key == GLUT_KEY_F6)        keyboardEvent(F6_KEY);
   //else if (key == GLUT_KEY_F7)        keyboardEvent(F7_KEY);
   //else if (key == GLUT_KEY_F8)        keyboardEvent(F8_KEY);
   //else if (key == GLUT_KEY_F9)        keyboardEvent(F9_KEY);
   //else if (key == GLUT_KEY_F10)       keyboardEvent(F10_KEY);
   //else if (key == GLUT_KEY_F11)       keyboardEvent(F11_KEY);
   //else if (key == GLUT_KEY_F12)       keyboardEvent(F12_KEY);
   //else { /* ignore */ }
}



//-----------------------------------------------------------------------------
// Mouse motion (with either key pressed) event handler
//-----------------------------------------------------------------------------
void Display::mouseMotionEvent(const int x, const int y)
{    
   //setMouse(x,y);
}


//-----------------------------------------------------------------------------
// passive mouse motion (no mouse key pressed) event handler
//-----------------------------------------------------------------------------
void Display::passiveMotionEvent(const int x, const int y)
{    
   // set our mouse to the current position and update our number of clicks
   // do another pick, just to make sure
   //setMouse(x,y);
}


// ---
// mouse button handler
// ---
void Display::mouseButtonEvent(const int button, const int state, const int mods)
{    
   std::cout << "MOUSE BUTTON, STATE, MODS = " << button << ", " << state << ", " << mods << std::endl;
   // find a graphic with the name "testGraphic1, and rotate it"
   Eaagles::Basic::Pair* pair = findByName("testGraphic1");
   if (pair != 0) {
      Eaagles::BasicGL2_0::Graphic* g = dynamic_cast<Eaagles::BasicGL2_0::Graphic*>(pair->object());
      if (g != 0) {

         // rotation test
         g->rotateModelMatrix(35.0, 0.0, 0.0);
         // translation test
         //g->translateModelMatrix(0.09, 0.09);
         // scale test
         //g->scaleModelMatrix(1.5f, 1.5f, 1.0f);
      }
   }

}
void Display::keyboardEvent(const int key, const int state, const int mods)
{    
   std::cout << "KEYBOARD , STATE, MODS = " << key << ", " << state << ", " << mods << std::endl;
   if (key >= GLFW_KEY_SPACE && key <= GLFW_KEY_WORLD_2 && state == GLFW_RELEASE) {
      // GLFW doesn't differenciate between lower and upper characters, so we have to convert it ourself.
      // It defaults to upper case.
      if ((mods & GLFW_MOD_SHIFT) == 0) {
         int newKey = key + 32;
         // convert to lower
         BaseClass::keyboardEvent(newKey);
      }
      else BaseClass::keyboardEvent(key);

//      int numMons = 0;
//      GLFWmonitor** mons = glfwGetMonitors(&numMons);
//      int xPos = 0, yPos = 0;
//      int width = 0, height = 0;
//      for (int i = 0; i < numMons; i++) {
//         std::cout << "MONITOR " << i+1 << " : " << std::endl;
//         glfwGetMonitorPos(mons[i], &xPos, &yPos);
//         std::cout << "VIEWPORT POSITION = " << xPos << ", " << yPos << std::endl;
//         glfwGetMonitorPhysicalSize(mons[i], &width, &height);
//         std::cout << "PHYSICAL SIZE (MM) = " << width << "mm by " << height << "mm" << std::endl;
//         const char* name = glfwGetMonitorName(mons[i]);
//         std::cout << "MONITOR NAME = " << name << std::endl;
//         int numModes = 0;
//         const GLFWvidmode* mode = glfwGetVideoModes(mons[i], &numModes);
//         std::cout << "VIDEO MODES SUPPORTED: " << std::endl;
//         for (int j = 0; j < numModes; j++) {
//            std::cout << "REFRESH RATE = " << mode->refreshRate << std::endl;
//            std::cout << "(" << mode->width << " x " << mode->height << ")" << std::endl;
//            std::cout << "RGB Depth = " << mode->redBits << ", " << mode->greenBits << ", " << mode->blueBits << std::endl;
//         }
//      }
   }
}
void Display::closeEvent()
{
   std::cout << "DISPLAY IS CLOSING NOW" << std::endl;
}
void Display::focus(const int state)
{
   if (state == GL_TRUE) std::cout << "DISPLAY GOT FOCUS!" << std::endl;
   else std::cout << "DISPLAY LOST FOCUS!" << std::endl;
}

//------------------------------------------------------------------------------
// getSlotByIndex() - needed for slottable
//------------------------------------------------------------------------------
Basic::Object* Display::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}

} 
} 
