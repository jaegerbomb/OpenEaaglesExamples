#
include ../makedefs

PGM = mainCockpit

OBJS  = SimStation.o TestDisplay.o
OBJS += TestIoHandler.o
OBJS += Factory.o main.o

LDFLAGS += -L../lib/linux -lxZeroMQHandlers -lzmq
LDFLAGS += -lxPanel
LDFLAGS += -loeRecorder -loeSensors -loeDynamics
LDFLAGS += -loeSimulation -loeInstruments -loeDis
LDFLAGS += -loeIoDevice -loeOtw -loeDafif
LDFLAGS += -loeGlut -loeBasicGL -loeBasic
LDFLAGS += -lprotobuf -lJSBSim
LDFLAGS += -lftgl -lfreetype -lcigicl 
LDFLAGS += -lglut -lGLU -lGL -lX11
LDFLAGS += -lpthread -lrt

CPPFLAGS += -I/usr/local/include
LDFLAGS  += -L../lib/linux -L$(OPENEAAGLES_LIB_DIR)

EPPFLAGS += -I.

all: $(PGM)

$(PGM): $(OBJS)
	$(CXX) -o $(PGM) $(OBJS) $(LDFLAGS)

edl:
	cpp configs/test1.epp >test1.edl   $(EPPFLAGS)

clean:
	-rm -f *.o
	-rm -f $(PGM)
