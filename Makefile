# Makefile for TBX Library

# include FixDeps:Rules/make

CXX=g++
CXXFLAGS=-O2 -Wall -mthrowback
CPPFLAGS=-MMD -MP
AR=ar

TARGET=libtbx.a

CCSRC = $(wildcard tbx/*.cc) $(wildcard tbx/view/*.cc) $(wildcard tbx/res/*.cc) $(wildcard tbx/doc/*.cc) $(wildcard tbx/ext/*.cc)


bin:	$(TARGET)

doc:
	doxygen tbx.doxyfile

all:	bin doc

$(TARGET): $(CCSRC:.cc=.o)
	$(AR) -rcs $(TARGET) $(CCSRC:.cc=.o)

clean:
	rm -f $(CCSRC:.cc=.o) $(CCSRC:.cc=.d) $(TARGET)

copytoapp: bin doc
	mkdir -p !TBX/tbx/h
	copy tbx.h.* !TBX.tbx.h.* ~CF
	mkdir -p !TBX/tbx/doc/h
	copy tbx.doc.h.* !TBX.tbx.doc.h.* ~CF
	mkdir -p !TBX/tbx/res/h
	copy tbx.res.h.* !TBX.tbx.res.h.* ~CF
	mkdir -p !TBX/tbx/view/h
	copy tbx.view.h.* !TBX.tbx.view.h.* ~CF
	mkdir -p !TBX/tbx/ext/h
	copy tbx.ext.h.* !TBX.tbx.ext.h.* ~CF
	mkdir -p !TBX/tbx/ext/res/h
	copy tbx.ext.res.h.* !TBX.tbx.ext.res.h.* ~CF
	copy libtbx/a !TBX.libtbx/a ~CF
	mkdir -p !TBX/html
	copy docs.html.* !TBX.html.* ~CF

cleanapp:
	rm -rf !TBX/tbx
	rm -rf !TBX/html
	IfThere !TBX.libtbx/a Then Delete !TBX.libtbx/a

install:
	./install-cross


-include $(CCSRC:.cc=.d)

