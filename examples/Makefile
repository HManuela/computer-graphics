HOMEWORKNAME=template
TEAMNAME=ioana birsan B5
PmsREPEND=[GPCL]
CC=g++
CFLAGS=-Wall
LDFLAGS=-lm -lGL -lGLU -lglut
CPPSOURCES= $(wildcard ./*.cpp)
CPPOBJECTS= $(CPPSOURCES:.cpp=.o)
CSOURCES= $(wildcard ./src/*.c)
SOURCES= $(CSOURCES) $(CPPSOURCES)
OBJECTS= $(COBJECTS) $(CPPOBJECTS)
CEXES= $(CSOURCES:.c=)
CPPEXES= $(CPPSOURCES:.cpp=)
EXECUTABLES= $(CEXES) $(CPPEXES)
ZIPNAME="[GPCL] $(TEAMNAME) $(HOMEWORKNAME).zip"

all: clean $(SOURCES) $(EXECUTABLES)

debug:
	make CFLAGS="-Wall -ggdb -emacs -O0"

release:
	make CFLAGS="-Wall -DNDEBUG -O3"


$(EXECUTABLES): $(SOURCES)
	$(CC) $< -o $@ $(GENERALFLAGS) $(CFLAGS) $(LDFLAGS) 
	./$@
	
clean: 
	rm -Rf $(EXECUTABLES) $(ZIPNAME) || echo ""

zip: clean
	zip -r $(ZIPNAME) .
	
