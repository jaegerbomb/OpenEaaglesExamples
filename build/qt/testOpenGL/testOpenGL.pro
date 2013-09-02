# testOpenGL OpenEaaglesExample
include(../config.pri)

# our root
MY_ROOT = $$HOME_ROOT/testOpenGL

# Configuration settings
TEMPLATE = app
CONFIG += console

# destination and working directories
win32: DESTDIR = $${MY_ROOT}
else:unix:!macx:!symbian: DESTDIR = $${MY_ROOT}

OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
RCC_DIR = ./tmp/rcc

# Using static GLEW library
DEFINES += GLEW_STATIC

# include paths
INCLUDEPATH +=                          \
   $${OE_ROOT}/include                  \
   $${OE_3RD_PARTY_ROOT}/include

# source files
SOURCES += \
   $${MY_ROOT}/*.cpp

# header files
HEADERS += \
   $${MY_ROOT}/*.h

# other files
OTHER_FILES += \
   $${MY_ROOT}/inputs/*.epp         \


# Windows (MSVC) release libraries
win32:CONFIG(release, debug|release): LIBS +=           \
    # open eaagles
    -L$${OE_ROOT}/lib/$${MSVC_VER}/                     \
    -loeBasicGL2.0  -loeBasic                               \
    # oe 3rd party
    -L$${OE_3RD_PARTY_ROOT}/lib/$${MSVC_VER}-32/        \
    -lglfw                                              \
    # system
    -lwinmm                                             \
    -lws2_32                                            \
    -ladvapi32

# Windows (MSVC) Debug libraries
else:win32:CONFIG(debug, debug|release): LIBS +=        \
    # open eaagles
    -L$${OE_ROOT}/lib/$${MSVC_VER}/                     \
    -loeBasicGL2.0_d  -loeBasic_d          \
    -lftgl_d -lfreetype2_d                              \
    # oe 3rd party
    -L$${OE_3RD_PARTY_ROOT}/lib/$${MSVC_VER}-32/        \
    -lglew32s_d                                         \
    -lglfw3_d                                           \
    # system
    -lwinmm                                             \
    -lws2_32                                            \
    -ladvapi32

# Linux libraries
else:unix:!macx:!symbian: LIBS +=                   \
    # open eaagles
    -L$${OE_ROOT}/lib/linux/                        \
    -loeBasicGL -loeBasic                           \
    # oe 3rd party
    -L$${OE_3RD_PARTY_ROOT}/lib/linux               \
    -lftgl -lfreetype -lglut                        \
    # system
    -lGLU -lGL -lX11                                \
    -lglut -lGLU -lGL -lX11 -lpthread -lrt


