all: calib

CFLAGS = -W -Werror -fPIC -g `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`
INCLUDE = -I/usr/local/include/libfreenect
FREE_LIBS = -L/usr/local/lib -lfreenect
SRC = main.cpp CvKinect.cpp Mutex.cpp Area.cpp
OBJ = $(SRC:%.c=%.o)

calib: $(OBJ)
	$(CXX) $(INCLUDE) $(CFLAGS) $? -o $@  $(LIBS) $(FREE_LIBS)

%.o: %.cpp
	$(CXX) -c $(INCLUDE) $(CFLAGS) $< -o $@

clean:
	rm -rf  *.o

distclean: clean
	rm -rf calib
