#ifndef DXF_TABLE_HPP_INCLUDED
#define DXF_TABLE_HPP_INCLUDED

#include "dxf/file_element.hpp"
#include <iostream>

namespace dxf{

   struct file_image_t;

   struct table_t : file_element_t{

      enum class table_type_t{
         unknown,
         appid,
         block_record,
         dimstyle,
         layer,
         ltype,
         style,
         ucs,
         view,
         vport
      };

      virtual ~table_t (){}
      virtual std::ostream & output_derived(std::ostream & out)const =0;
      virtual int16_t get_max_entries() const = 0;
      std::ostream & output(std::ostream & os) const
      {
          output_head(os);
          output_derived(os);
          output_tail(os);
          return os;
      }
      table_type_t get_table_type() const {return m_table_type;}

      table_t(table_type_t table_type)
      : file_element_t{}
      ,m_table_type{table_type}
      {}
     
     private:
      table_t (table_t const &) = delete;
      table_t (table_t &&) = delete;
      table_t& operator = (table_t const &) = delete;
      table_t& operator = (table_t &&) = delete;

      std::ostream & output_head(std::ostream & os) const;
      std::ostream & output_tail(std::ostream & os) const;

      table_type_t m_table_type;
   };
}



#endif // DXF_TABLE_HPP_INCLUDED
