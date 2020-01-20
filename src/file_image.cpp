

#include <dxf/file_image.hpp>
#include <dxf/layer_table.hpp>

dxf::file_image_t::file_image_t(dxf::acad_version_t version)
: header{*this}
,tables{*this}
,blocks{*this}
,entities{*this}
{
   header.set_acad_version(version);
   auto layer = new layer_t{"0"};
   auto layer_table = new layer_table_t(); 
   tables.add(layer_table);
   layer_table->add_layer(layer);
}
namespace dxf {

   std::ostream & operator << (std::ostream & os, file_image_t const & image)
   {
      image.output(os);
      return os;
   }
}

std::ostream & dxf::file_image_t::output(std::ostream & os)const
{
   if (header.get_acad_version() > dxf::acad_version_t::MINIMAL){
      header.output(os);
      tables.output(os);
      blocks.output(os);
   }

   entities.output(os);

   os << "   0\nEOF\n";
   return os;
}

void dxf::file_image_t::add_layer(dxf::layer_t* layer)
{
   tables.get_layers_table()->add_layer(layer);
}

bool dxf::file_image_t::layer_exists(std::string const & layer_name)const
{
   return tables.get_layers_table()->layer_exists(layer_name);
}
