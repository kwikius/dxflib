/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_DXF_BISON_DXF_BISON_HPP_INCLUDED
# define YY_DXF_BISON_DXF_BISON_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int dxf_bison_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    HEADERSECTION = 258,
    HEADERDATA = 259,
    CLASSESSECTION = 260,
    TABLESSECTION = 261,
    BLOCKSSECTION = 262,
    ENTITIESSECTION = 263,
    OBJECTSSECTION = 264,
    THUMBNAILIMAGESECTION = 265,
    UNKNOWNSECTION = 266,
    ENDSEC = 267,
    COMMENT = 268,
    UNKNOWNOBJECT = 269,
    BLOCK = 270,
    ENDBLOCK = 271,
    TABLE = 272,
    ENDTAB = 273,
    LTYPETABLE = 274,
    LAYERTABLE = 275,
    UNKNOWNTABLE = 276,
    UNKNOWNENTITY = 277,
    LINE = 278,
    LWPOLYLINE = 279,
    DATAPAIR = 280,
    EOF_DXF_FILE = 281
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE dxf_bison_lval;

int dxf_bison_parse (void);

#endif /* !YY_DXF_BISON_DXF_BISON_HPP_INCLUDED  */
