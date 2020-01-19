
#include "dxf_bison_fwd.hpp"
#include "../lexer.hpp"

// Note The dxf_bison_lval is defined in the bison generated source file
extern token_ptr dxf_bison_lval ;

int dxf_parser_init(const char* filename)
{
  int const ret = dxf::input::lexer::init(filename);
  if ( ret < 0 ){
    return ret;
  }
  return 0;
}

namespace {
   int num_errors = 0;
}

int dxf_bison_error(const char* error)
{
  ++num_errors;
  std::string str1 = error;
  if ( dxf_bison_lval != nullptr){
   
   std::cerr << "in " << dxf_bison_lval->fp.to_string() << " : error : " << str1 << '\n';

  }else{
      std::cerr << error <<'\n';
  }
  return 0;
 }

