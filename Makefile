CFLAGS = -W -Wall -Werror -std=c99 -fPIC -g `pkg-config --cflags opencv`
TARGET = calib
LIBS = `pkg-config --libs opencv`
INCLUDE = -I/usr/local/include/libfreenect
FREE_LIBS = -L/usr/local/lib -lfreenect
SRC = main.c
OBJ = $(SRC:%.c=%.o)

$(TARGET): $(OBJ)
	$(CXX) $(INCLUDE) $(CFLAGS) $? -o $@  $(LIBS) $(FREE_LIBS)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm -rf  $(TARGET)-sol *.o

distclean: clean
	rm -rf $(TARGET)
