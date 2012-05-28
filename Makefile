PROGRAMS = grab  
DOCUMENTATIONS = doc

version = v0.0.0
#OPT = -Dcimg_display=0 -Dcimg_debug=2 -Dcimg_use_vt100 -DVERSION=\"$(version)\"
OPT = -O0 -Dcimg_debug=2 -Dcimg_use_vt100 -DVERSION=\"$(version)\" -I.. -Wall -W -lm -ansi -pedantic -fno-tree-pre -Dcimg_use_vt100 -I/usr/X11R6/include -Dcimg_use_xshm -L/usr/X11R6/lib -lpthread -lX11 -lXext
CC = gcc
CPP = g++

all: $(PROGRAMS) $(DOCUMENTATIONS)

prog:$(PROGRAMS)

grab: grab.cpp grab.h
	$(CPP) $(OPT) grab.cpp -o $@

doc: grab.Doxygen grab.cpp grab.h
	echo ${version} > VERSION
	./doxIt.sh

clean:
	rm -rf $(DOCUMENTATIONS)/*
	rm -f *.o
	rm -rf .libs
	@list='$(PROGRAMS)'; for p in $$list; do \
	  rm -f $$p ; \
	done


