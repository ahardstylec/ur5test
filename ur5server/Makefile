# Makefile by Andreas Collmann
# Projekt: ur5-tcp-testclient

VERSION = 0.08
CC      = gcc
CFLAGS  = -Wall -g -DVERSION=\"$(VERSION)\"
LIBPATH = -Llibs
LDFLAGS = 

#-Lkinematics -Lconfiguration -Ldev -Lcollision -Lm -Lmath

Targets = ur5-server

all: $(Targets)

ur5server = ur5-server.o tcphelper.o

ur5-server.o: ur5-server.c tcphelper.o
	$(CC) -c ur5-server.c

tcphelper.o: tcphelper.c
	$(CC) -c tcphelper.c

ur5-server: $(ur5server)
	$(CC) $(CFLAGS) -o ur5-server $(ur5server) $(LIBPATH) $(LDFLAGS)

clean: 
	rm *.o $(Targets)