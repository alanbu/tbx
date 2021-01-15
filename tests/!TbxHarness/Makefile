# Makefile for TbxHarness application

CXX=g++
CXXFLAGS=-O2 -ITBX: -I.

LD=g++
LDFLAGS=-LTBX: -ltbx -static

TARGET=!RunImage
TARGETELF=tbxharness

SRC=main.cc mainwindow.cc eventlog.cc
OBJ=$(SRC:.cc=.o)

all: $(TARGET)

$(TARGET):	$(TARGETELF)
	elf2aif $(TARGETELF) $(TARGET)

$(TARGETELF):	$(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(TARGETELF)

main.o mainwindow.o: mainwindow.h
mainwindow.o: mainwindow.cc eventlog.h
eventlog.o: eventlog.cc eventlog.h

clean:
	rm -f $(OBJ) $(TARGETELF) $(TARGET)
