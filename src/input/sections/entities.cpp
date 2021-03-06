
#include <cstdlib>
#include <stack>
#include <algorithm>

#include <quan/two_d/out/vect.hpp>
#include <quan/three_d/out/vect.hpp>

#include <dxf/entity/line.hpp>
#include <dxf/entity/lwpolyline.hpp>
#include <dxf/entity/face3d.hpp>

#include "../lexer.hpp"
#include "../sections.hpp"

namespace {
   // should work for several entity types
   // entity/layer parsing
   bool get_layer(std::list<dxf::input::lexer::groupcode_pair> const & args, std::string & layer )
   {
      auto iter = std::find_if(args.begin(),args.end(),[](const auto & gp){ return gp.first == 8;});
      return (iter != args.end()) 
         ? layer = iter->second, true 
         : false;
   }

   // entity/colour parsing
   bool get_colour(std::list<dxf::input::lexer::groupcode_pair> const & args, int16_t & colour)
   {
      auto iter = std::find_if(args.begin(),args.end(),[](const auto & gp){ return gp.first == 62;});
      if(iter != args.end()) {
         int temp = 0;
         if (dxf::input::lexer::get_integer(iter->second,temp)){
            colour = static_cast<int16_t>(temp);
            return true;
         }else{
            dxf_bison_error("Line : invalid colour");
            return false;
         }
      }else{
         return false;
      }
   }

   //common entity attributes input parsing function
   bool add_entity_attributes(std::list<dxf::input::lexer::groupcode_pair> const & args, dxf::entity_t * entity)
   {
      std::string layer; get_layer(args,layer);

      int16_t colour = -1; get_colour(args,colour);

       if ( send_dxf_input_to_stdout() ){
          //  std::cout <<  "Line";
            if (layer != ""){
               std::cout << ", layer = " << layer;
            }
            if ( colour != -1){
               if ( layer != ""){
                  std::cout << ", ";
               }
               std::cout << "colour = " << colour ;
            }
            std::cout << ";\n";
        }

        if ( send_dxf_input_to_image() ){
             assert(entity != nullptr);

             if ( layer != ""){
                entity->set_layer_name(layer);
             }

             if( colour != -1){
               entity->set_colour_number(colour);
             }
        }
        return true;
   }

   // line input parsing function
   bool line(std::list<dxf::input::lexer::groupcode_pair> const & args )
   {
      dxf::line_t * line = nullptr;
      quan::three_d::vect<double> start, end;
      if ( dxf::input::lexer::get_vect3double(10,20,30,args,start) && dxf::input::lexer::get_vect3double(11,21,31,args,end) ){
         if ( send_dxf_input_to_stdout() ){
            std::cout <<  "Line [" << start << ", " << end << "]";
         }
         if ( send_dxf_input_to_image() ){
             auto file_image = get_dxf_file_image();
             line = new dxf::line_t(start,end);
             file_image->entities.add(line);
          }
          add_entity_attributes(args,line);          
          return true;
      }else{
         dxf_bison_error("Line : invalid args");
         return false;
      }
   }

   // lwpolyline input parsing function
   bool lwpolyline(std::list<dxf::input::lexer::groupcode_pair> const & args )
   {
      if ( send_dxf_input_to_stdout() ){
         std::cout << "lwpolyline\n";
      }

      dxf::lwpolyline_t * lwpolyline = nullptr;

      if ( send_dxf_input_to_image() ){
         auto file_image = get_dxf_file_image();
         lwpolyline = new dxf::lwpolyline_t{};
         file_image->entities.add(lwpolyline);
      }

      quan::two_d::vect<double> point; // point to read input to
      uint8_t point_data_complete_flags = 0; // bit[0] x, bit[1] y use to check point has all reqd data
      for ( auto const & gp : args){
         switch (gp.first){
            case 70: { // open/closed
                  int oc = -1;
                  if ( !dxf::input::lexer::get_integer(gp.second,oc)){
                     dxf_bison_error("lwpolyline : invalid open/closed");
                     return false;
                  }
                  if ( send_dxf_input_to_image() ){
                    lwpolyline->set_closed(oc != 0);
                  }
                  if ( send_dxf_input_to_stdout() ){
                     std::cout << ((oc !=0) ? "closed" : "open");
                     std::cout << " lwpolyline\n";
                  }
               }
               break;

            case 10: // // current/next point.x
               if ( (point_data_complete_flags & 0b01) != 0){
                  dxf_bison_error("lwpolyline : invalid point.x");
                  return false;
               }
               if (! dxf::input::lexer::get_double(gp.second,point.x) ){
                  dxf_bison_error("lwpolyline : expected point.x float");
                  return false;
               }
               point_data_complete_flags |= 0b01;
               break;
            
            case 20: // // current/ next point.y
               if ( (point_data_complete_flags & 0b10) != 0 ){
                  dxf_bison_error("lwpolyline : invalid point.y");
                  return false;
               }
               if (! dxf::input::lexer::get_double(gp.second,point.y) ){
                  dxf_bison_error("lwpolyline : expected point.y float");
                  return false;
               }
               point_data_complete_flags |= 0b10;
               break;

            default:
               break;
         }
         // look for completed points
         if ( point_data_complete_flags == 0b11){
            if ( send_dxf_input_to_stdout() ){
               std::cout << point <<'\n';
            }
            if ( send_dxf_input_to_image() ){
               lwpolyline->push_back(point);
            }
            point_data_complete_flags = 0b00;
         }
      }
      
      if (point_data_complete_flags != 0b00){
         dxf_bison_error("lwpolyline : invalid last point");
         return false;
      }
      add_entity_attributes(args,lwpolyline);
      return true;
   }

