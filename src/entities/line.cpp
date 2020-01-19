
#include <iostream>

#include <dxf/entity/line.hpp>
#include <dxf/file_image.hpp>

 std::ostream & dxf::line_t::output_derived(std::ostream & os)const 
 {
     if ( get_file_image().header.get_acad_version() > dxf::acad_version_t::R12){
        os << "   100\nAcDbLine\n";
     }
     os << "   10\n" << m_start.x << "\n   20\n" << m_start.y << "\n   30\n" << m_start.z << "\n";
     os << "   11\n" << m_end.x << "\n   21\n" << m_end.y << "\n   31\n" << m_end.z << "\n";
     return os;
 }
