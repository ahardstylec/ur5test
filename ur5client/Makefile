# Makefile by Andreas Collmann
# Projekt: ur5-tcp-testclient

VERSION = 0.08
CC      = gcc
CFLAGS  = -Wall -g -DVERSION=\"$(VERSION)\"
LIBPATH = -Llibs
LDFLAGS = 

#-Lkinematics -Lconfiguration -Ldev -Lcollision -Lm -Lmath

Targets = ur5-client

all: $(Targets)

ur5client = ur5-client.o

ur5-client.o: ur5-client.c
	$(CC) -c ur5-client.c

ur5-client: $(ur5client)
	$(CC) $(CFLAGS) -o ur5-client $(ur5client) $(LIBPATH) $(LDFLAGS)

clean: 
	rm *.o $(Targets)