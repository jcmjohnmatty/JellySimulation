CC       = gcc
CCFLAGS  = -g -O0 -Wall
INC      = -I /usr/include -I/usr/local/include -I/usr/local/lib/include \
		   -I. -I..
LIBS     = -L/usr/local/lib -lm
GLLIBS   = -lGL -lGLU -lglut
OBJS     = psys.o

.PHONY:
all: $(OBJS) test

test:
	$(MAKE) -C test

psys.o: psys.c psys.c
	$(CC) $(CCFLAGS) $(INC) -c psys.c -o psys.o $(LIBS) $(GLLIBS)

.PHONY:
clean:
	/bin/rm -f $(OBJS)
	/bin/rm -f $(PROGRAM)
	/bin/rm -f core
	$(MAKE) -C test clean
