#ifndef DXF_ENTITY_HPP_INCLUDED
#define DXF_ENTITY_HPP_INCLUDED

#include <iostream>
#include <string>

#include <dxf/file_element.hpp>

namespace dxf{

   struct file_image_t;

   struct entity_t : file_element_t{

      enum entity_type_t{
         unknown,
         line,
         vertex,
         lwpolyline
      };

      virtual ~entity_t (){}
      virtual std::ostream & output_derived(std::ostream & out)const =0;
      std::ostream & output(std::ostream & os) const
      {
          output_head(os);
          output_derived(os);
          output_tail(os);
          return os;
      }

      void set_layer_name(std::string const & name)
      {
         m_layer_name = name;
      }

      entity_type_t get_entity_type() const {return m_entity_type;}
      std::string get_layer_name() const { return m_layer_name;}

      int16_t get_colour_number() const { return m_colour_number;}
      void set_colour_number(int16_t colour_number) { m_colour_number = colour_number;}
     protected:

      entity_t(entity_type_t entity_type)
      :file_element_t{}
      ,m_entity_type{entity_type}
      ,m_layer_name{"0"}
      ,m_colour_number{256}
      {}
      
     private:

      entity_t ( entity_t const &) = delete;
      entity_t (entity_t &&) = delete;
      entity_t& operator = ( entity_t const &) = delete;
      entity_t& operator = ( entity_t &&) = delete;

      std::ostream & output_head(std::ostream & os) const;
      std::ostream & output_tail(std::ostream & os) const;
      entity_type_t m_entity_type;
      std::string m_layer_name;
      int16_t m_colour_number;
      
   };
}

#endif // DXF_ENTITY_HPP_INCLUDED
