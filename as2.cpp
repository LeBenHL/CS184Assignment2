#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <iostream>
#include <Eigen/Dense>
#include "lodepng.h"
#include "camera.h"
#include "scene.h"
#include "sphere.h"
#include "triangle.h"
#include "directional_light.h"
#include "point_light.h"

using namespace Eigen;
using namespace std;

ThreeDVector* global_ambient = new ThreeDVector(0, 0, 0);
float PI = 3.14159265359;

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
int depth = 5;
//DIFFUSE
ThreeDVector* diffuse = new ThreeDVector(0, 0, 0);
//SPECULAR
ThreeDVector* specular = new ThreeDVector(0, 0, 0);
//SHININESS
float shininess = 1;
//EMISSION
ThreeDVector* emission = new ThreeDVector(0, 0, 0);
//VERTICES
vector<ThreeDVector*> vertices;
//Normal Vertices
vector<ThreeDVector*> normal_vertices;
//GRID SIZE (FOR Anti Aliasing)
int grid_size = 1;
//Matrix Stack
stack<Matrix4f> matrix_stack;
Matrix4f current_matrix = Matrix4f::Identity();

void createPng(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
{
  //Encode the image
  unsigned error = lodepng::encode(filename, image, width, height);

  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

void parseObj(const char* filename) {
  vector<ThreeDVector*> vertices;

  std::ifstream inpfile(filename);
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
      //Valid commands:
      //v x y z [w]
      else if(!splitline[0].compare("v")) {
        float x = atof(splitline[1].c_str());
        float y = atof(splitline[2].c_str());
        float z = atof(splitline[3].c_str());
        ThreeDVector* vertex = new ThreeDVector(x, y, z);
        vertices.push_back(vertex);
      }
      //f v1 v2 v3 v4 ....
      //We assume they are all triangles defined with 3 vertices
      //Most files seem to be like this
      //1 indexed vertices so we need to add 1
      else if(!splitline[0].compare("f")) {
        const char* v1 = splitline[1].c_str();
        const char* v2 = splitline[2].c_str();
        const char* v3 = splitline[3].c_str();
        char v1_str[500];
        char v2_str[500];
        char v3_str[500];
        strncpy(v1_str, v1, sizeof(v1_str));
        strncpy(v2_str, v2, sizeof(v2_str));
        strncpy(v3_str, v3, sizeof(v3_str));
        int v1_index = atoi(strtok(v1_str, "\\")) - 1;
        int v2_index = atoi(strtok(v2_str, "\\")) - 1;
        int v3_index = atoi(strtok(v3_str, "\\")) - 1;
        int max_v1_v2 = max(v1_index, v2_index);
        int max_v1_v2_v3 = max(max_v1_v2, v3_index);
        if (vertices.size() < max_v1_v2_v3 + 1) {
          cerr << "Tried to access vertex that was not defined yet" << endl;
          exit(1);
        }
        ThreeDVector* v1_vector = vertices[v1_index];
        ThreeDVector* v2_vector = vertices[v2_index];
        ThreeDVector* v3_vector = vertices[v3_index];
        Triangle* triangle = new Triangle(v1_vector->clone(), v2_vector->clone(), v3_vector->clone());
        triangle->diffuse = diffuse->clone();
        triangle->specular = specular->clone();
        triangle->power_coefficient = shininess;
        triangle->emission = emission->clone();
        triangle->transformation = Matrix4f(current_matrix);
        triangle->inverse = triangle->transformation.inverse();
        triangle->inverse_transpose = triangle->inverse.transpose();
        surfaces.push_back(triangle);
      }
    }
  }

  vertices.clear();
}

