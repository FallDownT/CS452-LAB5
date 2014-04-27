run: lab5.cpp
	g++ lab5.cpp InitShader.cpp -lGL -lGLEW -lSDL2

clean:
	rm -f *.out *~
