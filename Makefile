
EXE = fantasy-world
FLAGS = -ITGUI/include -Wall -Os -ffast-math -s
LIBS = -L. -ltgui -lsfml-graphics -lsfml-window -lsfml-system -lPocoNetSSL -lPocoNet -lPocoFoundation


SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)


$(EXE) : $(OBJECTS)
	$(CXX) $(FLAGS) -o $@ $^ $(LIBS)

%.o : %.cpp
	$(CXX) $(FLAGS) -o $@ -c $<

clean :
	$(RM) $(OBJECTS) $(EXE)

.PHONY : clean
