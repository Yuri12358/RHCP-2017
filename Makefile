bin=bin/app
bin_debug=debug/bin/app
objs=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/*.cpp))
objs_debug=$(patsubst src/%.cpp,debug/obj/%.o,$(wildcard src/*.cpp))
libs=-ltgui -lsfml-graphics -lsfml-window -lsfml-system
hdr=include/Prjoct2/
precomp=$(hdr)Precomp.h.gch
precomp_flag=-include $(hdr)Precomp.h

include_paths=-Iinclude 
library_paths=

all: bin obj saves debug debug/bin debug/obj $(precomp) $(bin) $(bin_debug)

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

$(precomp):
	g++ $(hdr)Precomp.h -o $@

$(bin): $(objs)
	g++ $(objs) $(libs) $(library_paths) -o $(bin)

$(bin_debug): $(objs_debug)
	g++ $(objs_debug) $(libs) $(library_paths) -o $(bin_debug)

obj/Main.o: src/Main.cpp \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/App.o: src/App.cpp $(hdr)App.hpp \
	$(hdr)MainMenuState.hpp \
	$(hdr)EditorState.hpp \
	$(hdr)JSONHolder.hpp \
	$(hdr)GUIHolder.hpp \
	$(hdr)State.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/TextureHolder.o: src/TextureHolder.cpp $(hdr)TextureHolder.hpp \
	$(hdr)JSONHolder.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/JSONHolder.o: src/JSONHolder.cpp $(hdr)JSONHolder.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/ComponentInfo.o: src/ComponentInfo.cpp $(hdr)ComponentInfo.hpp \
	$(hdr)JSONHolder.hpp \
	$(hdr)Direction.hpp 
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/QuadTree.o: src/QuadTree.cpp $(hdr)QuadTree.hpp \
	$(hdr)ComponentInfo.hpp \
	$(hdr)JSONHolder.hpp \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/ComponentRenderer.o: src/ComponentRenderer.cpp $(hdr)ComponentRenderer.hpp \
	$(hdr)TextureHolder.hpp \
	$(hdr)ComponentInfo.hpp \
	$(hdr)JSONHolder.hpp \
	$(hdr)Direction.hpp \
	$(hdr)QuadTree.hpp \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/GUIHolder.o: src/GUIHolder.cpp $(hdr)GUIHolder.hpp \
	$(hdr)TextureHolder.hpp \
	$(hdr)JSONHolder.hpp \
	$(hdr)Utility.hpp \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/Direction.o: src/Direction.cpp $(hdr)Direction.hpp
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/History.o: src/History.cpp $(hdr)History.hpp \
	$(hdr)JSONHolder.hpp \
	$(hdr)QuadTree.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/Debug.o: src/Debug.cpp $(hdr)Debug.hpp \
	$(hdr)JSONHolder.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/Utility.o: src/Utility.cpp $(hdr)Utility.hpp $(hdr)Utility.inl
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/EditorState.o: src/EditorState.cpp $(hdr)EditorState.hpp \
	$(hdr)ComponentRenderer.hpp \
	$(hdr)ComponentInfo.hpp \
	$(hdr)TextureHolder.hpp \
	$(hdr)JSONHolder.hpp \
	$(hdr)GUIHolder.hpp \
	$(hdr)QuadTree.hpp \
	$(hdr)History.hpp \
	$(hdr)State.hpp \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/MenuState.o: src/MenuState.cpp $(hdr)MenuState.hpp \
	$(hdr)TextureHolder.hpp \
	$(hdr)Utility.hpp \
	$(hdr)State.hpp \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/MainMenuState.o: src/MainMenuState.cpp $(hdr)MainMenuState.hpp \
	$(hdr)CorridorMenuState.hpp \
	$(hdr)TextureHolder.hpp \
	$(hdr)MenuState.hpp \
	$(hdr)Utility.hpp \
	$(hdr)State.hpp \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) -o $@
	g++ -c -g $< $(include_paths) -o debug/$@

obj/CorridorMenuState.o: src/CorridorMenuState.cpp $(hdr)CorridorMenuState.hpp \
	$(hdr)Form5MenuState.hpp \
	$(hdr)Form9MenuState.hpp \
	$(hdr)Form11MenuState.hpp \
	$(hdr)TextureHolder.hpp \
	$(hdr)MenuState.hpp \
	$(hdr)Utility.hpp \
	$(hdr)State.hpp \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/Form5MenuState.o: src/Form5MenuState.cpp $(hdr)Form5MenuState.hpp \
	$(hdr)TextureHolder.hpp \
	$(hdr)EditorState.hpp \
	$(hdr)MenuState.hpp \
	$(hdr)State.hpp \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/Form9MenuState.o: src/Form9MenuState.cpp $(hdr)Form9MenuState.hpp \
	$(hdr)TextureHolder.hpp \
	$(hdr)EditorState.hpp \
	$(hdr)MenuState.hpp \
	$(hdr)State.hpp \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

obj/Form11MenuState.o: src/Form11MenuState.cpp $(hdr)Form11MenuState.hpp \
	$(hdr)TextureHolder.hpp \
	$(hdr)EditorState.hpp \
	$(hdr)MenuState.hpp \
	$(hdr)State.hpp \
	$(hdr)App.hpp
	g++ -c $< $(include_paths) $(precomp_flag) -o $@
	g++ -c -g $< $(include_paths) $(precomp_flag) -o debug/$@

clear:
	-rm bin/* obj/* debug/bin/* debug/obj/* $(precomp)

