#ifndef DXF_HEADER_HPP_INCLUDED
#define DXF_HEADER_HPP_INCLUDED

#include <iostream>
#include <string>
#include <quan/max.hpp>

#include <dxf/section.hpp>
#include <dxf/acad_version.hpp>

namespace dxf{

   // Header is only required after R12
   // After R12  Heder requires $ACADVER and $HANDSEED
   // Inkscape says it requires R13

   struct header_t final : section_t{

      // dxf output is dependent on acad version
      // TODO ADD a Minimal file version
      // which just does minimal output?


      header_t(file_image_t & file_image)
         :section_t{file_image,section_type::header}
         ,m_acad_version{acad_version_t::MINIMAL}
         ,m_handle_seed{257}
         {}

      void set_acad_version(acad_version_t version){m_acad_version = version;}

      std::ostream& output_derived(std::ostream& os)const final
      {
         os << "   9\n$ACADVER\n   1\n" <<  get_acad_version_string() << '\n';
         if ( get_acad_version() >= acad_version_t::R13){
          os << "   9\n$HANDSEED\n   5\n" <<  std::hex << get_handle_seed() << '\n';
          os << "   9\n$MEASUREMENT\n   70\n1\n";
         }
         return os;
      }

      const char * const get_acad_version_string()const;
      acad_version_t get_acad_version() const {return m_acad_version;}
      uint64_t get_new_handle()
      {
         uint64_t const result = m_handle_seed;
         ++m_handle_seed;
         return result;
      }
      void set_handle_seed( uint64_t val)
      {
         m_handle_seed = quan::max( m_handle_seed,val);
      }

      private:

         header_t(header_t const &) = delete;
         header_t(header_t &&) = delete;
         header_t & operator =(header_t const &) = delete;
         header_t& operator = (header_t &&) = delete;

         uint64_t get_handle_seed()const { return m_handle_seed ;}
         acad_version_t m_acad_version;
         uint64_t m_handle_seed;


   };

};


#endif // DXF_HEADER_HPP_INCLUDED
