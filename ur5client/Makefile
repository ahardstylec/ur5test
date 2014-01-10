# Makefile by Andreas Collmann
# Projekt: ur5-tcp-testclient

VERSION = 0.08
CC      = g++
CFLAGS  = -Wall -g -DVERSION=\"$(VERSION)\"
LIBPATH = -L../libs
HFLAGS 	= -I../libs
LDFLAGS = -lur5

#-Lkinematics -Lconfiguration -Ldev -Lcollision -Lm -Lmath

Targets = ur5-client

all: $(Targets)

ur5client = ur5-client.o

ur5-client.o: ur5-client.c
	$(CC) $(HFLAGS) -c ur5-client.c

ur5-client: $(ur5client)
	$(CC) $(CFLAGS) $(HFLAGS) -o ur5-client $(ur5client) $(LIBPATH) $(LDFLAGS)

clean: 
	rm *.o $(Targets)