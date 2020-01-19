
#include <map>
#include <iostream>

#include <dxf/section.hpp>

namespace {

   std::map<dxf::section_t::section_type,std::string> section_output_string_map = {
       {dxf::section_t::section_type::header,"HEADER"},
       {dxf::section_t::section_type::classes,"CLASSES"},
       {dxf::section_t::section_type::tables,"TABLES"},
       {dxf::section_t::section_type::blocks,"BLOCKS"},  
       {dxf::section_t::section_type::entities,"ENTITIES"},
       {dxf::section_t::section_type::objects,"OBJECTS"},
       {dxf::section_t::section_type::thumbnailimage,"THUMBNAILIMAGE"},    
   };
}

std::ostream& dxf::section_t::output_head (std::ostream& os) const
{
   os << "   0\nSECTION\n   2\n" << section_output_string_map[m_section_type]  <<'\n';
   return os;
}


std::ostream& dxf::section_t::output_tail(std::ostream& os) const
{
   return os << "   0\nENDSEC\n";
}
