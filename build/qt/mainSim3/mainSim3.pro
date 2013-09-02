# mainSim3 OpenEaaglesExample
include(../config.pri)

# our root
MY_ROOT = $$HOME_ROOT/mainSim3

# Configuration settings
TEMPLATE = app
CONFIG += console

# destination and working directories
win32: DESTDIR = $${MY_ROOT}
else:unix:!macx:!symbian: DESTDIR = $${MY_ROOT}

OBJECTS_DIR = ./tmp/obj
MOC_DIR = ./tmp/moc
RCC_DIR = ./tmp/rcc

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
    -loeInstruments                                   \
    -loeBasicGL -loeBasic                           \
    -loeIoDevice                                      \
    -loeDis -loeSensors -loeVehicles              \
    -loeSimulation -loeDafif                        \
    -loeGlut -loeOtw                                \
    # oe 3rd party
    -L$${OE_3RD_PARTY_ROOT}/lib/$${MSVC_VER}-32/        \
    -lftgl -lfreetype2                              \
    -lfreeglut                                        \
    -lJSBSim                                          \
    -lccl_lib                                             \
    # system
    -lwinmm                                             \
    -lws2_32                                            \
    -ladvapi32


# Windows (MSVC) Debug libraries
else:win32:CONFIG(debug, debug|release): LIBS +=        \
    # open eaagles
    -L$${OE_ROOT}/lib/$${MSVC_VER}/                     \
    -loeInstruments_d                                   \
    -loeBasicGL_d -loeBasic_d                           \
    -loeIoDevice_d                                      \
    -loeDis_d -loeSensors_d -loeVehicles_d              \
    -loeSimulation_d -loeDafif_d                        \
    -loeGlut_d -loeOtw_d                                \
    # oe 3rd party
    -L$${OE_3RD_PARTY_ROOT}/lib/$${MSVC_VER}-32/        \
    -lftgl_d -lfreetype2_d                              \
    -lfreeglut_d                                        \
    -lJSBSim_d                                          \
    -lccl_lib_d                                             \
    # system
    -lwinmm                                             \
    -lws2_32                                            \
    -ladvapi32

# Linux libraries
else:unix:!macx:!symbian: LIBS +=                   \
    # open eaagles
    -L$${OE_ROOT}/lib/linux/                        \
    -loeBasicGL -loeInstruments                          \
    -loeSimulation -loeBasic -loeSensors  -loeDafif \
    -loeVehicles \
    -loeOtw \
    -loeDis \
    -loeIoDevice \
    -loeGlut \
    # oe 3rd party
    -L$${OE_3RD_PARTY_ROOT}/lib/linux               \
    -lJSBSim  \
    -lcigicl \
    -lftgl -lfreetype -lglut                        \
    # system
    -lGLU -lGL -lX11                                \
    -lglut -lGLU -lGL -lX11 -lpthread -lrt
