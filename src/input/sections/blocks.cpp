

#include "../lexer.hpp"
#include "../sections.hpp"

namespace {

   // 0
   int dxf_blocks_func(std::string const & str,token & tok, std::ostream & out)
   {
      if ( str == "BLOCK"){

         tok.set_id(BLOCK);

         // TODO // parse data in block
         // for now just look for ENDBLOCK
         if ( send_dxf_input_to_stdout() ){
            std::cout << "block\n";
         }
         dxf::input::lexer::groupcode_pair g_pair;
         for(;;){
            
            if (!dxf::input::lexer::peek_next_pair(g_pair)){
               return 0;
            }
            if ( g_pair.first == 0){
               // block should be followed by endblk
               if (g_pair.second == "ENDBLK"){
                  break;
               }else{
                  // if not badly formed dxf file
                  if ( (g_pair.second == "BLOCK") || (g_pair.second == "ENDSEC") ){
                      dxf_bison_error("expected ENDBLK");
                      return 0;
                  }
               }
            }else{
               if (!dxf::input::lexer::get_next_pair(g_pair)){
                  return 0;
               }
               // parse the block data
               std::cout << "block pair{" << g_pair.first << "," << g_pair.second << "}\n";
            }
          }
      }else{
         if (str == "ENDBLK"){

            tok.set_id(ENDBLOCK);
            if ( send_dxf_input_to_stdout() ){
               std::cout << "end block\n";
            }
            // TODO parse endblock data
            // for now just look for endsec
            
            dxf::input::lexer::groupcode_pair g_pair;
            for(;;){
               if (!dxf::input::lexer::peek_next_pair(g_pair)){
                  return 0;
               }
               if ( g_pair.first == 0){
                  if ( (g_pair.second == "BLOCK") || (g_pair.second == "ENDSEC") ){
                     break;
                  }else {
                     dxf_bison_error("Unexpected group pair 0 in ENDBLK");
                     return 0;
                  }
               }else{
                  if (!dxf::input::lexer::get_next_pair(g_pair)){
                     return 0;
                  }
                  if ( send_dxf_input_to_stdout() ){
                     std::cout << "end block pair{" << g_pair.first << "," << g_pair.second << "}\n";
                  }
               }
            }
         }else{
            if (str == "ENDSEC"){
               //pop out of the BLOCKS section
               return end_section_func(str,tok, out);
            }else{
                // error
                dxf_bison_error("Unknown ID after 0 in blocks Sec TODO : investigate");
                return 0;
            }
         }
      }
      return tok.get_id();
   }

   dxf::input::lexer::fun_map_t blocks_fun_map = {
      {0,dxf::input::lexer::group_function_t{dxf_blocks_func}}
   };

}  //namespace

void init_blocks_fun_map(section_type_map_t & section_type_map)
{
   section_type_map["BLOCKS"]= {BLOCKSSECTION,&blocks_fun_map};
}
