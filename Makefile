OBJS = as2.o lodepng.o point_light.o directional_light.o three_d_vector.o camera.o pixel.o ray.o scene.o surface.o sampler.o raytracer.o sphere.o triangle.o Bounds.o AABBNode.o
CC = g++
LDFLAGS = 
INCLUDE = -I ./
	
RM = /bin/rm -f 
all: main 
main: $(OBJS)
	$(CC) $(LDFLAGS) $(INCLUDE) -o as2 $(OBJS) $(LDFLAGS) 
as2.o: as2.cpp lodepng.h camera.h scene.h sphere.h triangle.h directional_light.h point_light.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c as2.cpp -o as2.o
point_light.o: point_light.h point_light.cpp light.h scene.h three_d_vector.h ray.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c point_light.cpp -o point_light.o
directional_light.o: directional_light.h directional_light.cpp light.h three_d_vector.h ray.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c directional_light.cpp -o directional_light.o
three_d_vector.o: three_d_vector.h three_d_vector.cpp
	$(CC) $(LDFLAGS) $(INCLUDE) -c three_d_vector.cpp -o three_d_vector.o
camera.o: camera.h camera.cpp ray.h three_d_vector.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c camera.cpp -o camera.o
pixel.o: pixel.h pixel.cpp
	$(CC) $(LDFLAGS) $(INCLUDE) -c pixel.cpp -o pixel.o
ray.o: ray.h ray.cpp three_d_vector.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c ray.cpp -o ray.o
scene.o: scene.h scene.cpp three_d_vector.h ray.h camera.h surface.h sampler.h raytracer.h light.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c scene.cpp -o scene.o
surface.o: surface.h surface.cpp ray.h three_d_vector.h Bounds.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c surface.cpp -o surface.o	
sampler.o: sampler.h sampler.cpp three_d_vector.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c sampler.cpp -o sampler.o	
raytracer.o: raytracer.h raytracer.cpp ray.h three_d_vector.h surface.h light.h AABBNode.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c raytracer.cpp -o raytracer.o
sphere.o: sphere.h sphere.cpp surface.h three_d_vector.h ray.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c sphere.cpp -o sphere.o
triangle.o: triangle.h triangle.cpp surface.h three_d_vector.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c triangle.cpp -o triangle.o
Bounds.o: Bounds.h Bounds.cpp
	$(CC) $(LDFLAGS) $(INCLUDE) -c Bounds.cpp -o Bounds.o
AABBNode.o: AABBNode.h AABBNode.cpp Bounds.h surface.h three_d_vector.h
	$(CC) $(LDFLAGS) $(INCLUDE) -c AABBNode.cpp -o AABBNode.o


clean: 
	$(RM) *.o as2
 


