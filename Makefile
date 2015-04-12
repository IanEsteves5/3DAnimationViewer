EXE = trabalhoCG.exe
OBJ = main.o myUti.o objModel.o animAnimation.o

all: $(EXE)

trabalhoCG.exe: $(OBJ)
	g++ -o $@ $^ -lfreeglut -lglu32 -lopengl32
   
trabalhoCG: $(OBJ)
	g++ -o $@ $^ -framework OpenGL -framework GLUT

main.o: myUti.h objModel.h
myUti.o: myUti.h
objModel.o: myUti.h objModel.h
animAnimation.o: myUti.h animAnimation.h

%.o: %.cpp
	g++ -c $<

clean:
	rm $(EXE) $(OBJ)