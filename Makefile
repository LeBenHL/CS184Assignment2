OBJS = as2.o lodepng.o point_light.o directional_light.o three_d_vector.o camera.o pixel.o ray.o scene.o surface.o sampler.o raytracer.o
CC = g++
LDFLAGS = 
	
RM = /bin/rm -f 
all: main 
main: $(OBJS)
	$(CC) $(LDFLAGS) -o as2 $(OBJS) $(LDFLAGS) 
as2.o: as2.cpp lodepng.h camera.h pixel.h
	$(CC) $(LDFLAGS) -c as2.cpp -o as2.o
point_light.o: point_light.h point_light.cpp light.h scene.h
	$(CC) $(LDFLAGS) -c point_light.cpp -o point_light.o
directional_light.o: directional_light.h directional_light.cpp light.h three_d_vector.h
	$(CC) $(LDFLAGS) -c directional_light.cpp -o directional_light.o
three_d_vector.o: three_d_vector.h three_d_vector.cpp
	$(CC) $(LDFLAGS) -c three_d_vector.cpp -o three_d_vector.o
camera.o: camera.h camera.cpp ray.h three_d_vector.h
	$(CC) $(LDFLAGS) -c camera.cpp -o camera.o
pixel.o: pixel.h pixel.cpp
	$(CC) $(LDFLAGS) -c pixel.cpp -o pixel.o
ray.o: ray.h ray.cpp three_d_vector.h
	$(CC) $(LDFLAGS) -c ray.cpp -o ray.o
scene.o: scene.h scene.cpp three_d_vector.h ray.h camera.h surface.h sampler.h raytracer.h
	$(CC) $(LDFLAGS) -c scene.cpp -o scene.o
surface.o: surface.h surface.cpp ray.h
	$(CC) $(LDFLAGS) -c surface.cpp -o surface.o	
sampler.o: sampler.h sampler.cpp three_d_vector.h
	$(CC) $(LDFLAGS) -c sampler.cpp -o sampler.o	
raytracer.o: raytracer.h raytracer.cpp ray.h three_d_vector.h surface.h
	$(CC) $(LDFLAGS) -c raytracer.cpp -o raytracer.o

clean: 
	$(RM) *.o as2
 


