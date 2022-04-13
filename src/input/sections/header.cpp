

#include <stack>
#include <list>
#include <quan/three_d/out/vect.hpp>
#include "../lexer.hpp"
#include "../sections.hpp"
#include <dxf/header.hpp>

//HEADER section
/*
   initialise the header map functions
   and put in the header map in the sections map
*/

namespace {

   typedef bool (*header_variable_output_function)(std::list<dxf::input::lexer::groupcode_pair> const & args );
   typedef std::map<std::string,header_variable_output_function> header_variables_map_t;
/*
   AutoCAD drawing database version number:
      AC1006 = R10;
      AC1009 = R11 and R12;
      AC1012 = R13;
      AC1014 = R14;
      AC1015 = AutoCAD 2000;
      AC1018 = AutoCAD 2004;
      AC1021 = AutoCAD 2007;
      AC1024 = AutoCAD 2010
*/
   const std::map<std::string,dxf::acad_version_t> acad_version_map = {
      {"AC1006",dxf::acad_version_t::AC1006},
      {"AC1009",dxf::acad_version_t::AC1009},
      {"AC1012",dxf::acad_version_t::AC1012},
      {"AC1014",dxf::acad_version_t::AC1014},
      {"AC1015",dxf::acad_version_t::AC1015},
      {"AC1018",dxf::acad_version_t::AC1018},
      {"AC1021",dxf::acad_version_t::AC1021},
      {"AC1024",dxf::acad_version_t::AC1024}
   };
   //$ACADVER
   bool acadver(std::list<dxf::input::lexer::groupcode_pair> const & args )
   {
      // expect 1 arg
      if(args.size() !=1){
         dxf_bison_error("expected 1 argument to header variable");
         return false;
      }
      auto const & arg = args.front();
      if ( arg.first == 1){
         if ( send_dxf_input_to_stdout() ){
            std::cout << "DXF_file_version = " << arg.second << "\n";
         }
         if ( send_dxf_input_to_image() ){
            auto const iter = acad_version_map.find(arg.second) ;
            if ( iter != acad_version_map.end()){
               get_dxf_file_image()->header.set_acad_version(iter->second);
            }else{
              std::string erstr = "\"" + arg.second + "\" unknown dxf version";
              dxf_bison_error(erstr.c_str());
            }
         }
         return true;
      }else{
         dxf_bison_error("expected group_code:1 to header variable");
         return false;
      }
   }

   // $HANDSEED  next free handle
   bool handseed(std::list<dxf::input::lexer::groupcode_pair> const & args )
   {
      // expect 1 arg
      if(args.size() !=1){
         dxf_bison_error("expected 1 argument to header variable");
         return false;
      }
      auto const & arg = args.front();
      if ( arg.first == 5){
         if ( send_dxf_input_to_stdout() ){
         // TODO parsehex number
            std::cout << "Next Free Handle  = " << arg.second << "\n";
         }
         if( send_dxf_input_to_image()){
            uint64_t temp = 0;
            //
            if ( !dxf::input::lexer::get_handle(arg.second,temp) ){
               dxf_bison_error("failed to comvert $handseed to int");
               return false;
            }
            get_dxf_file_image()->header.set_handle_seed(temp);
         }
         return true;
      }else{
         dxf_bison_error("expected group_code:1 to header variable");
         return false;
      }
   }

   // $MEASUREMENT units 0 is english, 1 is metric
   bool measurement(std::list<dxf::input::lexer::groupcode_pair> const & args )
   {
      if(args.size() !=1){
         dxf_bison_error("expected 1 argument to header variable");
         return false;
      }
      auto const & arg = args.front();
      if ( arg.first == 70){
         int munits = -1;
         if ( dxf::input::lexer::get_integer(arg.second,munits) && ( (munits==1 || munits == 0))){
            if ( send_dxf_input_to_stdout() ){
               std::cout << "Measurement units = " << ((munits ==1)? "metric" : "imperial") << "\n";
            }
            return true;
         }else{
            dxf_bison_error("expected integer 1 or 0");
            return false;
         }
      }else{
         dxf_bison_error("expected group_code:70");
         return false;
      }
   }

