# Makefile by Andreas Collmann
# Projekt: ur5-tcp-testclient

VERSION = 0.08
CC      = g++
CFLAGS  = -Wall -g -DVERSION=\"$(VERSION)\"

LIBPATH = -L../libs/
LDFLAGS = -lur5
HFILES = -I../libs

#-Lkinematics -Lconfiguration -Ldev -Lcollision -Lm -Lmath

Targets = ur5-server

all: $(Targets)

ur5server = ur5-server.o tcphelper.o

ur5-server.o: ur5-server.c
	$(CC) $(HFILES) -c ur5-server.c

tcphelper.o: tcphelper.c
	$(CC) $(HFILES) -c tcphelper.c

ur5-server: $(ur5server)
	$(CC) $(CFLAGS) $(LIBPATH) $(HFILES) -o ur5-server $(ur5server) $(LDFLAGS)

clean: 
	rm *.o $(Targets)