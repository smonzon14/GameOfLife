# Linux (default)
EXE = cgol
LDFLAGS = -lGL -lGLU -lglut

# Windows (cygwin)
ifeq ($(OS), "Windows_NT")
	EXE = hello.exe
    LDFLAGS = -lopengl32 -lglu32 -lglut32
endif

# OS X
ifeq ($(OSTYPE), "darwin9.0")
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
endif

$(EXE) : main.cpp
		gcc -o $@ $< $(CFLAGS) $(LDFLAGS)