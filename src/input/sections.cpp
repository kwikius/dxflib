
#include "lexer.hpp"
#include "sections.hpp"

void init_header_fun_map(section_type_map_t & sections_type_map);
void init_blocks_fun_map(section_type_map_t & section_type_map);
void init_tables_fun_map(section_type_map_t & section_type_map);
void init_entities_fun_map(section_type_map_t & section_type_map);

namespace {

   int sections_func(std::string const & str,token & tok, std::ostream & out);

   dxf::input::lexer::fun_map_t sections_fun_map = {
     {0, dxf::input::lexer::group_function_t{sections_func}},
     { 999, dxf::input::lexer::group_function_t{comment_func} },
   };

}

int comment_func(std::string const & str,token & tok, std::ostream & out)
{
   std::string * str1 = new std::string{str};
  // tok.clear_attribute();
   if ( send_dxf_input_to_stdout() ){
      std::cout << "Comment {\"" << str << "\"}\n";
   }
   tok.set_ptr(str1,COMMENT);
   return tok.get_id();
};

// This is common to all sections
int end_section_func(std::string const & str,token & tok, std::ostream & out)
{
  if ( str != "ENDSEC"){
       dxf_bison_error("expected ENDSEC");
       return 0;
  }
  if ( send_dxf_input_to_stdout() ){
     std::cout << "ENDSEC }\n\n";
  }
  tok.set_id(ENDSEC);
  dxf::input::lexer::set_fun_map(&sections_fun_map);
  return tok.get_id();
}

namespace {

   section_type_map_t section_type_map;

   dxf::input::lexer::fun_map_t classes_fun_map;  

   dxf::input::lexer::fun_map_t objects_fun_map;
   dxf::input::lexer::fun_map_t thumbnailimage_fun_map;

}

void init_section_type_map()
{
   init_header_fun_map(section_type_map);
   init_blocks_fun_map(section_type_map);
   init_tables_fun_map(section_type_map);
   init_entities_fun_map(section_type_map);
   
// TODO for now ignore blocks clases etc
 //  section_type_map["CLASSES"]= {CLASSESSECTION,&classes_fun_map};
  // section_type_map["OBJECTS"]= {OBJECTSSECTION,&objects_fun_map};
  // section_type_map["THUMBNAILIMAGE"] = {THUMBNAILIMAGESECTION,&thumbnailimage_fun_map};
   
}

namespace {

   int sections_func(std::string const & str,token & tok, std::ostream & out)
   {
      if ( str == "SECTION" ){

         dxf::input::lexer::groupcode_pair g_pair;
         if (!dxf::input::lexer::get_next_pair(g_pair)){
            return 0;
         }
         // group code should be 2
         if ( g_pair.first != 2){
            dxf_bison_error("unexpected group-code after SECTION");
            return 0;
         }
         if ( send_dxf_input_to_stdout() ){
            std::cout << "In section : " << g_pair.second << " {\n";
         }
         auto iter = section_type_map.find(g_pair.second);
         if ( iter != section_type_map.end()){
            section_type_t const & st = iter->second;
            tok.set_id(st.id);
            dxf::input::lexer::set_fun_map(st.fun_map);
         }else{
            if ( send_dxf_input_to_stdout() ){
               std::cout << "unknown section type\n";
            }
            tok.set_id(UNKNOWNSECTION);
            for (;;){
               if (!dxf::input::lexer::peek_next_pair(g_pair)){
                  return 0;
               }
               // look for end sec
               if (( g_pair.first == 0) && ( g_pair.second == "ENDSEC")) {
                  if ( send_dxf_input_to_stdout() ){
                     std::cout << "end of unknown section part 1\n";
                  }
                  return tok.get_id();
               }else{
                  if (!dxf::input::lexer::get_next_pair(g_pair)){
                     return 0;
                  }
               }
            }
         }
      }else{
         if ( str == "EOF"){
            if ( send_dxf_input_to_stdout() ){
               std::cout << "got EOF\n";
            }
            tok.set_id(EOF_DXF_FILE);
         }else{
            // get here on unknown section
            // could check here that we are in an unknown section
            if ( str == "ENDSEC"){
               if ( send_dxf_input_to_stdout() ){
                  std::cout << "end of unknown section part 2\n";
               }
               tok.set_id(ENDSEC);
               
            }else {
               tok.set_id(UNKNOWNOBJECT);
               dxf_bison_error("Unknown Object");
            }
         }
      }
      return tok.get_id();
   }
}// namespace

/*
  initialise the map and init the lexer map to it since it is at global level
*/
void init_sections_map()
{

   dxf::input::lexer::set_fun_map(&sections_fun_map);

   // initialise all the sections in the section_type_map
   init_section_type_map();

}
