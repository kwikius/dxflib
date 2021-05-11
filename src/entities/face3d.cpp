
#include <iostream>

#include <dxf/entity/face3d.hpp>
#include <dxf/file_image.hpp>
/*
 3DFACE    Four points defining the corners of the face: (10, 20, 30),
            (11, 21, 31), (12, 22, 32), and (13, 23, 33).  70 (invisible
            edge flags -optional 0).  If only three points were entered
            (forming a triangular face), the third and fourth points will
            be the same.  The meanings of the bit-coded "invisible edge
            flags" are shown in the following table.

                      Flag bit value           Meaning
                             1        First edge is invisible
                             2        Second edge is invisible
                             4        Third edge is invisible
                             8        Fourth edge is invisible
*/

 std::ostream & dxf::face3d_t::output_derived(std::ostream & os)const 
 {
     if ( get_file_image().header.get_acad_version() > dxf::acad_version_t::R12){
        os << "   100\nAcDbFace\n"; 
     }
     for ( uint32_t i = 0U; i < 4 ; ++i){
        os << "   1" << i << '\n' << m_points[i].x << "\n   2" << i << '\n' << m_points[i].y << "\n   3" << i << '\n' << m_points[i].z << '\n';
     }
     os << "   70\n" << m_visibility_flags << "\n";
     return os;
 }
