.SUFFIXES: .cc .c .o .cpp

# set CROSS to yes when cross-compiling. Leave empty or set to anything
# else for native building.
# CROSS=yes
CROSS=
PLATFORM=i386-mingw32

#all: hello3.exe
all: step01.exe step02.exe step03.exe step04.exe step05.exe\
   step06.exe step07.exe step08.exe step09.exe step10.exe\
	 step11.exe step12.exe


ifeq "$(strip $(CROSS))" "yes"
  prefix=/homes/src/mingw32/linux
  RM=rm -f
  DEV_NULL=/dev/null
  toolprefix=$(tooldir)-
else
  prefix=d:/gcc
  # prefix=g:/NT_MINGW32
  RM=del
  DEV_NULL=NUL 2>&1
  toolprefix=d:/gcc/bin/
  owlroot=e:/owl
endif

gcc_version=gcc-2.95
tooldir=i386-mingw32msvc

CC=$(toolprefix)gcc
CXX=$(toolprefix)c++
LD=$(toolprefix)ld
DLLTOOL=$(toolprefix)dlltool
AS=$(toolprefix)as

DLLTOOL_FLAGS = --as=$(AS) -k

WINDRES = $(toolprefix)windres
#WINDRES_FLAGS = --include-dir $(prefix)/$(tooldir)/include
WINDRES_FLAGS = --include-dir $(owlroot)/include

gcclib=$(prefix)/lib/gcc-lib/$(tooldir)/$(gcc_version)
DLLCRT0=$(prefix)/$(tooldir)/lib/dllcrt0.o
LDFLAGS=-L$(prefix)/lib -L$(prefix)/$(tooldir)/lib -L$(gcclib) -L$(owlroot)/lib
CFLAGS=-Wl, -mwindows -DSUPPRESS_OLE -D_OWLPCH -I$(owlroot)/include

#
# The only extra library needed is libuser32.a. The rest are supplied
# by the respective compiler driver (eg., c++).
#
LIBS=-luser32 -lkernel32 -lstdc++ -lowlwf -lgdi32 -luuid

#hello3.exe: hello3.cpp hellores.rc 
#	$(CXX) -o hello3.exe step01.cpp hellores.o $(CFLAGS) $(LIBS) $(LDFLAGS)

step01.exe: step01.cpp 
	$(CXX) -o step01.exe step01.cpp $(CFLAGS) $(LIBS) $(LDFLAGS)

step02.exe: step02.cpp 
	$(CXX) -o step02.exe step02.cpp $(CFLAGS) $(LIBS) $(LDFLAGS)

step03.exe: step03.cpp 
	$(CXX) -o step03.exe step03.cpp $(CFLAGS) $(LIBS) $(LDFLAGS)

step04.exe: step04.cpp 
	$(CXX) -o step04.exe step04.cpp $(CFLAGS) $(LIBS) $(LDFLAGS)

step05.exe: step05.cpp step05r.o
	$(CXX) -o step05.exe step05.cpp step05r.o $(CFLAGS) $(LIBS) $(LDFLAGS)

step06.exe: step06.cpp step06r.o
	$(CXX) -o step06.exe step06.cpp step06r.o $(CFLAGS) $(LIBS) $(LDFLAGS)

step07.exe: step07.cpp step07r.o
	$(CXX) -o step07.exe step07.cpp step07r.o $(CFLAGS) $(LIBS) $(LDFLAGS)

step08.exe: step08.cpp step08r.o
	$(CXX) -o step08.exe step08.cpp step08r.o $(CFLAGS) $(LIBS) $(LDFLAGS)

step09.exe: step09.cpp step09r.o
	$(CXX) -o step09.exe step09.cpp step09r.o $(CFLAGS) $(LIBS) $(LDFLAGS)

step10.exe: step10.cpp step10r.o
	$(CXX) -o step10.exe step10.cpp step10r.o $(CFLAGS) $(LIBS) $(LDFLAGS)

step11.exe: step11.cpp step11r.o
	$(CXX) -o step11.exe step11.cpp step11r.o $(CFLAGS) $(LIBS) $(LDFLAGS)

step12.exe: step12.cpp step12r.o
	$(CXX) -o step12.exe step12.cpp step12r.o $(CFLAGS) $(LIBS) $(LDFLAGS)







step05r.o: step05.rc
	$(WINDRES) $(WINDRES_FLAGS) -i step05.rc -o step05r.o

step06r.o: step06.rc
	$(WINDRES) $(WINDRES_FLAGS) -i step06.rc -o step06r.o

step07r.o: step07.rc
	$(WINDRES) $(WINDRES_FLAGS) -i step07.rc -o step07r.o

step08r.o: step08.rc
	$(WINDRES) $(WINDRES_FLAGS) -i step08.rc -o step08r.o

step09r.o: step09.rc
	$(WINDRES) $(WINDRES_FLAGS) -i step09.rc -o step09r.o

step10r.o: step10.rc
	$(WINDRES) $(WINDRES_FLAGS) -i step10.rc -o step10r.o

step11r.o: step11.rc
	$(WINDRES) $(WINDRES_FLAGS) -i step11.rc -o step11r.o

step12r.o: step12.rc
	$(WINDRES) $(WINDRES_FLAGS) -i step12.rc -o step12r.o

.o: .rc
	$(WINDRES) -i $< -o $@

.cc.o:
	$(CXX) -c $(CFLAGS) $<

.cpp.o:
	$(CXX) -c $(CFLAGS) $<

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	$(RM) *.dll *.o *.a *.exe *.exp *.base > $(DEV_NULL)

