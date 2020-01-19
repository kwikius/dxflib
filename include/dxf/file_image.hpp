#ifndef DXF_FILE_IMAGE_HPP_INCLUDED
#define DXF_FILE_IMAGE_HPP_INCLUDED

#include <iostream>

#include <dxf/header.hpp>
#include <dxf/tables.hpp>
#include <dxf/blocks.hpp>
#include <dxf/entities.hpp>
#include <stdexcept>

namespace dxf{

   struct layer_t ;

   /*
      cloning etc?
      probably should be clonable
      move
   */

   struct file_image_t{

      file_image_t( dxf::acad_version_t version);

      file_image_t( const char * filename);


      header_t header;
      tables_t tables;
      blocks_t blocks;
      entities_t entities;

      void add_layer( layer_t*);
      
      std::ostream & output(std::ostream & os) const;

    private:

      file_image_t( file_image_t const & ) = delete;
      file_image_t& operator = ( file_image_t const & ) = delete;
      file_image_t( file_image_t && ) = delete;
      file_image_t& operator = ( file_image_t && ) = delete;
   };

   std::ostream & operator << (std::ostream &, file_image_t const & image);
}

#endif // DXF_FILE_IMAGE_HPP_INCLUDED