void loadScene(std::string file) {

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
        sphere->diffuse = diffuse->clone();
        sphere->specular = specular->clone();
        sphere->power_coefficient = shininess;
        sphere->emission = emission->clone();
        sphere->transformation = Matrix4f(current_matrix);
        sphere->inverse = sphere->transformation.inverse();
        sphere->inverse_transpose = sphere->inverse.transpose();
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
        float x = atof(splitline[1].c_str());
        float y = atof(splitline[2].c_str());
        float z = atof(splitline[3].c_str());
        // Create a new vertex with these 3 values, store in some array
        ThreeDVector* vertex = new ThreeDVector(x, y, z);
        vertices.push_back(vertex);
      }
      //vertexnormal x y z nx ny nz
      //  Similar to the above, but deﬁne a surface normal with each vertex.
      //  The vertex and vertexnormal set of vertices are completely independent
      //  (as are maxverts and maxvertnorms).
      else if(!splitline[0].compare("vertexnormal")) {
        float x = atof(splitline[1].c_str());
        float y = atof(splitline[2].c_str());
        float z = atof(splitline[3].c_str());
        float nx = atof(splitline[4].c_str());
        float ny = atof(splitline[5].c_str());
        float nz = atof(splitline[6].c_str());
        // Create a new vertex+normal with these 6 values, store in some array
        ThreeDVector* vertex = new ThreeDVector(x, y, z);
        normal_vertices.push_back(vertex);
      }
      //tri v1 v2 v3
      //  Create a triangle out of the vertices involved (which have previously been speciﬁed with
      //  the vertex command). The vertices are assumed to be speciﬁed in counter-clockwise order. Your code
      //  should internally compute a face normal for this triangle.
      else if(!splitline[0].compare("tri")) {
        // Create new triangle:
        //   Store pointer to array of vertices
        //   Store 3 integers to index into array
        //   Store current property values
        //   Store current top of matrix stack
        int v1 = atoi(splitline[1].c_str());
        int v2 = atoi(splitline[2].c_str());
        int v3 = atoi(splitline[3].c_str());
        int max_v1_v2 = max(v1, v2);
        int max_v1_v2_v3 = max(max_v1_v2, v3);
        if (vertices.size() < max_v1_v2_v3 + 1) {
          cerr << "Tried to access vertex that was not defined yet" << endl;
          exit(1);
        }

        ThreeDVector* a = vertices[v1];
        ThreeDVector* b = vertices[v2];
        ThreeDVector* c = vertices[v3];
        Triangle* triangle = new Triangle(a->clone(), b->clone(), c->clone());
        triangle->diffuse = diffuse->clone();
        triangle->specular = specular->clone();
        triangle->power_coefficient = shininess;
        triangle->emission = emission->clone();
        triangle->transformation = Matrix4f(current_matrix);
        triangle->inverse = triangle->transformation.inverse();
        triangle->inverse_transpose = triangle->inverse.transpose();
        surfaces.push_back(triangle);
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
        int v1 = atoi(splitline[1].c_str());
        int v2 = atoi(splitline[2].c_str());
        int v3 = atoi(splitline[3].c_str());
        int max_v1_v2 = max(v1, v2);
        int max_v1_v2_v3 = max(max_v1_v2, v3);
        if (normal_vertices.size() < max_v1_v2_v3 + 1) {
          cerr << "Tried to access vertex that was not defined yet" << endl;
          exit(1);
        }

        ThreeDVector* a = normal_vertices[v1];
        ThreeDVector* b = normal_vertices[v2];
        ThreeDVector* c = normal_vertices[v3];
        Triangle* triangle = new Triangle(a->clone(), b->clone(), c->clone());
        triangle->diffuse = diffuse->clone();
        triangle->specular = specular->clone();
        triangle->power_coefficient = shininess;
        triangle->emission = emission->clone();
        triangle->transformation = Matrix4f(current_matrix);
        triangle->inverse = triangle->transformation.inverse();
        triangle->inverse_transpose = triangle->inverse.transpose();
        surfaces.push_back(triangle);
      }

      //translate x y z
      //  A translation 3-vector
      else if(!splitline[0].compare("translate")) {
        float x = atof(splitline[1].c_str());
        float y = atof(splitline[2].c_str());
        float z = atof(splitline[3].c_str());
        Matrix4f m = Matrix4f::Identity();
        m(0,3) = x;
        m(1,3) = y;
        m(2,3) = z;
        current_matrix = current_matrix * m;
      }
      //rotate x y z angle
      //  Rotate by angle (in degrees) about the given axis as in OpenGL.
      else if(!splitline[0].compare("rotate")) {
        float x = atof(splitline[1].c_str());
        float y = atof(splitline[2].c_str());
        float z = atof(splitline[3].c_str());
        float angle = atof(splitline[4].c_str());
        float theta = angle * 2 * PI / 180;
        ThreeDVector* rotation_axis = new ThreeDVector(x, y, z);
        rotation_axis->normalize_bang();
        
        Matrix4f m = Matrix4f::Identity();

        //From Wikipedia Rotation matrix from axis and angle
        float cos_theta = cos(theta);
        float one_minus_cos_theta = 1 - cos(theta);
        float sin_theta = sin(theta);

        float ux_square = rotation_axis->x * rotation_axis->x;
        float uy_square = rotation_axis->y * rotation_axis->y;
        float uz_square = rotation_axis->z * rotation_axis->z;

        float ux_uy = rotation_axis->x * rotation_axis->y;
        float ux_uz = rotation_axis->x * rotation_axis->z;
        float uy_uz = rotation_axis->y * rotation_axis->z;

        float ux_times_sin_theta = rotation_axis->x * sin_theta;
        float uy_times_sin_theta = rotation_axis->y * sin_theta;
        float uz_times_sin_theta = rotation_axis->z * sin_theta;

        m(0, 0) = cos_theta + ux_square * one_minus_cos_theta;
        m(0, 1) = ux_uy * one_minus_cos_theta - uz_times_sin_theta;
        m(0, 2) = ux_uz * one_minus_cos_theta + uy_times_sin_theta;
        m(1, 0) = ux_uy * one_minus_cos_theta + uz_times_sin_theta;
        m(1, 1) = cos_theta + uy_square * one_minus_cos_theta;
        m(1, 2) = uy_uz * one_minus_cos_theta - ux_times_sin_theta;
        m(2, 0) = ux_uz * one_minus_cos_theta - uy_times_sin_theta;
        m(2, 1) = uy_uz * one_minus_cos_theta + ux_times_sin_theta;
        m(2, 2) = cos_theta + uz_square * one_minus_cos_theta;        

        current_matrix = current_matrix * m;
        // Update top of matrix stack
      }
      //scale x y z
      //  Scale by the corresponding amount in each axis (a non-uniform scaling).
      else if(!splitline[0].compare("scale")) {
        float x = atof(splitline[1].c_str());
        float y = atof(splitline[2].c_str());
        float z = atof(splitline[3].c_str());
        Matrix4f m = Matrix4f::Identity();
        m(0,0) = x;
        m(1,1) = y;
        m(2,2) = z;
        current_matrix = current_matrix * m;
      }
      //pushTransform
      //  Push the current modeling transform on the stack as in OpenGL. 
      //  You might want to do pushTransform immediately after setting 
      //   the camera to preserve the “identity” transformation.
      else if(!splitline[0].compare("pushTransform")) {
        //memcpy(saved_matrix, &current_matrix, sizeof(saved_matrix));
        Matrix4f saved_matrix = Matrix4f(current_matrix);
        matrix_stack.push(saved_matrix);
      }
      //popTransform
      //  Pop the current transform from the stack as in OpenGL. 
      //  The sequence of popTransform and pushTransform can be used if 
      //  desired before every primitive to reset the transformation 
      //  (assuming the initial camera transformation is on the stack as 
      //  discussed above).
      else if(!splitline[0].compare("popTransform")) {
        if(matrix_stack.empty()){
          std::cerr << "Empty stack can't be popped." << endl;
        }
        matrix_stack.pop();
        current_matrix = matrix_stack.top();
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
        delete diffuse;
        diffuse = new ThreeDVector(r, g, b);
      }
      //specular r g b 
      //  speciﬁes the specular color of the surface.
      else if(!splitline[0].compare("specular")) {
        float r = atof(splitline[1].c_str());
        float g = atof(splitline[2].c_str());
        float b = atof(splitline[3].c_str());
        delete specular;
        specular = new ThreeDVector(r, g, b);
      }
      //shininess s
      //  speciﬁes the shininess of the surface.
      else if(!splitline[0].compare("shininess")) {
        // shininess: atof(splitline[1].c_str())
        shininess = atof(splitline[1].c_str());
      }
      //emission r g b
      //  gives the emissive color of the surface.
      else if(!splitline[0].compare("emission")) {
        float r = atof(splitline[1].c_str());
        float g = atof(splitline[2].c_str());
        float b = atof(splitline[3].c_str());
        delete emission;
        emission = new ThreeDVector(r, g, b);
      }
      //obj filename 
      else if(!splitline[0].compare("obj")) {
        const char* file_name = splitline[1].c_str();
        parseObj(file_name);
      } 
      //aa grid_size 
      else if(!splitline[0].compare("aa")) {
        grid_size = atoi(splitline[1].c_str());
      } 
      else {
        std::cerr << "Unknown command: " << splitline[0] << std::endl;
      }
    }
  

    Scene* scene = new Scene(camera, surfaces, lights, width, height, depth, grid_size);

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
