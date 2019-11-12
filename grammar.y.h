/* A Bison parser, made by GNU Bison 3.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_GRAMMAR_Y_H_INCLUDED
# define YY_YY_GRAMMAR_Y_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 12 "grammar.y"

    #include <assert.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    #include <unistd.h>

    #ifndef NREADLINE
    #include <readline/history.h>
    #include <readline/readline.h>
    #endif

    #include "ast.h"
    #include "config.h"
    #include "parser.h"

    #define YYLTYPE parser_location_t

#line 69 "grammar.y.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    START_SINGLE = 258,
    START_FILE = 259,
    SEMICOLON = 260,
    LINE_END = 261,
    INPUT_END = 262,
    INDENT = 263,
    DEDENT = 264,
    INDENT_ERROR = 265,
    INDENT_OVERFLOW = 266,
    IF = 267,
    ELIF = 268,
    ELSE = 269,
    DO = 270,
    WHILE = 271,
    CONTINUE = 272,
    BREAK = 273,
    DEL = 274,
    NONE = 275,
    TRUE = 276,
    FALSE = 277,
    ASSIGN = 278,
    EQUALS = 279,
    LT = 280,
    GT = 281,
    LE = 282,
    GE = 283,
    OR = 284,
    AND = 285,
    NOT = 286,
    PLUS = 287,
    MINUS = 288,
    ASTERISK = 289,
    FSLASH = 290,
    PERCENT = 291,
    LPAREN = 292,
    RPAREN = 293,
    LBRACKET = 294,
    RBRACKET = 295,
    LBRACE = 296,
    RBRACE = 297,
    COMMA = 298,
    COLON = 299,
    INVALID_TOKEN = 300,
    STRING = 301,
    INTEGER = 302,
    FLOAT = 303,
    IDENT = 304
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 44 "grammar.y"

    Node *node_value;
    NodeList *node_list;

    struct {
        Node *first;
        Node *second;
    } node_pair;
    struct {
        NodeList *first;
        NodeList *second;
    } node_list_pair;

    const char *string_value;
    int64_t int_value;
    double float_value;

#line 148 "grammar.y.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct yypstate yypstate;

int yypush_parse (yypstate *ps, int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, void *scanner);

yypstate * yypstate_new (void);
void yypstate_delete (yypstate *ps);
/* "%code provides" blocks.  */
#line 33 "grammar.y"

    void yyerror(YYLTYPE *yylloc, void *scanner, const char* msg);

#line 188 "grammar.y.h"

#endif /* !YY_YY_GRAMMAR_Y_H_INCLUDED  */
