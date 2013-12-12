# Makefile for PackMan unit tests

# Detect host
HOST := $(shell uname -s)
ifeq ($(HOST),)
  # Assume RISC OS
  HOST := riscos
else
  ifeq ($(HOST),RISC OS)
    HOST := riscos
  endif
endif

ifeq ($(HOST),riscos)
  include FixDeps:Rules/make
  INC_DIRS= -ITBX:
  LIB_DIRS= -LTBX:
else
  INC_DIRS = -I..
  LIB_DIRS = -L..
endif

CXX=g++
CPPFLAGS= $(INC_DIRS)
CXXFLAGS= -O2 -Wall -mpoke-function-name -mthrowback

LD = g++

LDFLAGS = $(LIB_DIRS) -ltbx \
          -lcppunit -lstdc++ -static

ELFTARGET=unittestself

ifeq ($(HOST),riscos)
  TARGET=unittests
else
  TARGET=unittests,ff8
  LDFLAGS:=$(LDFLAGS) -L../../tbx/Debug
endif

CCSRC = $(wildcard *.cc)
OBJECTS = $(CCSRC:.cc=.o)

all:	$(TARGET)

$(TARGET): $(ELFTARGET)
	elf2aif $(ELFTARGET) $(TARGET)

$(ELFTARGET):	$(OBJECTS)
	$(LD) $(LDFLAGS) -o $(ELFTARGET) $(OBJECTS)

ifeq ($(HOST),riscos)
include $(CCSRC:.cc=.d)
endif

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)
	rm -f $(ELFTARGET)
