
#include <fstream>

#include <dxf/file_image.hpp>
#include <dxf/entity/line.hpp>
#include <dxf/entity/lwpolyline.hpp>
#include <dxf/colours.hpp>
#include <dxf/layer.hpp>

/*
 make a cube from lines and output it to a dxf file
  then load it into plib example to view in 3d
*/


void make_cube(
   double const & cube_size,
   dxf::file_image_t & fi,dxf::layer_t const & layer,
   quan::three_d::vect<double> const & pos
)
{
   using pt = dxf::line_t::vect;
   using line = dxf::line_t;

   pt p0{0,0,0};
   pt p1{cube_size,0,0};
   pt p2{cube_size,cube_size,0};
   pt p3{0,cube_size,0};
   
   pt p4{0,0,cube_size};
   pt p5{cube_size,0,cube_size};
   pt p6{cube_size,cube_size,cube_size};
   pt p7{0,cube_size,cube_size};

   auto add_line = [&fi ,&layer, & pos] (pt const & start, pt const & end){
      auto ln = new line (start + pos,end + pos);
      ln->set_layer_name(layer.get_layer_name());
      ln->set_colour_number(dxf::colour::red);
      fi.entities.add(ln);
   };

   add_line(p0,p1);
   add_line(p1,p2);
   add_line(p2,p3);
   add_line(p3,p0);

   add_line(p0,p4);
   add_line(p1,p5);
   add_line(p2,p6);
   add_line(p3,p7);

   add_line(p4,p5);
   add_line(p5,p6);
   add_line(p6,p7);
   add_line(p7,p4);

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

   std::ofstream out{"/home/andy/cpp/projects/plib-examples/plib_examples-1.8.5/src/ssg/load_save/data/cube_from_lines.dxf"};

   out << file_image;

   std::cout << file_image <<'\n';
  
   return 0;
}