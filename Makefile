OBJS = as2.o lodepng.o
CC = g++
LDFLAGS = 
	
RM = /bin/rm -f 
all: main 
main: $(OBJS)
	$(CC) $(LDFLAGS) -o as2 $(OBJS) $(LDFLAGS) 
as2.o: as2.cpp lodepng.h
	$(CC) $(LDFLAGS) -c as2.cpp -o as2.o

clean: 
	$(RM) *.o as2
 