   //$EXTMAX
   bool extmax(std::list<dxf::input::lexer::groupcode_pair> const & args )
   {
      quan::three_d::vect<double> vect;
      if ( dxf::input::lexer::get_vect3double(10,20,30,args,vect)){
         if ( send_dxf_input_to_stdout() ){
            std::cout << "extmax = " << vect <<'\n';
         }
         return true;
      }else{
         return false;
      }
   }

   //$EXTMIN
   bool extmin(std::list<dxf::input::lexer::groupcode_pair> const & args )
   {
      quan::three_d::vect<double> vect;
      if ( dxf::input::lexer::get_vect3double(10,20,30,args,vect)){
         if ( send_dxf_input_to_stdout() ){
            std::cout << "extmin = " << vect <<'\n';
         }
         return true;
      }else{
         dxf_bison_error("invalid args");
         return false;
      }
   }

   // $INSBASE
   bool insbase(std::list<dxf::input::lexer::groupcode_pair> const & args )
   {
      quan::three_d::vect<double> vect;
      if ( dxf::input::lexer::get_vect3double(10,20,30,args,vect)){
         if ( send_dxf_input_to_stdout() ){
            std::cout << "insbase = " << vect <<'\n';
         }
         return true;
      }else{
         dxf_bison_error("invalid args");
         return false;
      }
   }

   // Add header variables and their parse functions as needed here
   header_variables_map_t header_variables_map = {
       {"$ACADVER",acadver} ,
       {"$HANDSEED",handseed} ,
       {"$MEASUREMENT",measurement}, //
       {"$EXTMIN",extmin},
       {"$EXTMAX",extmax},
       {"$INSBASE",insbase}
   };

}

namespace {

   bool output_header_variable(std::stack<dxf::input::lexer::groupcode_pair>& pair_stack){

      assert(pair_stack.size() > 1);

      // create a list for variable args
      std::list<dxf::input::lexer::groupcode_pair> args;
      // and push args on it
      while (pair_stack.size() > 1){
          args.push_back(pair_stack.top());
          pair_stack.pop();
      }
      // last left on stack is variable name
      auto var_name = pair_stack.top();
      pair_stack.pop();

      // look up variable name in map
      auto iter = header_variables_map.find(var_name.second);
      if ( iter != header_variables_map.end()){
          // and call output fun if found
          return iter->second(args);
      }else{
         if ( send_dxf_input_to_stdout() ){
            std::cout << " unknown header variable \":" << var_name.second <<'\n';
         }
         // ok unknown header var is acceptable
         // just ignore it
         return true;
      }

   }

   // groupcode:9 str is Variable name
   // extra data is dependent on variable name
   int dxf_header_variable_func(std::string const & str,token & tok, std::ostream & out)
   {
      dxf::input::lexer::groupcode_pair g_pair{9,str};

      // push var and args as they arrive
      std::stack<dxf::input::lexer::groupcode_pair> pair_stack;

      for (;;){
         pair_stack.push(g_pair);
         if (!dxf::input::lexer::peek_next_pair(g_pair)){
            return 0;
         }

         if ( ( g_pair.first == 0 ) || (g_pair.first == 9)){
            // peeked start of next var or endsec
            if ( ! output_header_variable(pair_stack)){
               return 0;
            }
            if (g_pair.first == 0 ){
               break;
            }
         }
         if (!dxf::input::lexer::get_next_pair(g_pair)){
            return 0;
         }
      }
      // return tok for all header data
      tok.set_id(HEADERDATA);

      return tok.get_id();
   }

   dxf::input::lexer::fun_map_t header_fun_map = {
     {9,dxf::input::lexer::group_function_t{dxf_header_variable_func}},
     {0,dxf::input::lexer::group_function_t{end_section_func}}
   };

}  //namespace

void init_header_fun_map(section_type_map_t & section_type_map)
{
   section_type_map["HEADER"]= {HEADERSECTION,&header_fun_map};
}
