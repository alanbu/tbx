# Makefile for TbxURI

# If you have fix deps uncomment following line for dependency scanning
# include FixDeps:Rules/make

CXXFLAGS = -O2 -Wall -mpoke-function-name -mthrowback -ITBX:

LD = g++
LDFLAGS = -LTBX: -ltbx -lstdc++ -static

CCSRC = $(wildcard *.cc)
OBJECTS = $(CCSRC:.cc=.o)

^.!RunImage:	!RunImage_e1f
	elf2aif !RunImage_e1f ^.!RunImage
!RunImage_e1f:	$(OBJECTS)
	$(LD) $(LDFLAGS) -o !RunImage_e1f $(OBJECTS)

# IF you have fix deps uncomment following line
# #include $(CCSRC:.cc=.d)
