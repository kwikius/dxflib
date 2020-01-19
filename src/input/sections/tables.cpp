

#include "../lexer.hpp"
#include "../sections.hpp"

namespace {

   struct table_type_t{
      int id;
      dxf::input::lexer::fun_map_t * fun_map;
   };
   typedef std::map<std::string, table_type_t> table_type_map_t;

   // map table type to functions e.g LTYPE LAYER etc
   table_type_map_t table_type_map;
}

// map for types of table
void init_table_type_map()
{
/*
   init_appid_map();
   init_block_record_map();
   init_dimstyle_map();
   init_layer_map();
   init_ltype_map();
   init_style_map();
   init_ucs_map();
   init_view_map();
   init_vport_map();
*/
}

namespace {



   // 0
   int dxf_tables_func(std::string const & str,token & tok, std::ostream & out)
   {
      // 0 TABLE
      if ( str == "TABLE"){

         dxf::input::lexer::groupcode_pair g_pair;
         if (!dxf::input::lexer::get_next_pair(g_pair)){
            return 0;
         }
         if ( g_pair.first != 2){
            dxf_bison_error("unexpected group code afer TABLE");
            return 0;
         }
         std::cout << "In table : " << g_pair.second << " {\n";
         auto iter = table_type_map.find(g_pair.second);
         if ( iter != table_type_map.end()){
            table_type_t const & tt = iter->second;
            tok.set_id(tt.id);
            dxf::input::lexer::set_fun_map(tt.fun_map);
         }else{
            // unknown table type
            tok.set_id(UNKNOWNTABLE);
            // remove data till end of table
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
}

void init_tables_fun_map(section_type_map_t & section_type_map)
{
  //  tables_fun_map[0] = dxf_lexer::group_function_t{dxf_tables_func};
    section_type_map["TABLES"]= {TABLESSECTION,&tables_fun_map};
    init_table_type_map();
}