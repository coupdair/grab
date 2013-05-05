PROGRAMS = version lavision grab  
DOCUMENTATIONS = doc

version = v0.2.1.dev
#version_cimg = 125
version_cimg = 149
version_rs232=`cat ../rs232/VERSION`
OPT_LIBRARY = -DGRAB_VERSION=\"$(version)\" -DRS232_VERSION=\"$(version_rs232)\"
OPT_FORMAT = -Dcimg_use_jpeg -ljpeg -Dcimg_use_png -lpng -lz -Dcimg_use_tiff -ltiff -Dcimg_use_lavision -D_LINUX
OPT_NETCDF = -Dcimg_use_netcdf -I../NetCDF/include/ -lnetcdf_c++ -L../NetCDF/lib/ -lnetcdf -I/usr/include/netcdf-3/
OPT_XWINDOWS = -I/usr/X11R6/include -Dcimg_use_xshm -L/usr/X11R6/lib -lpthread -lX11 -lXext
##CImg.v1.2.5
#OPT = -Dcimg_display=0 -Dcimg_debug=2 -Dcimg_use_vt100 $(OPT_LIBRARY) $(OPT_FORMAT) -Dversion_cimg=$(version_cimg) $(OPT_NETCDF)
#OPT = $(OPT_XWINDOWS) -Dcimg_display=1 -Dcimg_debug=2 -Dcimg_use_vt100 $(OPT_LIBRARY) $(OPT_FORMAT) -Dversion_cimg=$(version_cimg) $(OPT_NETCDF)
##CImg.v1.4.9
OPT = -Dcimg_display=0 -Dcimg_debug=2 -Dcimg_use_vt100 $(OPT_LIBRARY) $(OPT_FORMAT) -Dversion_cimg=$(version_cimg)
OPT = $(OPT_XWINDOWS)  -Dcimg_debug=2 -Dcimg_use_vt100 $(OPT_LIBRARY) $(OPT_FORMAT) -Dversion_cimg=$(version_cimg)

CC = gcc
CPP = g++ -O0

all: $(PROGRAMS) $(DOCUMENTATIONS)

prog:$(PROGRAMS)

version: Makefile
	echo ${version} > VERSION
#	cd ..; rm CImg; ln -s CImg-1.2.5 CImg; cd ./grab/
	cd ..; rm CImg; ln -s CImg-1.4.9 CImg; cd ./grab/

lavision: Makefile
	cd ../LaVision/
#	make
	cd ../convert/
	cp -p ../LaVision/*.o ../LaVision/linux/*.o ../grab/obj/
	rm ../grab/obj/Read_Examples.o

grab: grab.cpp grab.h Makefile ./obj/ReadIM7.o
	$(CPP) grab.cpp -o $@ $(OPT) ./obj/*.o

doc: grab.Doxygen grab.cpp grab.h Makefile
	./doxIt.sh

clean:
	rm -rf $(DOCUMENTATIONS)/*
	rm -f *.o
	rm -rf .libs
	@list='$(PROGRAMS)'; for p in $$list; do \
	  rm -f $$p ; \
	done


