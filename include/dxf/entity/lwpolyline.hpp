

#ifndef DXF_LWPOLYLINE1_HPP_INCLUDED
#define DXF_LWPOLYLINE1_HPP_INCLUDED

/*
  LWPOLYLINE for lightweight polyline
  only 2d points in x and y
*/

#include <list>

#include "dxf/entity.hpp"
#include <quan/two_d/vect.hpp>
#include <initializer_list>

namespace dxf {

   struct lwpolyline_t : entity_t {

      static constexpr bool open = false;
      static constexpr bool closed = true;

       typedef quan::two_d::vect<double> point;

       lwpolyline_t():entity_t{entity_type_t::lwpolyline}{}

       lwpolyline_t(std::initializer_list<point> l)
       :entity_t{entity_type_t::lwpolyline},
        m_point_list(l),
        m_closed{true}
        {}
       
       void push_front(point const & pt)
       {
          m_point_list.push_front(pt);
       }

       void push_back(point const & pt)
       {
          m_point_list.push_back(pt);
       }

       void set_closed( bool b) { m_closed = b;}

       std::ostream & output_derived(std::ostream & os)const 
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
     
     private:
       std::list<point> m_point_list;
       bool m_closed;

   };

}

#endif // DXF_LWPOLYLINE1_HPP_INCLUDED