 /*
 3DFACE    Four points defining the corners of the face: (10, 20, 30),
            (11, 21, 31), (12, 22, 32), and (13, 23, 33).  70 (invisible
            edge flags -optional 0).  If only three points were entered
            (forming a triangular face), the third and fourth points will
            be the same.  The meanings of the bit-coded "invisible edge
            flags" are shown in the following table.

                      Flag bit value           Meaning
                             1        First edge is invisible
                             2        Second edge is invisible
                             4        Third edge is invisible
                             8        Fourth edge is invisible
*/

     // 3d face input parsing function
   //100 Subclass marker (AcDbFace)
   bool face3d(std::list<dxf::input::lexer::groupcode_pair> const & args )
   {

      dxf::face3d_t * face3d = nullptr;

      quan::three_d::vect<double> pts[4];
      uint16_t visibility_flags;

      // get all the points
      for ( uint32_t i = 0; i < 4; ++i){
         bool const result = dxf::input::lexer::get_vect3double(10 + i, 20 + i, 30 + i,args,pts[i]);
      
         if ( !result){
            if ( i == 3){
              pts[3] = pts[2];
            }else{
               dxf_bison_error("3d face : invalid point arguments");
               return false;
            }
         }
      }
      // get visibility flags
      auto iter = std::find_if(args.begin(),args.end(),[](const auto & gp){ return gp.first == 70;});
      if(iter != args.end()) {
         int temp = 0;
         if (dxf::input::lexer::get_integer(iter->second,temp)){
            visibility_flags = static_cast<uint16_t>(temp);
         }else{
            dxf_bison_error("face3d : invalid visibility flags");
            return false;
         }
      }else{
         dxf_bison_error("face3d : invalid arguments");
         return false;
      }
      if ( send_dxf_input_to_stdout() ){     
         std::cout << "3dFace [" << pts[0] << ", " << pts[1] << ", " << pts[2] << ", " << pts[3] << "]";
         std::cout << "visibility flags = " << visibility_flags <<'\n';
      }

      if ( send_dxf_input_to_image() ){
          auto file_image = get_dxf_file_image();
          face3d = new dxf::face3d_t(pts,visibility_flags);
          file_image->entities.add(face3d);
       }
       add_entity_attributes(args,face3d);
       return true;
     // find 
   }

   // function for parsing arg sets
   typedef bool (*entity_output_function_t)(std::list<dxf::input::lexer::groupcode_pair> const & args );

   //map entity id to a parse function
   struct entity_type_t{
      int id ;
      entity_output_function_t output_function;
   };
   
   typedef std::map<std::string,entity_type_t> entity_type_map_t;

   entity_type_map_t entity_type_map ={

      {"LINE",entity_type_t{LINE,line}}
     //3DLINE appears to be the same as LINE
      ,{"3DLINE",entity_type_t{LINE,line}}
      ,{"LWPOLYLINE",entity_type_t{LWPOLYLINE,lwpolyline}}
      ,{"3DFACE",entity_type_t{FACE3D,face3d}}
   };

   // entity level parser emit for entities
   bool output_entity(token & tok,std::stack<dxf::input::lexer::groupcode_pair>& pair_stack)
   {
      assert(pair_stack.size() > 1);

      // create a list for the entity args
      std::list<dxf::input::lexer::groupcode_pair> args;
      // and push args on it
      while (pair_stack.size() > 1){
          args.push_back(pair_stack.top());
          pair_stack.pop();
      }
      // last left on stack is variable name
      auto var_name = pair_stack.top();
      pair_stack.pop();
     
      // look up entity in map
      auto iter = entity_type_map.find(var_name.second);
      if ( iter != entity_type_map.end()){
          // and call output fun if found
          tok.set_id(iter->second.id);
          // need some sort of dxf::entity* to be returned from this?
          return iter->second.output_function(args);
          // call common on args for colour, layer
      }else{
          tok.set_id(UNKNOWNENTITY);
          if ( send_dxf_input_to_stdout() ){
             std::cout << " unknown entity \":" << var_name.second << "\"\n";
          }
         // ok unknown header entity is acceptable
         // just ignore it
         return true;
      }
   }

   int dxf_entities_func(std::string const & str,token & tok, std::ostream & out)
   {
      if ( str == "ENDSEC"){
         return end_section_func(str,tok, out);
      }else{
         std::stack<dxf::input::lexer::groupcode_pair> pair_stack;
         dxf::input::lexer::groupcode_pair g_pair{0,str};
         for (;;){
            pair_stack.push(g_pair);
            if (!dxf::input::lexer::peek_next_pair(g_pair)){
               return 0;
            }
            if ( g_pair.first == 0){
            // end of entity data
               if (! output_entity(tok,pair_stack)){
                  return 0;
               }
               return tok.get_id();
            }else{ 
               if (!dxf::input::lexer::get_next_pair(g_pair)){
                  return 0;
               }
            }
         }
      }
   }

   dxf::input::lexer::fun_map_t entities_fun_map = {
      {0,dxf::input::lexer::group_function_t{dxf_entities_func} }
   };

} // namespace

void init_entities_fun_map(section_type_map_t & section_type_map)
{
   section_type_map["ENTITIES"]= {ENTITIESSECTION,&entities_fun_map};
}
