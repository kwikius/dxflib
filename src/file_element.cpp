

#include <dxf/file_element.hpp>
#include <dxf/file_image.hpp>


dxf::file_element_t::file_element_t():m_file_image(nullptr),m_handle{0}{}

dxf::file_element_t::file_element_t(dxf::file_image_t * file_image)
:m_file_image{(assert(file_image != nullptr),file_image)}
,m_handle{file_image->header.get_new_handle()}
{}

void dxf::file_element_t::set_file_image(dxf::file_image_t & file_image)
{
   if ( m_file_image != &file_image){
      m_file_image = &file_image; 
      m_handle = file_image.header.get_new_handle();
   }
}
