//section.h
#ifndef DXF_SECTION_HPP_INCLUDED
#define DXF_SECTION_HPP_INCLUDED

#include <iosfwd>
#include <cassert>
#include <iostream>

#include <dxf/file_element.hpp>

namespace dxf {

   struct file_image_t;

	struct section_t : file_element_t{

      enum class section_type{
         unknown,
         header,
         classes,
         tables,
         blocks,
         entities,
         objects,
         thumbnailimage 
      };

		std::ostream& output(std::ostream& os)const
      {
          output_head(os);
          output_derived(os);
          output_tail(os);
          return os;
      }

		virtual ~section_t(){}
      section_type get_section_type()const {return m_section_type;}
    protected:
      section_t(file_image_t & file_image,section_type st):file_element_t{&file_image},m_section_type{st}{}
      
      virtual std::ostream& output_derived(std::ostream&)const=0;

    private:

      section_t ( section_t const &) = delete;
      section_t ( section_t &&) = delete;
      section_t & operator =( section_t const &) = delete;
      section_t & operator =( section_t &&) = delete;

		std::ostream& output_head (std::ostream&) const;
		std::ostream& output_tail(std::ostream& os) const;

      section_type m_section_type;

	};

}
#endif //DXF_SECTION_HPP_INCLUDED
