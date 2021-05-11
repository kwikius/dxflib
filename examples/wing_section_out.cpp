
#include <fstream>
#include "aerofoil/aerofoil.hpp"
#include <dxf/file_image.hpp>

int main(int argc,const char* argv[])
{
   aerofoil foil("/home/andy/cpp/projects/aerofoil/Sections/selig/ag34.dat");

   std::ofstream out("ag34_200_mm_1_mm.dxf");

   foil.output_dxf(out,quan::length::mm{200},quan::length::mm{1});
}