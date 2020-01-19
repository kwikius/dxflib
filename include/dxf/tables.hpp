#ifndef DXF_TABLES_HPP_INCLUDED
#define DXF_TABLES_HPP_INCLUDED

#include <list>
#include <dxf/section.hpp>
#include <dxf/table.hpp>
#include <dxf/layer_table.hpp>

namespace dxf {

   /*
      section containing all the tables
      They are held in a list of tables
      to find if a particular table exists
      use get_table (id);
   */

   struct tables_t final : section_t{

      tables_t(file_image_t & file_image) 
      :section_t{ file_image,section_type::tables} 
      {}

      ~tables_t()
      {
         for ( auto & elem : m_tables){
            delete elem;
            elem = nullptr;
         }
      }

      std::ostream& output_derived(std::ostream& os)const final
      {
          for(auto table : m_tables){
             table->output(os);
          }
          return os;
      }

      void add(table_t * table) 
      {
         assert(table != nullptr);
         table->set_file_image(this->get_file_image());

         auto const table_type = table->get_table_type();
         // check table is not already here
         assert(get_table(table_type) == nullptr);
         // from ref man
         // "The order of the tables may change, but the LTYPE table always precedes the
         // LAYER table." ( so just put ltype table at the front)
         if ( table_type == table_t::table_type_t::ltype){
           m_tables.push_front(table);
         }else{
            m_tables.push_back(table);
         }
      }
       
      // layers table should always be there
      layer_table_t* get_layers_table() const
      {
         auto table = get_table( table_t::table_type_t::layer);
         assert(table);
         auto r = dynamic_cast<layer_table_t*>(table);
         assert(r);
         return r;
      }

      
      table_t* get_table(table_t::table_type_t id)const
      {
         for ( auto iter = m_tables.begin(); iter != m_tables.end(); ++iter) 
         {
            if ((*iter)->get_table_type() == id){
               return *iter;
            }
         }
         return nullptr;
      }
      private:
         tables_t (tables_t const &) = delete;
         tables_t (tables_t &&) = delete;
         tables_t& operator= (tables_t const &) = delete;
         tables_t& operator= (tables_t &&) = delete;

         std::list<table_t*> m_tables;
   };

} // dxf

#endif // DXF_TABLES_HPP_INCLUDED
