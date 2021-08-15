CC=gcc
BLDDIR = .
INCDIR = $(BLDDIR)/includes
CFLAGS= -g -I$(INCDIR)
SERVERDIR=$(BLDDIR)/server
CLIENTDIR=$(BLDDIR)/client
SERVER= $(wildcard $(SERVERDIR)/*.c)
CLIENT=$(wildcard $(CLIENTDIR)/*.c)

serve:
	$(CC) $(SERVER) $(CFLAGS) $< -o $(BLDDIR)/$@

cli: $(OBJ2)
	$(CC) $(CLIENT) $(CFLAGS) $< -o $(BLDDIR)/$@

runserver:
	./serve

runclient:
	./cli