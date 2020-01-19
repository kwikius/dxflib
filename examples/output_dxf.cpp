
#include <fstream>

#include <dxf/file_image.hpp>
#include <dxf/entity/line.hpp>
#include <dxf/entity/lwpolyline.hpp>
#include <dxf/colours.hpp>
#include <dxf/layer.hpp>

int main()
{
   dxf::file_image_t file_image{dxf::acad_version_t::R14};

   auto layer = new dxf::layer_t{"MyLayer1"};
   
   file_image.add_layer(layer);

   dxf::lwpolyline_t * poly = new dxf::lwpolyline_t {
      {0.0,0.0},
      {100.0,100.0},
      {200.0,100.0},
      {100.0,0.0} 
   };

   poly->set_layer_name(layer->get_layer_name());
   poly->set_closed(true);
   poly->set_colour_number(dxf::colour::cyan);
   file_image.entities.add(poly);

   std::ofstream out{"outputx.dxf"};

   file_image.output(out);
  
   return 0;
}