
#include <map>

#include "dxf/table.hpp"
#include "dxf/file_image.hpp"

namespace {

   std::map<dxf::table_t::table_type_t,std::string> table_output_string_map = {
       
       {dxf::table_t::table_type_t::block_record,"BLOCK_RECORD"},
       {dxf::table_t::table_type_t::dimstyle,"DIMSTYLE"},
       {dxf::table_t::table_type_t::layer,"LAYER"},
       {dxf::table_t::table_type_t::ltype,"LTYPE"},  
       {dxf::table_t::table_type_t::style,"STYLE"},
       {dxf::table_t::table_type_t::ucs,"UCS"},
       {dxf::table_t::table_type_t::view,"VIEW"},
       {dxf::table_t::table_type_t::vport,"VPORT"},    
   };
}

std::ostream& dxf::table_t::output_head (std::ostream& os) const
{
   os << "   0\nTABLE\n   2\n" << table_output_string_map[m_table_type]  <<'\n';
   if ( get_file_image().header.get_acad_version() > dxf::acad_version_t::R12){
      os << "   5\n" << std::hex << get_handle() << std::dec << "\n";
      // Soft-pointer ID/handle to owner object
       os << "   330\n0\n";
       os << "   100\nAcDbSymbolTable\n";
       os << "   70\n" << get_max_entries() << '\n';
   }
   return os;
}


std::ostream& dxf::table_t::output_tail(std::ostream& os) const
{
   return os << "   0\nENDTAB\n";
}
