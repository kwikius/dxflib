
#include <fstream>
#include <dxf/file_image.hpp>
#include <dxf/entity/line.hpp>
#include <sstream>

#include "aerofoil.hpp"

bool aerofoil::output_dxf(
      std::ostream & out,
      quan::length::mm const & chord, 
      quan::length::mm const & te_thickness)const
{
    dxf::file_image_t file_image{dxf::acad_version_t::R14};

    quan::two_d::vect<quan::length::mm> first_coord;
    quan::two_d::vect<quan::length::mm> prev_coord;
    
    bool start = true;
    int surface = 1;
    for ( auto pt : get_coords_vect()){
      
       quan::two_d::vect<quan::length::mm> dxf_cd{
         pt.x * chord , 
         pt.y * chord + (surface * pt.x * te_thickness / 2.0 )
       };
       
       if (start){
          first_coord = dxf_cd;
          prev_coord = dxf_cd;
          start = false;
       }else{
          if ( surface == 1 ) {
            if( prev_coord.x == dxf_cd.x){
               surface = 0;
            }else{
               if (prev_coord.x < dxf_cd.x){
                  surface = -1;
               }
            }
         }
         if ( surface == 0) {
            if (prev_coord.x < dxf_cd.x){
               surface = -1;
            }
         }
         auto const line = new dxf::line_t {
               quan::three_d::vect<double>{
                  prev_coord.x.numeric_value(),
                  prev_coord.y.numeric_value(),
                  0.0
               },
               quan::three_d::vect<double>{
                  dxf_cd.x.numeric_value(),
                  dxf_cd.y.numeric_value(),
                  0.0
               }
          };
          file_image.entities.add(line);
       }
       prev_coord = dxf_cd;
    }
    // close the path
    auto const line = new dxf::line_t {
      quan::three_d::vect<double>{
         prev_coord.x.numeric_value(),
         prev_coord.y.numeric_value(),
         0.0
      },
      quan::three_d::vect<double>{
         first_coord.x.numeric_value(),
         first_coord.y.numeric_value(),
         0.0
         }
    };
    file_image.entities.add(line);
    out << file_image;
    return true;
}

namespace {

   void strip_leading(std::string & str)
   {
      while (isspace(str[0])){
        str = str.substr(1,std::string::npos);
      }
   }

   void strip_trailing(std::string & str)
   {
      for(;;){
         auto len = str.length();
         if (isspace(str[len-1])){
           str = str.substr(0,len-1);
         }else{
           break;
         }
      }
   }
}

bool aerofoil::load(std::string const & data_file)
{
   std::ifstream f(data_file);
   if ( ! f || f.fail()){
      throw std::runtime_error( "Invalid input file");
   }
   // get the aerofoil name
   std::string aerofoil_name;
   for(;;){
      if ( !f || f.eof() ){
         throw std::runtime_error("expected aerofoil name and coords");
      }
      getline(f,aerofoil_name);
      strip_leading(aerofoil_name);
      strip_trailing(aerofoil_name);
      if ( aerofoil_name != "") {
         break;
      }
   }

   // get the aerofoil coords
   std::vector<quan::two_d::vect<double> > coords;

   for(;;){
      if ( !f || f.eof() ) {
         break;
      }
      std::string line;
      getline(f,line);
      strip_leading(line);
      strip_trailing(line);
      if( line !=""){
         std::istringstream is(line);
         quan::two_d::vect<double> coord;

         is >> coord.x;
         // fix some selig files seem to have commas etc here
         is >> coord.y;
         coords.push_back(coord);
      }
   }
   if (coords.size() < 4){
      throw std::runtime_error("insufficient coords in aerofoil file");
      return false;
   }
   this->set_name(aerofoil_name.c_str());
   this->m_coords.clear();
   this->m_coords = coords;
   return true;
}