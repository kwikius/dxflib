
#include <map>

#include <dxf/header.hpp>

namespace {

   const std::map<dxf::acad_version_t,const char * const> acad_version_map = {
      {dxf::acad_version_t::AC1006,"AC1006"},
      {dxf::acad_version_t::AC1009,"AC1009"},
      {dxf::acad_version_t::AC1012,"AC1012"},
      {dxf::acad_version_t::AC1014,"AC1014"},
      {dxf::acad_version_t::AC1015,"AC1015"},
      {dxf::acad_version_t::AC1018,"AC1018"},
      {dxf::acad_version_t::AC1021,"AC1021"},
      {dxf::acad_version_t::AC1024,"AC1024"}
   };

} //namespace

const char * const dxf::header_t::get_acad_version_string()const
{
   return acad_version_map.at(m_acad_version);
}
