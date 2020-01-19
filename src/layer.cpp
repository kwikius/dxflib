
#include <dxf/layer.hpp>
#include <dxf/file_image.hpp>


std::ostream & dxf::layer_t::output(std::ostream & os)const
{
   if ( get_file_image().header.get_acad_version() > dxf::acad_version_t::R12){
      os << "   100\nAcDbSymbolTableRecord\n";
      os << "   100\nAcDbLayerTableRecord\n";
   }
   os << "   2\n" << m_layer_name << "\n";
   return os;
}
