# /********************************************************************************************
#  * File:		$URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Trunk/Makefile.Unix $
#  * Author:		$LastChangedBy: OBTUSE\matthew $
#  * Revision:		$Revision: 1071 $
#  * Last Updated:	$LastChangedDate: 2007-08-07 16:37:44 -0400 (Tue, 07 Aug 2007) $
#  ********************************************************************************************/

LIB_SOURCE=\
	Object.cpp Type.cpp Exception.cpp PlatformException.cpp \
	Debug/Assert.cpp Debug/Debug.cpp Debug/Trace.cpp \
	Text/String.cpp \
	Time/DateTime.cpp Time/Timer.cpp \
	IO/File.cpp IO/Stream.cpp IO/Path.cpp IO/Pipe.cpp IO/StdIO.cpp IO/MemoryMappedFile.cpp IO/BinaryStream.hpp IO/SerialPort.hpp IO/MemoryStream.hpp \
	System/CommandLineParser.cpp System/Application.cpp System/Module.cpp System/Process.cpp System/Version.cpp System/Configuration.cpp \
	System/Console.cpp System/ConsoleApplication.hpp \
	System/Threading/Mutex.cpp System/Threading/CriticalSection.cpp System/Threading/Thread.cpp \
	Net/Socket.cpp Net/IPAddress.cpp Net/IPEndPoint.cpp Net/SocketStream.cpp \
	Graphics/Color.cpp \
	Memory/Memory.cpp 

ROBOT_SOURCE=\
	DeviceControl.cpp Image.cpp LADARControl.cpp LADARMotorControl.cpp Map.cpp MapGeneration.cpp MotorEncoder.cpp MotorControl.cpp Path.cpp PathPlanning.cpp RangeImage.cpp RobotCommand.cpp RobotMotion.cpp RobotNavigation.cpp RobotScanning.cpp RobotVision.cpp Subsystem.cpp TemperatureControl.cpp RobotServer.cpp

ENCODERTEST_SOURCE=\
	MotorControl.cpp MotorEncoder.cpp MotorEncoderTest.cpp

CONTROLLERTEST_SOURCE=\
	MotorControl.cpp MotorEncoder.cpp MotorControllerTest.cpp

LADARTEST_SOURCE=\
	Map.cpp LADARControl.cpp LADARMotorControl.cpp LADARTest.cpp RangeImage.cpp


CFLAGS += -fpermissive -I./Src -I./Library/Src -pthread
LFLAGS += -pthread -ldl -rdynamic

CFLAGS_DEBUG = -DDEBUG -g3
LFLAGS_DEBUG =

CFLAGS_RELEASE = -O3 -DRELEASE -funroll-loops -ffast-math
LFLAGS_RELEASE = -O3 

SRC = Src/
LIBSRC = Library/Src/
BIN = Bin/Linux/
INCLUDE = /usr/include/

CC=gcc
CXX=g++

ROBOT_OBJECTS=$(addprefix $(BIN), $(addsuffix .o, $(basename $(ROBOT_SOURCE))))
CONTROLLERTEST_OBJECTS=$(addprefix $(BIN), $(addsuffix .o, $(basename $(CONTROLLERTEST_SOURCE))))
ENCODERTEST_OBJECTS=$(addprefix $(BIN), $(addsuffix .o, $(basename $(ENCODERTEST_SOURCE))))
LADARTEST_OBJECTS=$(addprefix $(BIN), $(addsuffix .o, $(basename $(LADARTEST_SOURCE))))
LIB_OBJECTS=$(addprefix $(BIN), $(addsuffix .o, $(basename $(LIB_SOURCE))))

all: CFLAGS += $(CFLAGS_DEBUG)
all: LFLAGS += $(LFLAGS_DEBUG)
all: $(BIN)Robot $(BIN)MotorControllerTest $(BIN)MotorEncoderTest $(BIN)LADARTest

release: CFLAGS += $(CFLAGS_RELEASE)
release: LFLAGS += $(LFLAGS_RELEASE)
release: $(BIN)Robot $(BIN)MotorControllerTest $(BIN)MotorEncoderTest $(BIN)LADARTest

clean:
	@echo Cleaning...
	@rm -f $(BIN)Robot $(BIN)MotorControllerTest $(BIN)MotorEncoderTest $(BIN)LADARTest
	@rm -f $(ROBOT_OBJECTS) $(LIB_OBJECTS) $(ENCODERTEST_OBJECTS) $(CONTROLLERTEST_OBJECTS) $(LADARTEST_OBJECTS)

update:
	@echo Updating SVN...
	@svn update


$(BIN)Robot: $(ROBOT_OBJECTS) $(LIB_OBJECTS)
	@echo Linking $@...
	@$(CXX) -o $@ $(ROBOT_OBJECTS) $(LIB_OBJECTS) $(LFLAGS)

$(BIN)MotorControllerTest: $(CONTROLLERTEST_OBJECTS) $(LIB_OBJECTS)
	@echo Linking $@...
	@$(CXX) -o $@ $(CONTROLLERTEST_OBJECTS) $(LIB_OBJECTS) $(LFLAGS)

$(BIN)MotorEncoderTest: $(ENCODERTEST_OBJECTS) $(LIB_OBJECTS)
	@echo Linking $@...
	@$(CXX) -o $@ $(ENCODERTEST_OBJECTS) $(LIB_OBJECTS) $(LFLAGS)

$(BIN)LADARTest: $(LADARTEST_OBJECTS) $(LIB_OBJECTS)
	@echo Linking $@...
	@$(CXX) -o $@ $(LADARTEST_OBJECTS) $(LIB_OBJECTS) $(LFLAGS)

$(BIN)%.o: $(SRC)%.c
	@echo Compiling $(<F)...
	@mkdir -p $(@D)
	@$(CC) -c -o $@ $(CFLAGS) -I$(INCLUDE) $<

$(BIN)%.o: $(SRC)%.cpp
	@echo Compiling $(<F)...
	@mkdir -p $(@D)
	@$(CXX) -c -o $@ $(CFLAGS) -I$(INCLUDE) $<

$(BIN)%.o: $(LIBSRC)%.c
	@echo Compiling $(<F)...
	@mkdir -p $(@D)
	@$(CC) -c -o $@ $(CFLAGS) -I$(INCLUDE) $<

$(BIN)%.o: $(LIBSRC)%.cpp
	@echo Compiling $(<F)...
	@mkdir -p $(@D)
	@$(CXX) -c -o $@ $(CFLAGS) -I$(INCLUDE) $<
