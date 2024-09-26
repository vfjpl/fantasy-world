
LIB = libtgui.so
FLAGS = -ITGUI/include -Os -ffast-math -s -fpic -shared
LIBS = -lsfml-graphics -lsfml-window -lsfml-system


SOURCES = $(wildcard $(patsubst %,%/*.cpp,TGUI/src/TGUI TGUI/src/TGUI/Loading TGUI/src/TGUI/Renderers TGUI/src/TGUI/Widgets))
OBJECTS = $(SOURCES:.cpp=.o)


$(LIB) : $(OBJECTS)
	$(CXX) $(FLAGS) -o $@ $^ $(LIBS)

%.o : %.cpp
	$(CXX) $(FLAGS) -o $@ -c $<

clean :
	$(RM) $(OBJECTS)

.PHONY : clean
