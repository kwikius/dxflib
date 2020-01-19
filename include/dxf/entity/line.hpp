#ifndef DXF_ENTITY_LINE_HPP_INCLUDED
#define DXF_ENTITY_LINE_HPP_INCLUDED

#include "dxf/entity.hpp"

#include <quan/three_d/vect.hpp>

namespace dxf{

   struct file_image_t;

   struct line_t final : entity_t {

       typedef quan::three_d::vect<double> vect;

       line_t(vect const & start, vect const & end)
       :entity_t{entity_type_t::line}
       ,m_start{start}
       ,m_end{end}
       {}

       std::ostream & output_derived(std::ostream & os)const ;

       vect m_start;
       vect m_end;
   };
}

#endif // DXF_ENTITY_LINE_HPP_INCLUDED
