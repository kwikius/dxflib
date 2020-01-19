#ifndef DXF_FILE_ELEMENT_HPP_INCLUDED
#define DXF_FILE_ELEMENT_HPP_INCLUDED

#include <cstdint>
#include <cassert>

namespace dxf{

   struct file_image_t;
   struct entities_t;
   struct tables_t;
      
   struct file_element_t{
      int32_t get_handle() const { return m_handle;}
      void set_file_image(file_image_t &);
      file_image_t & get_file_image() { assert(m_file_image != nullptr); return *m_file_image;}
      file_image_t const & get_file_image()const { assert(m_file_image != nullptr);return *m_file_image;}
    protected:
      file_element_t(file_image_t * file_image);
      file_element_t();
    private:

      file_element_t(file_element_t const &) = delete;
      file_element_t(file_element_t &&) = delete;
      file_element_t& operator =(file_element_t const &) = delete;
      file_element_t& operator =(file_element_t &&) = delete;

      friend struct entities_t;
      friend struct tables_t;
      file_image_t * m_file_image;
      int32_t m_handle;
   };

}

#endif // DXF_FILE_ELEMENT_HPP_INCLUDED
