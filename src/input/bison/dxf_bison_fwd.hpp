#ifndef DXF_INPUT_BISON_FWD_HPP_INCLUDED
#define DXF_INPUT_BISON_FWD_HPP_INCLUDED

#include <dxf/input/token.hpp>

typedef  token* token_ptr;

#define YYSTYPE token_ptr

int dxf_parser_init(const char* filename);

int dxf_bison_lex();
int dxf_bison_error(const char* error);

#endif // DXF_INPUT_BISON_FWD_HPP_INCLUDED
