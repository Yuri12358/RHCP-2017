bin=bin/app
bin_debug=debug/bin/app
objs=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/*.cpp))
objs_debug=$(patsubst src/%.cpp,debug/obj/%.o,$(wildcard src/*.cpp))
libs=-ltgui -lsfml-graphics -lsfml-window -lsfml-system
hdr=include/Prjoct2/

include_paths=-Iinclude 
library_paths=

all: bin obj saves debug debug/bin debug/obj $(bin) $(bin_debug)

bin:
	mkdir bin

obj:
	mkdir obj

saves:
	mkdir saves

debug:
	mkdir debug

debug/bin:
	mkdir debug/bin

debug/obj:
	mkdir debug/obj

$(bin): $(objs)
	g++ $(objs) $(libs) $(library_paths) -o $(bin)

$(bin_debug): $(objs_debug)
	g++ $(objs_debug) $(libs) $(library_paths) -o $(bin_debug)

obj/Main.o: src/Main.cpp \
	obj/App.o
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/App.o: src/App.cpp $(hdr)App.hpp \
	obj/ComponentRenderer.o \
	obj/TextureHolder.o \
	obj/ComponentInfo.o \
	obj/JSONHolder.o \
	obj/GUIHolder.o \
	obj/QuadTree.o \
	obj/Utility.o \
	obj/History.o \
	obj/Debug.o
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/TextureHolder.o: src/TextureHolder.cpp $(hdr)TextureHolder.hpp \
	obj/JSONHolder.o
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/JSONHolder.o: src/JSONHolder.cpp $(hdr)JSONHolder.hpp
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/ComponentInfo.o: src/ComponentInfo.cpp $(hdr)ComponentInfo.hpp \
	obj/JSONHolder.o \
	obj/Direction.o 
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/QuadTree.o: src/QuadTree.cpp $(hdr)QuadTree.hpp \
	obj/ComponentInfo.o \
	obj/JSONHolder.o \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/ComponentRenderer.o: src/ComponentRenderer.cpp $(hdr)ComponentRenderer.hpp \
	obj/TextureHolder.o \
	obj/ComponentInfo.o \
	obj/JSONHolder.o \
	obj/Direction.o \
	obj/QuadTree.o \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/GUIHolder.o: src/GUIHolder.cpp $(hdr)GUIHolder.hpp \
	obj/TextureHolder.o \
	obj/JSONHolder.o \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/Direction.o: src/Direction.cpp $(hdr)Direction.hpp
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/History.o: src/History.cpp $(hdr)History.hpp \
	obj/JSONHolder.o \
	obj/QuadTree.o
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/Debug.o: src/Debug.cpp $(hdr)Debug.hpp \
	obj/JSONHolder.o
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/Utility.o: src/Utility.cpp $(hdr)Utility.hpp $(hdr)Utility.inl
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

clear:
	-rm bin/* obj/* debug/bin/* debug/obj/*

