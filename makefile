SOURCES := main.c udp_outbound.c udp_inbound.c msg_input.c msg_print.c list.c

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L

# #compile and link the application
all: build

build:
	gcc $(CFLAGS) $(SOURCES) -lpthread -o talk


#run the application
run: talk
	./talk

#remove built files
clean:
	rm -rf talk main.o udp_inbound.o udp_outbound.o list.o*~