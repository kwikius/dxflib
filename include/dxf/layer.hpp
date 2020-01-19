#ifndef DXF_LAYER_HPP_INCLUDED
#define DXF_LAYER_HPP_INCLUDED

#include <string>
#include <iostream>
#include <dxf/file_element.hpp>

namespace dxf{

    /*
      Layer is indexed by name so name is unique in app
      has attributes , such as line weight plot-style color number, layer frozen etc
   */
   struct layer_t final : file_element_t {
      layer_t(std::string const & layer_name)
      :m_layer_name{layer_name}
      {}
      std::string const & get_layer_name()const { return m_layer_name;}

      std::ostream & output(std::ostream & os)const;

   private:

      layer_t( layer_t const &) = delete;
      layer_t( layer_t &&) = delete;
      layer_t& operator=(layer_t const &) = delete;
      layer_t& operator=(layer_t &&) = delete;

      std::string m_layer_name;
   };
}

#endif // DXF_LAYER_HPP_INCLUDED
