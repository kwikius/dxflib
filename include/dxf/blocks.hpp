#ifndef DXF_BLOCKS_HPP_INCLUDED
#define DXF_BLOCKS_HPP_INCLUDED

#include <dxf/section.hpp>

namespace dxf {

   struct blocks_t final : section_t{

      blocks_t(file_image_t & file_image) 
      :section_t{file_image,section_type::blocks}
      {}

      std::ostream& output_derived(std::ostream& os)const
      {
         return os;
      }
   };

}

#endif // DXF_BLOCKS_HPP_INCLUDED
