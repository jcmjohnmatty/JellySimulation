CC       = gcc
CCFLAGS  = -g -pg -O0 -Wall -Wno-deprecated-declarations -Wno-trigraphs
INC      = -I /usr/include -I/usr/local/include -I/usr/local/lib/include \
		   -I. -I..

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
		GLLIBS   = -lGL -lGLU -lglut
		LIBS     = -L/usr/local/lib -lm
endif
ifeq ($(UNAME_S),Darwin)
		GLLIBS      = -framework GLUT -framework OpenGL -framework Cocoa 
endif

PROGRAM  = jellyfish
OBJS     = glmain.o jelly/indexutil.o jelly/jelly_animate.o jelly/jelly_body.o \
		   jelly/jelly_tentacle.o jelly/vertex.o util/linalg.o \
		   util/transform.o util/uba.o

$(PROGRAM): $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o $(PROGRAM) $(LIBS) $(GLLIBS)

glmain.o: glmain.c glmain.h jelly/jelly.h jelly/jelly_animate.h \
		  jelly/jelly_body.h textures/blooper_head.h  textures/sewer_bg.h \
		  textures/sewer_floor.h util/linalg.h util/transform.h
	$(CC) $(CCFLAGS) $(INC) -c glmain.c -o glmain.o

jelly/indexutil.o: jelly/indexutil.c jelly/indexutil.h
	$(CC) $(CCFLAGS) $(INC) -c jelly/indexutil.c -o jelly/indexutil.o 

jelly/jelly_animate.o: jelly/jelly_animate.c util/linalg.h jelly/indexutil.h \
					   jelly/jelly_animate.h
	$(CC) $(CCFLAGS) $(INC) -c jelly/jelly_animate.c -o jelly/jelly_animate.o 

jelly/jelly_body.o: jelly/jelly_body.c jelly/indexutil.h jelly/jelly_body.h \
					jelly/vertex.h util/linalg.h util/uba.h
	$(CC) $(CCFLAGS) $(INC) -c jelly/jelly_body.c -o jelly/jelly_body.o 

jelly/jelly_tentacle.o: jelly/jelly_tentacle.c jelly/indexutil.h \
						jelly/jelly_tentacle.h jelly/vertex.h util/linalg.h \
						util/uba.h
	$(CC) $(CCFLAGS) $(INC) -c jelly/jelly_tentacle.c -o jelly/jelly_tentacle.o 

jelly/vertex.o: jelly/vertex.c jelly/vertex.h
	$(CC) $(CCFLAGS) $(INC) -c jelly/vertex.c -o jelly/vertex.o 

util/linalg.o: util/linalg.c util/linalg.h
	$(CC) $(CCFLAGS) $(INC) -c util/linalg.c -o util/linalg.o 

util/transform.o: util/transform.c util/transform.h
	$(CC) $(CCFLAGS) $(INC) -c util/transform.c -o util/transform.o 

util/uba.o: util/uba.c util/uba.h
	$(CC) $(CCFLAGS) $(INC) -c util/uba.c -o util/uba.o 

.PHONY:
clean:
	/bin/rm -f $(OBJS)
	/bin/rm -f $(PROGRAM)
	/bin/rm -f gmon.out
	/bin/rm -f core
