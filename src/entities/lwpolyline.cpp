

#include <iostream>

#include <dxf/entity/lwpolyline.hpp>
#include <dxf/file_image.hpp>


 std::ostream & dxf::lwpolyline_t::output_derived(std::ostream & os)const 
 {
     if ( get_file_image().header.get_acad_version() > dxf::acad_version_t::R12){
        os << "   100\nAcDbPolyline\n";
     }
     // 90 number of vertices
     os << "   90\n" << m_point_list.size() << "\n";
     os << "   70\n" << ((m_closed) ? 1 : 0) << "\n";
     int16_t id = 1;
     for ( auto const & pt : m_point_list){
      //   os << "   71\n" << id << "\n";
         os << "   10\n" << pt.x  << "\n   20\n"  << pt.y << "\n";
         ++id;
     }
     return os;
 }