
#include <fstream>
#include "aerofoil/aerofoil.hpp"
#include <dxf/file_image.hpp>

int main(int argc,const char* argv[])
{
   dxf::file_image_t file_image{dxf::acad_version_t::R14};

   aerofoil foil("/home/andy/cpp/projects/aerofoil/Sections/selig/ag16.dat");

   std::ofstream out("ag16_150_mm_1_mm.dxf");

   foil.output_dxf(out,quan::length::mm{150},quan::length::mm{1});
}