

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <list>

#include <quan/two_d/out/line.hpp>
#include <quan/two_d/out/vect.hpp>
#include <quan/two_d/out/line.hpp>
#include <quan/two_d/out/box.hpp>
#include <quan/two_d/clip.hpp>

#include <dxf/file_image.hpp>
#include <dxf/entity/line.hpp>

bool dxf_to_clip_boxes(const char *  input_file, std::list<quan::two_d::box<double> > & out);

namespace {
   template <typename T>
   inline constexpr quan::two_d::vect<T>
   to_vect_xy(quan::three_d::vect<T> const & v)
   {
      return {v.x,v.y};
   }

   template <typename T>
   inline constexpr quan::three_d::vect<T>
   to_vect_xyz(quan::two_d::vect<T> const & v)
   {
      return {v.x,v.y,T{}};
   }

   using vect_t = quan::two_d::vect<double>;
   using point_t = vect_t;
   using box_t = quan::two_d::box<double>;
   using line_t = quan::two_d::line<vect_t>;
   using clip_t = quan::two_d::clip<double>;
   using trisect_t = quan::two_d::trisect<double>;
   using entity_iterator = std::list<dxf::entity_t*>::iterator;

}

entity_iterator line_erase_boxed_region(entity_iterator iter, box_t const & box)
{
    dxf::line_t * dxf_line = dynamic_cast<dxf::line_t *>(*iter);
    if ( dxf_line != nullptr){
       clip_t clip{box};
       line_t line{to_vect_xy(dxf_line->m_start),to_vect_xy(dxf_line->m_end)};
        std::cout << line <<'\n';
       auto const clipped = clip(line);
       if ( clipped.first){
           // std::cout << "some or all of line is in clipped region\n";
           auto const clipped_line = clipped.second;
           std::cout << "clipped = " << clipped_line <<'\n';
           auto & entities = dxf_line->get_file_image().entities.get_entities();
           if  ((clipped_line.from == line.from ) && (clipped_line.to == line.to )) {
               std::cout << "all inside box - remove line\n";
               return entities.erase(iter);
           }else{
              if ((clipped_line.from != line.from ) && (clipped_line.to != line.to )){
                 std::cout << "both ends clipped\n";
                 auto new_line = new dxf::line_t{to_vect_xyz(line.from), to_vect_xyz(clipped_line.from)} ;

                 dxf_line->m_start = to_vect_xyz(clipped_line.to);
                //  inserts new_line before iter
                 new_line->set_file_image(dxf_line->get_file_image());
                 entities.insert(iter,new_line);
              }else {
                 if  (clipped_line.from != line.from ){
                    std::cout << "from end clipped\n";
                    dxf_line->m_end = to_vect_xyz(clipped_line.from);
                 }else{
                    if  (clipped_line.to != line.to ){
                       std::cout << "to end clipped\n";
                       dxf_line->m_start = to_vect_xyz(clipped_line.to);
                    }
                 }
              }
           }
       }else{
         std::cout << "not clipped\n";
       }
   }
   return ++iter;
}

/*
  dxf_removetabs  -g graphic_dxf -c clipboxdxf  -o out.dxf
*/
int main(int argc, char *argv[])
{
   const char* graphic_filename = "" ;
   const char* clipbox_filename = "";
   const char* out_filename = "" ;
   //get opt
   {
      int opt = 0;
      int arg_flags = 0;
      int constexpr have_graphic_filename = 1;
      int constexpr have_clipbox_filename = 2;
      int constexpr have_output_filename = 4;
      while ( (opt = getopt(argc, argv, "g:c:o:")) != -1) {
         switch (opt){
             case 'g' : {
               graphic_filename = optarg;
               arg_flags |= have_graphic_filename;
               break;
             }
             case 'c' : {
               clipbox_filename = optarg;
               arg_flags |= have_clipbox_filename;
               break;
             }
             case 'o' : {
               out_filename = optarg;
               arg_flags |= have_output_filename;
               break;
             }
             default  : std::cerr << "invalid arg\n"; break;
         }
      }

      if( (arg_flags & have_graphic_filename) == 0){
         std::cerr   <<  "expected graphic filename\n";
         return EXIT_FAILURE;
      }

      if( (arg_flags & have_clipbox_filename) == 0){
         std::cerr   <<  "expected clipbox filename\n";
         return EXIT_FAILURE;
      }

      if( (arg_flags & have_output_filename) == 0){
         std::cerr   <<  "expected output filename\n";
         return EXIT_FAILURE;
      }
   }
   // args ok
   std::list<quan::two_d::box<double> >  clip_boxes;
   if (dxf_to_clip_boxes(clipbox_filename , clip_boxes)){

      dxf::file_image_t dxf_image(graphic_filename);

      for (auto const & box : clip_boxes){
         auto entity_iter = dxf_image.entities.get_entities().begin();
         while (entity_iter != dxf_image.entities.get_entities().end()){
            entity_iter = line_erase_boxed_region(entity_iter,box);
         }
      }
      //output modified dxf
      dxf_image.set_acad_version(dxf::acad_version_t::R14);
      std::ofstream out{out_filename};
      out << dxf_image;
  } else{
    std::cout << "fail\n";
  }

}
