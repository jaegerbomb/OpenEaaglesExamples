
#include <iostream>
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Color.h"
#include "openeaagles/basic/Parser.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/basicFF.h"
#include "MyObj.h"

namespace Eaagles {
namespace Example05 {

// Description (input) File
// After being processed by the C preprocessor
const char* inputFileName = "file0.edl";

static class MyObj* sys = 0;

static Basic::Object* exampleFormFunc(const char* const formname)
{
  Basic::Object* newform = 0;

  // look in application's classes
  if ( strcmp(formname, MyObj::getFormName()) == 0 ) {
    newform = new MyObj;
  }
  // look in base classes
  if (newform == 0) newform = Basic::basicFormFunc(formname);
  return newform;
}

// read and parse input file
static void readInput()
{
  // Read the description file
  int errors = 0;
  Basic::Object* q1 = lcParser(inputFileName, exampleFormFunc, &errors);
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
      q1 = pp->object();
    }
    sys = dynamic_cast<MyObj*>(q1);
  }

  // Make sure we did get a valid object (we must have one!)
  if (sys == 0) {
    std::cout << "example: invalid description file!" << std::endl;
    exit(1);
  }
}

int exec(int, char **)
{
  // parse and read input file
  readInput();

  // print out some color information
  const Basic::PairStream* colorTable = sys->getColorTable();
  if(colorTable != 0) {
//    Pair* p = colorTable->findByName("green");
    const Basic::Identifier* id = sys->getTextColor();
    if (id != 0) {
      const Basic::Pair* p = colorTable->findByName(id->getString());
      if (p != 0) {
        std::cout << "Text color: " << id->getString();
        const Basic::Color* color = dynamic_cast<const Basic::Color*>(p->object());
        if (color != 0) {
          std::cout << " Red: "   << color->red();
          std::cout << " Green: " << color->green();
          std::cout << " Blue: "  << color->blue();
        }
        std::cout << std::endl;
      } else {
        std::cout << "Text color not found\n" << std::endl;
      }
    }
  }

  // print out vector information
  const Basic::List* vector = sys->getVector();
  if (vector != 0) {
    int numValues = vector->entries();
    int* values = new int[numValues];
    int n = vector->getNumberList(values,numValues);
    std::cout << "Vector: ";
    std::cout << "# Numeric Entries: " << n << " Values: ";
    for(int i=0; i < n; i++)
      std::cout << values[i] << " ";
    std::cout << std::endl;
    delete[] values;
  }

  // print out visible and message info
  std::cout << "Visible: " << sys->getVisible() << "\n";
  const Basic::String* message = sys->getMessage();
  std::cout << "Message: " << message->getString() << "\n";

  sys->unref();

  return 0;
}

} // namespace Example05
} // namespace Eaagles

//-----------------------------------------------------------------------------
// main() -- Main routine
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  Eaagles::Example05::exec(argc, argv);
}