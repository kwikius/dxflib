#ifndef DXF_INPUT_SECTIONS_HPP_INCLUDED
#define DXF_INPUT_SECTIONS_HPP_INCLUDED

#include <string>
#include <iosfwd>

#include "lexer.hpp"

int comment_func(std::string const & str,token & tok, std::ostream & out);
int end_section_func(std::string const & str,token & tok, std::ostream & out);

struct section_type_t{
   int id;
   dxf::input::lexer::fun_map_t * fun_map;
};

typedef std::map<std::string, section_type_t> section_type_map_t;


#endif // DXF_INPUT_SECTIONS_HPP_INCLUDED
