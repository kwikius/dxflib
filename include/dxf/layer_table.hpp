#ifndef DXF_LAYER_TABLE_HPP_INCLUDED
#define DXF_LAYER_TABLE_HPP_INCLUDED

#include <map>
#include <stdexcept>
#include <dxf/table.hpp>
#include <dxf/layer.hpp>

namespace dxf{

   struct layer_table_t final : table_t {
      layer_table_t() : table_t{table_type_t::layer}{}

      ~layer_table_t()
      {
         for ( auto iter = m_layers_map.begin() ; iter != m_layers_map.end(); ++iter){
            delete iter->second;
            m_layers_map.erase(iter);
         }
      }

      std::ostream & output_derived(std::ostream & os)const
      { 
         for ( auto layer : m_layers_map){
           layer.second->output(os);
         }
         return os;
      }

      bool layer_exists(std::string const & layer_name)const
      {
         return m_layers_map.find(layer_name) != m_layers_map.end();
      }

      void add_layer(layer_t * layer)
      {
         assert(layer != nullptr);
         
         auto const  layer_name = layer->get_layer_name();
         auto const iter = m_layers_map.find(layer_name);
         if (iter == m_layers_map.end()){
            layer->set_file_image(this->get_file_image());
            m_layers_map.insert({layer_name,layer});
            return;
         }
         std::string err_str = "dxf::add_layer, layer \"" + std::string{layer_name} + "\" already exists.";
         throw std::logic_error(err_str);
      }
      
      int16_t get_max_entries() const final { return m_layers_map.size() + size_t{1};}

      private:
         layer_table_t(layer_table_t const & ) = delete;
         layer_table_t(layer_table_t && ) = delete;
         layer_table_t& operator =(layer_table_t const & ) = delete;
         layer_table_t& operator =(layer_table_t && ) = delete;
         std::map<std::string,layer_t*> m_layers_map;
      
   };

}

#endif // DXF_LAYER_TABLE_HPP_INCLUDED
