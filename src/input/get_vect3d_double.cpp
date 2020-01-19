#include "lexer.hpp"

/*
 note that not all ids need to be present, then that part is inited to 0
  
*/
bool dxf::input::lexer::get_vect3double(int xid, int yid, int zid,std::list<dxf::input::lexer::groupcode_pair> const & args, quan::three_d::vect<double> & vect_out)
{
   quan::three_d::vect<double> vect;
   int arg_flags = 0;
   bool error = false;
   for ( auto arg : args){

      if ( ( arg.first == xid) || (arg.first == yid) || (arg.first == zid) ){
         double val;
         if (!dxf::input::lexer::get_double(arg.second,val)){
            dxf_bison_error("invalid double value");
            return 0;
         }
         if ( arg.first == xid){
            if (( arg_flags & 0b001) == 0){
               arg_flags |= 0b001;
               vect.x = val;
            }else{
               error = true;
            }
         }else {
            if(arg.first == yid){
               if (( arg_flags & 0b010) == 0){
                  arg_flags |= 0b010;
                  vect.y = val;
               }else{
                  error = true;
               }
            }else {
               if (arg.first == zid){
                  if (( arg_flags & 0b100) == 0){
                     arg_flags |= 0b100;
                     vect.z = val;
                  }else{
                     error = true;
                  }
               }
            }
         }
      }
   }
   if ( (error == false)){
      vect_out = vect;
      return true;
   }

   dxf_bison_error("invalid double vect 1");
   return false;
}
