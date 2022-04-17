

#include <iostream>
#include <fstream>
#include <filesystem>
#include <quan/std/string/unquote.hpp>
#include <dxf/file_image.hpp>


int main(int argc , const char* argv[])
{

   if (argc > 1){
      try{

         dxf::file_image_t dxf_image(argv[1]);

         std::ofstream out{"output_libtest2.dxf"};
         out << dxf_image;
         std::cout << "file written successfully\n";
      }catch(std::exception & e){

         std::cerr << e.what();
         return EXIT_FAILURE;
      }
   }else{
      std::cout << "useage : " << quan::unquote(std::filesystem::path{argv[0]}.filename()) << " <dxf-input-file>\n";
      return EXIT_FAILURE;
   }
}
