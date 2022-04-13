#ifndef DXF_INPUT_LEXER_HPP_INCLUDED
#define DXF_INPUT_LEXER_HPP_INCLUDED

#include <string>
#include <utility>
#include <iostream>
#include <map>
#include <list>

#include <quan/three_d/vect.hpp>

#include "bison.hpp"

namespace dxf { namespace input {

   struct lexer{

      typedef std::pair<int,std::string> groupcode_pair;

      typedef int (* map_function_t)(std::string const & str,token & tok, std::ostream & out);

      struct group_function_t{
         map_function_t parse_function;
      };

      typedef std::map<int, group_function_t> fun_map_t;

      static int init(const char * filename);
      static bool get_next_pair(groupcode_pair & out);
      static bool peek_next_pair(groupcode_pair & out);
      static bool ll_getline(std::string & str);

      static int lex();

      static quan::lexer::filepos const & get_filepos()
      {
         return m_filepos;
      }

      static void set_fun_map(fun_map_t* fmap) { m_cur_fun_map = fmap;}
      static bool get_handle(std::string const & str_in, uint64_t & handle);
      static bool get_integer(std::string const & str_in, int & out);
      static bool get_double(std::string const & str_in, double & out);
      static bool get_vect3double(int xid, int yid, int zid,std::list<dxf::input::lexer::groupcode_pair> const & args, quan::three_d::vect<double> & vect_out);

    private:
      static groupcode_pair m_input_pair;
      static groupcode_pair m_lookahead_pair;
      static bool m_have_lookahead_pair;
      static std::istream * m_inputstream;
      static quan::lexer::filepos m_filepos;
      static fun_map_t* m_cur_fun_map; // current map functions

   };

}}

#endif // DXF_INPUT_LEXER_HPP_INCLUDED
