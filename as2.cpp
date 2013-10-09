#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "lodepng.h"
#include "camera.h"
#include "scene.h"
#include "sphere.h"
#include "directional_light.h"
#include "point_light.h"

using namespace std;

ThreeDVector* global_ambient = new ThreeDVector(0, 0, 0);
float PI = 3.14159265359;

void createPng(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
{
  //Encode the image
  unsigned error = lodepng::encode(filename, image, width, height);

  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

void loadScene(std::string file) {

  //store variables and set stuff at the end
  int width = 1000, height = 1000;
  std::string fname = "output.png";

  //Camera
  Camera* camera = new Camera(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  //Surfaces
  vector<Surface*> surfaces;
  //LIGHTS CAMERA AND ACTION BUT NOT CAMERA (see above)
  vector<Light*> lights;
  //MAX DEPTH
  int depth = 1;

  //THE CURRENT SURFACE
  Surface* current_surface;

  std::ifstream inpfile(file.c_str());
  if(!inpfile.is_open()) {
    std::cout << "Unable to open file" << std::endl;
  } else {
    std::string line;
    //MatrixStack mst;

    
    while(inpfile.good()) {
      std::vector<std::string> splitline;
      std::string buf;

      std::getline(inpfile,line);
      std::stringstream ss(line);

      while (ss >> buf) {
        splitline.push_back(buf);
      }
      //Ignore blank lines
      if(splitline.size() == 0) {
        continue;
      }

      //Ignore comments
      if(splitline[0][0] == '#') {
        continue;
      }

      //Valid commands:
      //size width height
      //  must be first command of file, controls image size
      else if(!splitline[0].compare("size")) {
        width = atoi(splitline[1].c_str());
        height = atoi(splitline[2].c_str());
      }
      //maxdepth depth
      //  max # of bounces for ray (default 5)
      else if(!splitline[0].compare("maxdepth")) {
        depth = atoi(splitline[1].c_str());
      }
      //output filename
      //  output file to write image to 
      else if(!splitline[0].compare("output")) {
        fname = splitline[1];
      }

      //camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov
      //  speciﬁes the camera in the standard way, as in homework 2.
      else if(!splitline[0].compare("camera")) {
        float from_x = atof(splitline[1].c_str());
        float from_y = atof(splitline[2].c_str());
        float from_z = atof(splitline[3].c_str());
        float lookat_x = atof(splitline[4].c_str());
        float lookat_y = atof(splitline[5].c_str());
        float lookat_z = atof(splitline[6].c_str());
        float up_x = atof(splitline[7].c_str());
        float up_y = atof(splitline[8].c_str());
        float up_z = atof(splitline[9].c_str());
        float fov = atof(splitline[10].c_str());

        delete camera;
        camera = new Camera(from_x, from_y, from_z, lookat_x, lookat_y, lookat_z, up_x, up_y, up_z, fov);
      }

      //sphere x y z radius
      //  Deﬁnes a sphere with a given position and radius.
      else if(!splitline[0].compare("sphere")) {
        // x: atof(splitline[1].c_str())
        // y: atof(splitline[2].c_str())
        // z: atof(splitline[3].c_str())
        // r: atof(splitline[4].c_str())
        // Create new sphere:
        //   Store 4 numbers
        //   Store current property values
        //   Store current top of matrix stack
        float x = atof(splitline[1].c_str());
        float y = atof(splitline[2].c_str());
        float z = atof(splitline[3].c_str());
        float radius = atof(splitline[4].c_str());
        ThreeDVector* center = new ThreeDVector(x, y, z);
        Sphere* sphere = new Sphere(center, radius);
        current_surface = sphere;
        surfaces.push_back(sphere);
      }
      //maxverts number
      //  Deﬁnes a maximum number of vertices for later triangle speciﬁcations. 
      //  It must be set before vertices are deﬁned.
      else if(!splitline[0].compare("maxverts")) {
        // Care if you want
        // Here, either declare array size
        // Or you can just use a STL vector, in which case you can ignore this
      }
      //maxvertnorms number
      //  Deﬁnes a maximum number of vertices with normals for later speciﬁcations.
      //  It must be set before vertices with normals are deﬁned.
      else if(!splitline[0].compare("maxvertnorms")) {
        // Care if you want
      }
      //vertex x y z
      //  Deﬁnes a vertex at the given location.
      //  The vertex is put into a pile, starting to be numbered at 0.
      else if(!splitline[0].compare("vertex")) {
        // x: atof(splitline[1].c_str()),
        // y: atof(splitline[2].c_str()),
        // z: atof(splitline[3].c_str()));
        // Create a new vertex with these 3 values, store in some array
      }
      //vertexnormal x y z nx ny nz
      //  Similar to the above, but deﬁne a surface normal with each vertex.
      //  The vertex and vertexnormal set of vertices are completely independent
      //  (as are maxverts and maxvertnorms).
      else if(!splitline[0].compare("vertexnormal")) {
        // x: atof(splitline[1].c_str()),
        // y: atof(splitline[2].c_str()),
        // z: atof(splitline[3].c_str()));
        // nx: atof(splitline[4].c_str()),
        // ny: atof(splitline[5].c_str()),
        // nz: atof(splitline[6].c_str()));
        // Create a new vertex+normal with these 6 values, store in some array
      }
      //tri v1 v2 v3
      //  Create a triangle out of the vertices involved (which have previously been speciﬁed with
      //  the vertex command). The vertices are assumed to be speciﬁed in counter-clockwise order. Your code
      //  should internally compute a face normal for this triangle.
      else if(!splitline[0].compare("tri")) {
        // v1: atof(splitline[1].c_str())
        // v2: atof(splitline[2].c_str())
        // v3: atof(splitline[3].c_str())
        // Create new triangle:
        //   Store pointer to array of vertices
        //   Store 3 integers to index into array
        //   Store current property values
        //   Store current top of matrix stack
      }
      //trinormal v1 v2 v3
      //  Same as above but for vertices speciﬁed with normals.
      //  In this case, each vertex has an associated normal, 
      //  and when doing shading, you should interpolate the normals 
      //  for intermediate points on the triangle.
      else if(!splitline[0].compare("trinormal")) {
        // v1: atof(splitline[1].c_str())
        // v2: atof(splitline[2].c_str())
        // v3: atof(splitline[3].c_str())
        // Create new triangle:
        //   Store pointer to array of vertices (Different array than above)
        //   Store 3 integers to index into array
        //   Store current property values
        //   Store current top of matrix stack
      }

      //translate x y z
      //  A translation 3-vector
      else if(!splitline[0].compare("translate")) {
        // x: atof(splitline[1].c_str())
        // y: atof(splitline[2].c_str())
        // z: atof(splitline[3].c_str())
        // Update top of matrix stack
      }
      //rotate x y z angle
      //  Rotate by angle (in degrees) about the given axis as in OpenGL.
      else if(!splitline[0].compare("rotate")) {
        // x: atof(splitline[1].c_str())
        // y: atof(splitline[2].c_str())
        // z: atof(splitline[3].c_str())
        // angle: atof(splitline[4].c_str())
        // Update top of matrix stack
      }
      //scale x y z
      //  Scale by the corresponding amount in each axis (a non-uniform scaling).
      else if(!splitline[0].compare("scale")) {
        // x: atof(splitline[1].c_str())
        // y: atof(splitline[2].c_str())
        // z: atof(splitline[3].c_str())
        // Update top of matrix stack
      }
      //pushTransform
      //  Push the current modeling transform on the stack as in OpenGL. 
      //  You might want to do pushTransform immediately after setting 
      //   the camera to preserve the “identity” transformation.
      else if(!splitline[0].compare("pushTransform")) {
        //mst.push();
      }
      //popTransform
      //  Pop the current transform from the stack as in OpenGL. 
      //  The sequence of popTransform and pushTransform can be used if 
      //  desired before every primitive to reset the transformation 
      //  (assuming the initial camera transformation is on the stack as 
      //  discussed above).
      else if(!splitline[0].compare("popTransform")) {
        //mst.pop();
      }

      //directional x y z r g b
      //  The direction to the light source, and the color, as in OpenGL.
      else if(!splitline[0].compare("directional")) {
        float x = atof(splitline[1].c_str());
        float y = atof(splitline[2].c_str());
        float z = atof(splitline[3].c_str());
        float r = atof(splitline[4].c_str());
        float g = atof(splitline[5].c_str());
        float b = atof(splitline[6].c_str());
        // add light to scene...
        DirectionalLight* light = new DirectionalLight(x, y, z, r, g, b);
        lights.push_back(light);
      }
      //point x y z r g b
      //  The location of a point source and the color, as in OpenGL.
      else if(!splitline[0].compare("point")) {
        float x = atof(splitline[1].c_str());
        float y = atof(splitline[2].c_str());
        float z = atof(splitline[3].c_str());
        float r = atof(splitline[4].c_str());
        float g = atof(splitline[5].c_str());
        float b = atof(splitline[6].c_str());
        // add light to scene...
        PointLight* light = new PointLight(x, y, z, r, g, b);
        lights.push_back(light);
      }
      //attenuation const linear quadratic
      //  Sets the constant, linear and quadratic attenuations 
      //  (default 1,0,0) as in OpenGL.
      else if(!splitline[0].compare("attenuation")) {
        // const: atof(splitline[1].c_str())
        // linear: atof(splitline[2].c_str())
        // quadratic: atof(splitline[3].c_str())
      }
      //ambient r g b
      //  The global ambient color to be added for each object 
      //  (default is .2,.2,.2)
      else if(!splitline[0].compare("ambient")) {
        float r = atof(splitline[1].c_str());
        float g = atof(splitline[2].c_str());
        float b = atof(splitline[3].c_str());
        global_ambient = new ThreeDVector(r, g, b);
      }

      //diﬀuse r g b
      //  speciﬁes the diﬀuse color of the surface.
      else if(!splitline[0].compare("diffuse")) {
        float r = atof(splitline[1].c_str());
        float g = atof(splitline[2].c_str());
        float b = atof(splitline[3].c_str());
        // Update current properties
        current_surface->diffuse = new ThreeDVector(r, g, b);
      }
      //specular r g b 
      //  speciﬁes the specular color of the surface.
      else if(!splitline[0].compare("specular")) {
        float r = atof(splitline[1].c_str());
        float g = atof(splitline[2].c_str());
        float b = atof(splitline[3].c_str());
        // Update current properties
        current_surface->specular = new ThreeDVector(r, g, b);
      }
      //shininess s
      //  speciﬁes the shininess of the surface.
      else if(!splitline[0].compare("shininess")) {
        // shininess: atof(splitline[1].c_str())
        // Update current properties
        current_surface->power_coefficient = atof(splitline[1].c_str());
      }
      //emission r g b
      //  gives the emissive color of the surface.
      else if(!splitline[0].compare("emission")) {
        // r: atof(splitline[1].c_str())
        // g: atof(splitline[2].c_str())
        // b: atof(splitline[3].c_str())
        // Update current properties
      } else {
        std::cerr << "Unknown command: " << splitline[0] << std::endl;
      }
    }
  

    Scene* scene = new Scene(camera, surfaces, lights, width, height, depth);

    //NOTE: this sample will overwrite the file or test.png without warning!
    const char* filename = fname.c_str();

    //generate some image
    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    int new_y;
    ThreeDVector* color;
    float r, g, b;
    for (int y = 0; y < height; y++)
      for (int x = 0; x < width; x++) {
        new_y = height - 1 - y;
        color = scene->get_color(x, y);
        r = min(color->x * 255, float(255));
        g = min(color->y * 255, float(255.0));
        b = min(color->z * 255, float(255.0));
        image[4 * width * new_y + 4 * x + 0] = r;
        image[4 * width * new_y + 4 * x + 1] = g;
        image[4 * width * new_y + 4 * x + 2] = b;
        image[4 * width * new_y + 4 * x + 3] = 255;
        delete color;
      }

    createPng(filename, image, width, height);


    inpfile.close();
  }

}

int main(int argc, char *argv[]) {

  if (argc > 1) {
    loadScene(argv[1]);
  }

  return 0;
}
