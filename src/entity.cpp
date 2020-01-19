
#include <map>

#include <dxf/file_image.hpp>
#include <dxf/entity.hpp>
#include <dxf/header.hpp>

namespace{

   std::map<dxf::entity_t::entity_type_t,std::string> entity_output_string_map = {
      {dxf::entity_t::entity_type_t::line,"LINE"},
      {dxf::entity_t::entity_type_t::vertex,"VERTEX"},
      {dxf::entity_t::entity_type_t::lwpolyline,"LWPOLYLINE"}
   };
}

std::ostream & dxf::entity_t::output_head(std::ostream & os) const
{
   os << "   0\n" << entity_output_string_map.at(m_entity_type) << "\n";
   if ( get_file_image().header.get_acad_version() > dxf::acad_version_t::R12){
      // handle
      os << "   5\n" << std::hex << get_handle() << std::dec << "\n";
      os << std::dec << "   100\nAcDbEntity\n";
      os << "   8\n" << get_layer_name() << '\n';
      os << "   62\n" << get_colour_number() << "\n"; // default color number is 
   }
   return os;
}

std::ostream & dxf::entity_t::output_tail(std::ostream & os) const
{
    return os;
}
