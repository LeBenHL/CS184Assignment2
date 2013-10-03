OBJS = as2.o
CC = g++
LDFLAGS = -L./MagickLib -lMagick++-6.Q16 -lMagickWand-6.Q16 -lMagickCore-6.Q16 -fopenmp -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16 -fopenmp -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16 -fopenmp -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16 -I./MagickHeader
	
RM = /bin/rm -f 
all: main 
main: $(OBJS)
	$(CC) $(LDFLAGS) -o as2 $(OBJS) $(LDFLAGS) 
as2.o: as2.cpp
	$(CC) $(LDFLAGS) -c as2.cpp -o as2.o

clean: 
	$(RM) *.o as2
 


