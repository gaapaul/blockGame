CPP = g++
CPPFLAGS =  -I./../include/ -I/usr/include/glm/ -I/usr/include/GLFW/ -I/usr/include/freetype2
LDFLAGS = 
LDLIBS = -lglfw -lfreetype -lGL -lX11 -lXrandr -lXi -ldl

SRCF = main.cpp stb_image.cpp shader.cpp spriteRender.cpp tile.cpp gamescreen.cpp gamepiece.cpp game.cpp key.cpp glad.cpp renderText.cpp texture.cpp
OBJS = $(patsubst %.cpp, %.o, $(SRCF))

all: block

block: $(OBJS)
		$(CPP) $(LDFLAGS) -o block $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCF)
		rm -f ./.depend
		$(CPP) $(CPPFLAGS) -MM $^>>./.depend;

clean:
		rm -f $(OBJS)
		rm -f block
