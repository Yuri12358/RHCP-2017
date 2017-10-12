bin=bin/app
bin_debug=debug/bin/app
objs=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/*.cpp))
objs_debug=$(patsubst src/%.cpp,debug/obj/%.o,$(wildcard src/*.cpp))
libs=-ltgui -lsfml-graphics -lsfml-window -lsfml-system
hdr=include/Prjoct2/

all: bin obj debug debug/bin debug/obj $(bin) $(bin_debug)

bin:
	mkdir bin

obj:
	mkdir obj

debug:
	mkdir debug

debug/bin:
	mkdir debug/bin

debug/obj:
	mkdir debug/obj

$(bin): $(objs)
	g++ $(objs) $(libs) -o $(bin)

$(bin_debug): $(objs_debug)
	g++ $(objs_debug) $(libs) -o $(bin_debug)

obj/Main.o: src/Main.cpp \
	obj/App.o
	g++ -c $< -Iinclude -o $@
	g++ -c -g $< -Iinclude -o debug/$@

obj/App.o: src/App.cpp $(hdr)App.hpp \
	obj/JSONHolder.o \
	obj/TextureHolder.o \
	obj/QuadTree.o \
	obj/ComponentRenderer.o \
	obj/GUIHolder.o \
	obj/History.o
	g++ -c $< -Iinclude -o $@
	g++ -c -g $< -Iinclude -o debug/$@

obj/TextureHolder.o: src/TextureHolder.cpp $(hdr)TextureHolder.hpp
	g++ -c $< -Iinclude -o $@
	g++ -c -g $< -Iinclude -o debug/$@

obj/JSONHolder.o: src/JSONHolder.cpp $(hdr)JSONHolder.hpp
	g++ -c $< -Iinclude -o $@
	g++ -c -g $< -Iinclude -o debug/$@

obj/ComponentInfo.o: src/ComponentInfo.cpp $(hdr)ComponentInfo.hpp \
	obj/Direction.o
	g++ -c $< -Iinclude -o $@
	g++ -c -g $< -Iinclude -o debug/$@

obj/QuadTree.o: src/QuadTree.cpp $(hdr)QuadTree.hpp \
	obj/ComponentInfo.o \
	$(hdr)App.hpp
	g++ -c $< -Iinclude -o $@
	g++ -c -g $< -Iinclude -o debug/$@

obj/ComponentRenderer.o: src/ComponentRenderer.cpp $(hdr)ComponentRenderer.hpp \
	obj/QuadTree.o \
	obj/TextureHolder.o \
	obj/JSONHolder.o \
	$(hdr)App.hpp
	g++ -c $< -Iinclude -o $@
	g++ -c -g $< -Iinclude -o debug/$@

obj/GUIHolder.o: src/GUIHolder.cpp $(hdr)GUIHolder.hpp \
	$(hdr)App.hpp
	g++ -c $< -Iinclude -o $@
	g++ -c -g $< -Iinclude -o debug/$@

obj/Direction.o: src/Direction.cpp $(hdr)Direction.hpp
	g++ -c $< -Iinclude -o $@
	g++ -c -g $< -Iinclude -o debug/$@

obj/History.o: src/History.cpp $(hdr)History.hpp
	g++ -c $< -Iinclude -o $@
	g++ -c -g $< -Iinclude -o debug/$@

obj/Debug.o: src/Debug.cpp $(hdr)Debug.hpp
	g++ -c $< -Iinclude -o $@
	g++ -c -g $< -Iinclude -o debug/$@

clear:
	-rm bin/* obj/* debug/bin/* debug/obj/*

