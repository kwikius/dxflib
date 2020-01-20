
#include <iostream>
#include <fstream>
#include <dxf/file_image.hpp>

int main(int argc , const char* argv[])
{
   std::cout << "dxf input\n";
   if (argc > 1){
      try{

         dxf::file_image_t dxf_image(argv[1]);
         // for inkscape minimum version is R14
         dxf_image.set_acad_version(dxf::acad_version_t::R14);
         std::ofstream out{"output_test2.dxf"};
         out << dxf_image;

      }catch(std::exception & e){

         std::cout << e.what();
         return EXIT_FAILURE;
      }
   }else{
      std::cout << "useage : " << argv[0] << "<dxf-input-file>\n";
      return EXIT_FAILURE;
   }
}


