

#include "../lexer.hpp"
#include "../sections.hpp"

#include "tables.hpp"


namespace {

   dxf::input::lexer::map_function_t  current_table_function = nullptr;

}

void clear_current_tables_function()
{
   current_table_function = nullptr;
}


namespace {

   std::map<std::string,dxf::input::lexer::map_function_t> tables_function_map{
      {"LAYER",layer_table_function }
   };

   // 0
   int dxf_tables_func(std::string const & str,token & tok, std::ostream & out)
   {
      // 0 TABLE / ENDTAB/ENDSEC
      if ( str == "TABLE"){
         dxf::input::lexer::groupcode_pair g_pair;
         if (!dxf::input::lexer::get_next_pair(g_pair)){
            return 0;
         }
         if ( g_pair.first != 2){
            dxf_bison_error("unexpected group code after TABLE");
            return 0;
         }
         auto iter = tables_function_map.find(g_pair.second);
         if ( iter != tables_function_map.end()){
            if (send_dxf_input_to_stdout()){
               std::cout << "in table : " << g_pair.second <<'\n';
            }
            return iter->second(str,tok,out);
         }else{
            // unknown table type
            tok.set_id(UNKNOWNTABLE);
            // remove data till end of table
            if (send_dxf_input_to_stdout()){
               std::cout << "in unknown table : " << g_pair.second <<'\n';
            }
            for (;;){
               if (!dxf::input::lexer::peek_next_pair(g_pair)){
                     return 0;
               }
               // look for end of table
               if (( g_pair.first == 0) && ( g_pair.second == "ENDTAB")) {
                  return tok.get_id();
               }else{
                  if (!dxf::input::lexer::get_next_pair(g_pair)){
                     return 0;
                  }
               }
            }
         }
      }else{
         // 0 ENDTAB
         if ( str == "ENDTAB"){
            tok.set_id(ENDTAB);
            std::cout << "}\n";
            return tok.get_id();
         }else{
            if ( str == "ENDSEC"){
               return end_section_func(str,tok,out);
            }else{
              dxf_bison_error("Expected TABLE, ENDTABLE or ENDSEC");
            }
            return 0;
         }
      }
      return tok.get_id();
   }

   dxf::input::lexer::fun_map_t tables_fun_map = {
     {0,dxf::input::lexer::group_function_t{dxf_tables_func}}
   };

}//namespace

void init_tables_fun_map(section_type_map_t & section_type_map)
{
  //  tables_fun_map[0] = dxf_lexer::group_function_t{dxf_tables_func};
    section_type_map["TABLES"]= {TABLESSECTION,&tables_fun_map};
  //  init_table_type_map();
}