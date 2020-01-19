
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <quan/conversion/double_convert.hpp>

#include "lexer.hpp"

dxf::input::lexer::groupcode_pair dxf::input::lexer::m_input_pair;
dxf::input::lexer::groupcode_pair dxf::input::lexer::m_lookahead_pair;
bool dxf::input::lexer::m_have_lookahead_pair = false;
std::istream * dxf::input::lexer::m_inputstream = nullptr;
quan::lexer::filepos dxf::input::lexer::m_filepos;
dxf::input::lexer::fun_map_t* dxf::input::lexer::m_cur_fun_map = nullptr;

/*
   locally parse the dxf file into groupcode value pairs
*/
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
} // namespace
 // groupcode can be negative
bool dxf::input::lexer::get_integer(std::string const & str_in, int & out)
{
   std::string str = str_in;
   if (( str[0] == '+') || ( str[0] == '-')){
      str = str.substr(1,std::string::npos);
   }
   if ( str.length() == 0){
      return false;
   }
   if ( str.length() > 6){
      std::cout << "group id too long\n";
      return false;
   }
   for ( auto c: str){
      if (! isdigit(c)){
         return false;
      }
   }
   out = std::atoi(str_in.c_str());
   return true;
}

bool dxf::input::lexer::get_double(std::string const & str_in, double & out)
{
   quan::detail::converter<double,char*> conv;
   //std::cout << "instr length = " << str_in.length() << "\n";
   double res = conv (str_in.c_str(),str_in.length()+1);
   if ( conv.get_errno() == 0){
      out = res;
      return true;
   }  else{
      dxf_bison_error("string to double conversion failed");
      return false;
   }
}

bool dxf::input::lexer::ll_getline(std::string & str)
{
  if (! getline(*m_inputstream,str)){
//      dxf_bison_error("dxf input file failed");
      return false;
   }
   m_filepos.position.row++;
   strip_leading(str);
   strip_trailing(str);
   return true;
}

bool dxf::input::lexer::get_next_pair(dxf::input::lexer::groupcode_pair & out)
{
   if ( m_have_lookahead_pair){
      m_input_pair = m_lookahead_pair;
      m_have_lookahead_pair = false;
   }else{
      assert(m_inputstream != nullptr);
      assert(m_inputstream->good());
      std::string str;
      if ( ! ll_getline(str)){
         return false;
      }  
      if (! dxf::input::lexer::get_integer(str,m_input_pair.first)){
         dxf_bison_error("groupcode not integer");
         return false;
      }
      if ( ! ll_getline(str)){
         return false;
      }  
      m_input_pair.second = str;
   }
   out = m_input_pair;
   return true;
}

bool dxf::input::lexer::peek_next_pair(dxf::input::lexer::groupcode_pair & out)
{
   if ( ! m_have_lookahead_pair){
      assert(m_inputstream != nullptr);
      assert(m_inputstream->good());
      std::string str;
      if ( ! dxf::input::lexer::ll_getline(str)){
         return false;
      }  
      if (! dxf::input::lexer::get_integer(str,m_lookahead_pair.first)){
         dxf_bison_error("groupcode not integer");
         return false;
      }
      if ( ! dxf::input::lexer::ll_getline(str)){
         return false;
      }  
      m_lookahead_pair.second = str;
      m_have_lookahead_pair = true; 
   }
   out = m_lookahead_pair;
   return true;
}

namespace {
   // alias this token to bison token lval
   token the_token;
}  // ~namespace

// 
void init_sections_map();
// ret < 0 on fail else 0
int dxf::input::lexer::init(const char * filename)
{
   m_inputstream = new std::ifstream(filename);
   if (! m_inputstream){
     dxf_bison_error("couldnt open dxf input file or memory error");
     return -1;
   }
   if (!m_inputstream->good()){
      dxf_bison_error("couldnt open dxf input file");
      return -1;
   }

   dxf_bison_lval = &the_token;

   m_filepos.filename = filename;
   m_filepos.position.row = 0;
   m_filepos.position.column = 0;

   init_sections_map();

   return 0;
}

int dxf_bison_lex()
{
   return dxf::input::lexer::lex();
}

// return lex value
// 0 is end of input 
int dxf::input::lexer::lex()
{
   dxf::input::lexer::groupcode_pair g_pair;
   if (dxf::input::lexer::get_next_pair(g_pair)){
     // dxf_bison_lval = new token(dxf::input::lexer::get_file_pos());
      dxf_bison_lval->clear_attribute();
      dxf_bison_lval->set_filepos(dxf::input::lexer::get_filepos());
      auto iter = m_cur_fun_map->find(g_pair.first);
      if ( iter != m_cur_fun_map->end()){
         //std::cout << "parse1\n";
         auto fun = iter->second.parse_function;
         return fun(g_pair.second,*dxf_bison_lval,std::cout);
      }else{
         dxf_bison_error("unknown input");
         return 0;
      }
   }else{
      return 0;
   }
}
