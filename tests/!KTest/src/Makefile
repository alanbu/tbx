# Makefile for KTest

CXXFLAGS= -O2 -Wall -mpoke-function-name -mthrowback -ITBX:

LD= g++
LDFLAGS= -LTBX: -ltbx -lstdc++ -static

CCSRC = KTestWnd.cc main.cc
OBJECTS = $(CCSRC:.cc=.o)

^.!RunImage:	!RunImageElf
	elf2aif !RunImageElf ^.!RunImage

!RunImageElf:	$(OBJECTS)
	$(LD) $(LDFLAGS) -o !RunImageElf $(OBJECTS)

clean:
	IFThere o.KTestWnd Then Delete o.KTestWnd
	IfThere o.main Then Delete o.main
	IfThere !RunImageElf Then Delete !RunImageElf

KTestWnd.cc:	KTestWnd.h
main.cc:	KTestWnd.h
