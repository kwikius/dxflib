#ifndef DXF_ACAD_VERSION_HPP_INCLUDED
#define DXF_ACAD_VERSION_HPP_INCLUDED

namespace dxf{

   enum class acad_version_t{
      MINIMAL,  // minimal dxf file
      AC1006,    // R10
            R10 = AC1006,
      AC1009,    // R11, R12
            R11 = AC1009,
            R12 = AC1009,
      AC1012,    // R13
            R13 = AC1012,
      AC1014,    // R14
            R14 = AC1014,
      AC1015,    // AUTOCAD2000
      AC1018,    // AUTOCAD2004
      AC1021,    // AUTOCAD2007
      AC1024     // AUTOCAD2010
   };

} //dxf


#endif // DXF_ACAD_VERSION_HPP_INCLUDED
