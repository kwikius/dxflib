#ifndef DXF_ENTITIES_HPP_INCLUDED
#define DXF_ENTITIES_HPP_INCLUDED

#include <list>

#include <dxf/section.hpp>
#include <dxf/entity.hpp>

namespace dxf {

   struct entities_t final : section_t{

      entities_t(file_image_t & file_image) 
      :section_t{ file_image,section_type::entities} 
      {}

      ~entities_t()
      {
         for ( auto & elem : m_entities){
            delete elem;
            elem = nullptr;
         }
      }

      std::ostream& output_derived(std::ostream& os)const final
      {
          for(auto elem : m_entities){
             elem->output(os);
          }
          return os;
      }

      void add(entity_t * entity) 
      {
         assert(entity != nullptr);
         entity->set_file_image(this->get_file_image());
         m_entities.push_back(entity);
      }
      private:

         entities_t(entities_t const &) = delete;
         entities_t(entities_t &&) = delete;
         entities_t & operator=(entities_t const & ) = delete;  
         entities_t & operator=(entities_t && ) = delete;
     
         std::list<entity_t*> m_entities;

   };

}

#endif // DXF_ENTITIES_HPP_INCLUDED
