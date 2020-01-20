
#include <dxf/layer.hpp>
#include "../tables.hpp"

   int layer_table_function(std::string const & str,token & tok, std::ostream & out)
   {
      // on entry str is layer, tok is dont car
      dxf::input::lexer::groupcode_pair g_pair;
      for(;;){
         if (!dxf::input::lexer::peek_next_pair(g_pair)){
            return 0;
         }
         // look for end of table
         if (( g_pair.first == 0) && ( g_pair.second == "ENDTAB") ) {
             tok.set_id(LAYERTABLE);
             return tok.get_id();
         }else{
            if (!dxf::input::lexer::get_next_pair(g_pair)){
              return 0;
            }
         }
      
         switch (g_pair.first){
            case 2: // handle name
               if (send_dxf_input_to_stdout()){
                  std::cout << "layer_name = " << g_pair.second <<'\n';
               }
               if ( send_dxf_input_to_image() ){
                  auto file_image = get_dxf_file_image();
                  if (!file_image->layer_exists(g_pair.second) ){
                     if (send_dxf_input_to_stdout()){
                        std::cout << "adding layer " << g_pair.second <<'\n';
                     }
                     file_image->add_layer(new dxf::layer_t{g_pair.second});
                  }
               }
            break;
            default:
               if (send_dxf_input_to_stdout()){
                  std::cout << "unknown data pair{" << g_pair.first << ", " << g_pair.second << "\n";
               }
            break;
         }
      }
        
     // }
   }





