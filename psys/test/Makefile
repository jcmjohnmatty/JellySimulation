CC       = gcc
CCFLAGS  = -g -O0 -Wall
INC      = -I /usr/include -I/usr/local/include -I/usr/local/lib/include \
		   -I. -I..
LIBS     = -L/usr/local/lib -lm
GLLIBS   = -lGL -lGLU -lglut
PROGRAM  = psystest
OBJS     = glmain.o ../psys.o

$(PROGRAM): $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o $(PROGRAM) $(LIBS) $(GLLIBS)

glmain.o: glmain.c glmain.h ../psys.h
	$(CC) $(CCFLAGS) $(INC) -c glmain.c -o glmain.o $(LIBS) $(GLLIBS)

.PHONY:
clean:
	/bin/rm -f $(OBJS)
	/bin/rm -f $(PROGRAM)
	/bin/rm -f core