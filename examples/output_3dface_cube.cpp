
#include <fstream>

#include <dxf/file_image.hpp>
#include <dxf/entity/line.hpp>
#include <dxf/entity/lwpolyline.hpp>
#include <dxf/entity/face3d.hpp>
#include <dxf/colours.hpp>
#include <dxf/layer.hpp>

/*
 make a cube from 3dfaces and output it to a dxf file
  then load it into plib example to view in 3d
*/


void make_cube(
   double const & cube_size,
   dxf::file_image_t & fi,dxf::layer_t const & layer,
   quan::three_d::vect<double> const & pos
)
{
   using pt = dxf::line_t::vect;
   using face3d = dxf::face3d_t;

   pt p0{0,0,0};
   pt p1{cube_size,0,0};
   pt p2{cube_size,cube_size,0};
   pt p3{0,cube_size,0};
   
   pt p4{0,0,cube_size};
   pt p5{cube_size,0,cube_size};
   pt p6{cube_size,cube_size,cube_size};
   pt p7{0,cube_size,cube_size};

   auto add_face = [&fi ,&layer, & pos] (pt const & p1, pt const & p2, pt const & p3, pt const & p4){
      auto face = new face3d(p1 + pos,p2 + pos,p3 + pos,p4 + pos, 0);
      face->set_layer_name(layer.get_layer_name());
      face->set_colour_number(dxf::colour::red);
      fi.entities.add(face);
   };

   add_face(p0,p1,p2,p3);
   add_face(p0,p1,p5,p4);
   add_face(p1,p2,p6,p5);
   add_face(p2,p3,p7,p6);
   add_face(p3,p0,p4,p7);
   add_face(p4,p5,p6,p7);

}

int main()
{
   dxf::file_image_t file_image{dxf::acad_version_t::MINIMAL};

   auto layer = new dxf::layer_t{"MyLayer1"};
   file_image.add_layer(layer);

   using pt = dxf::line_t::vect;
   pt translation{-0.5,-0.5,0.25};
   double cube_size = 1.0;

   make_cube(cube_size,file_image,*layer,translation);

   std::ofstream out{"/home/andy/cpp/projects/plib-examples/plib_examples-1.8.5/src/ssg/load_save/data/cube_from_faces.dxf"};

   out << file_image;

   std::cout << file_image <<'\n';
  
   return 0;
}