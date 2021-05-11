#ifndef DXF_ENTITY_FACE3D_HPP_INCLUDED
#define DXF_ENTITY_FACE3D_HPP_INCLUDED

#include "dxf/entity.hpp"
#include <quan/three_d/vect.hpp>

namespace dxf{

   struct file_image_t;

   struct face3d_t final : entity_t {

       using vect = quan::three_d::vect<double>;

       face3d_t(
            vect const & p0,vect const & p1, vect const & p2, vect const & p3,
            uint16_t visibility_flags
       )
       :entity_t{entity_type_t::face3d}
       ,m_points({p0,p1,p2,p3})
       ,m_visibility_flags(visibility_flags)
       {}

       face3d_t(
            vect (&points)[4],
            uint16_t visibility_flags
       )
       :entity_t{entity_type_t::face3d}
       ,m_points(points)
       ,m_visibility_flags(visibility_flags)
       {}


       face3d_t(
            vect (&points)[3],
            uint16_t visibility_flags
       )
       :entity_t{entity_type_t::face3d}
       ,m_points({points[0],points[1],points[2],points[2]})
       ,m_visibility_flags(visibility_flags)
       {
       }

       std::ostream & output_derived(std::ostream & os)const ;
      
       vect m_points [4];
       uint16_t m_visibility_flags;
   };
}

#endif //DXF_ENTITY_FACE3D_HPP_INCLUDED
