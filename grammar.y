%{
/* 
 *  $Id$
 *  %% apg: grammar rules. 
 *
 *  Copyright (c) 1999 Bonelli Nicola <bonelli@antifork.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <common.h>
#include <tree.h>
#include <limits.h>
#include <string.h>

#include <regex.h>
#include <proto.h>


extern t_node *ptr_node, *root_node;

extern char *yytext;
extern int lineno, regno;
extern char filein[];

extern long type_bitsfield;
extern long opt_bitsfield;

int opt;

#define set_tbf(x) ( type_bitsfield |=x )
#define set_opt(x) (  opt_bitsfield |=x )

#define GETLSTC(x)   x[strlen(x)-1]
#define SETLSTC(x,y) x[strlen(x)-1]=y

%}

%union
{
  char *string;
  char *regex;
  long int number;
  unsigned long u_number;

}

%token EOL T_INT T_SHORT T_CHAR
%token T_U_32 T_U_16 T_U_8
%token T_STR T_HOST T_IPV4

%token <string>         T_STRING
%token <regex>          T_REGEX
%token <number>         T_NUMBER
%token <u_number>       T_U_NUMBER

%left ':'

%%

confile :
        | confline
        ;

confline: confline lines
        | confline EOL
        | lines
        | EOL
        ;

lines   : label '=' args EOL
        ;

label:T_STRING '(' T_NUMBER ',' T_NUMBER ')'
{
  if ($3 > $5)
    fatalerr ("%s:%d: tag (%s)[%d,%d], bogus bound; [minor<major] failure",
	      filein, lineno, $1, $3, $5);
  set_opt (OPT_REPLABEL);
  ptr_node = create_node (ptr_node, $1, $3, $5);
  check_uinode (root_node, $1);
}
|T_STRING '(' T_NUMBER ',' ')'
{
  set_opt (OPT_REPLABEL);
  ptr_node = create_node (ptr_node, $1, $3, -1);
  check_uinode (root_node, $1);
}
|T_STRING '(' ',' T_NUMBER ')'
{
  set_opt (OPT_REPLABEL);
  ptr_node = create_node (ptr_node, $1, -1, $4);
  check_uinode (root_node, $1);
}
|T_STRING '(' T_NUMBER ')'
{
  set_opt (OPT_REPLABEL);
  ptr_node = create_node (ptr_node, $1, $3, $3);
  check_uinode (root_node, $1);
}
|T_STRING
{
  ptr_node = create_node (ptr_node, $1, -1, -1);
  check_uinode (root_node, $1);
}
;

args:args ':' args | '(' T_STRING ')' T_INT
{
  set_tbf (TBF_INT);
  check_required ($2);
  create_leaf (ptr_node, $2, T_INT, INT_MIN +1 , INT_MAX , 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_INT '[' T_NUMBER ',' T_NUMBER ']'
{
  set_tbf (TBF_INT);
  check_required ($2);
  check_bound (T_INT, $2, $6, $8);
  create_leaf (ptr_node, $2, T_INT, $6, $8, 0, opt);
  check_uileaf (ptr_node, $2);
}

	/*** Overflow in int class. ***/

|'(' T_STRING ')' T_INT '[' T_U_NUMBER ',' T_NUMBER ']'
{
  fatalerr
    ("%s:%d: tag (%s)[%u,%d] overflow in int rappresentation. The largest range is [%d,%d]",
     filein, lineno, $2, $6, $8, INT_MIN +1, INT_MAX );
}

|'(' T_STRING ')' T_INT '[' T_NUMBER ',' T_U_NUMBER ']'
{
  fatalerr
    ("%s:%d: tag (%s)[%d,%u] overflow in int rappresentation. The largest range is [%d,%d]",
     filein, lineno, $2, $6, $8, INT_MIN +1, INT_MAX );

}
|'(' T_STRING ')' T_INT '[' T_U_NUMBER ',' T_U_NUMBER ']'
{
  fatalerr
    ("%s:%d: tag (%s)[%u,%u] overflow in int rappresentation. The largest range is [%d,%d]",
     filein, lineno, $2, $6, $8, INT_MIN +1, INT_MAX );
}

       /***/


|'(' T_STRING ')' T_SHORT
{
  set_tbf (TBF_SHORT);
  check_required ($2);
  create_leaf (ptr_node, $2, T_SHORT, SHRT_MIN, SHRT_MAX, 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_SHORT '[' T_NUMBER ',' T_NUMBER ']'
{
  set_tbf (TBF_SHORT);
  check_required ($2);
  check_bound (T_SHORT, $2, $6, $8);
  create_leaf (ptr_node, $2, T_SHORT, $6, $8, 0, opt);
  check_uileaf (ptr_node, $2);
}

	/*** Overflow in short class. ***/

|'(' T_STRING ')' T_SHORT '[' T_U_NUMBER ',' T_NUMBER ']'
{
  fatalerr
    ("%s:%d: tag (%s)[%u,%d] overflow in short rappresentation. The largest range is [%d,%d]",
     filein, lineno, $2, $6, $8, SHRT_MIN, SHRT_MAX);
}

|'(' T_STRING ')' T_SHORT '[' T_NUMBER ',' T_U_NUMBER ']'
{
  fatalerr
    ("%s:%d: tag (%s)[%d,%u] overflow in short rappresentation. The largest range is [%d,%d]",
     filein, lineno, $2, $6, $8, SHRT_MIN, SHRT_MAX);
}
|'(' T_STRING ')' T_SHORT '[' T_U_NUMBER ',' T_U_NUMBER ']'
{
  fatalerr
    ("%s:%d: tag (%s)[%u,%u] overflow in short rappresentation. The largest range is [%d,%d]",
     filein, lineno, $2, $6, $8, SHRT_MIN, SHRT_MAX);
}


/*** u_32 class ***/

|'(' T_STRING ')' T_U_32
{
  set_tbf (TBF_U_32);
  check_required ($2);
  create_leaf (ptr_node, $2, T_U_32, 0, UINT_MAX , 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_U_32 '[' T_NUMBER ',' T_NUMBER ']'
{
  set_tbf (TBF_U_32);
  check_required ($2);
  check_bound (T_U_32, $2, $6, $8);
  create_leaf (ptr_node, $2, T_U_32, $6, $8, 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_U_32 '[' T_U_NUMBER ',' T_NUMBER ']'
{
  set_tbf (TBF_U_32);
  check_required ($2);
  check_bound (T_U_32, $2, $6, $8);
  create_leaf (ptr_node, $2, T_U_32, $6, $8, 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_U_32 '[' T_NUMBER ',' T_U_NUMBER ']'
{
  set_tbf (TBF_U_32);
  check_required ($2);
  check_bound (T_U_32, $2, $6, $8);
  create_leaf (ptr_node, $2, T_U_32, $6, $8, 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_U_32 '[' T_U_NUMBER ',' T_U_NUMBER ']'
{
  set_tbf (TBF_U_32);
  check_required ($2);
  check_bound (T_U_32, $2, $6, $8);
  create_leaf (ptr_node, $2, T_U_32, $6, $8, 0, opt);
  check_uileaf (ptr_node, $2);
}

/*** u_16 class ***/


|'(' T_STRING ')' T_U_16
{
  set_tbf (TBF_U_16);
  check_required ($2);
  create_leaf (ptr_node, $2, T_U_16, 0, USHRT_MAX, 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_U_16 '[' T_NUMBER ',' T_NUMBER ']'
{
  set_tbf (TBF_U_16);
  check_required ($2);
  check_bound (T_U_16, $2, $6, $8);
  create_leaf (ptr_node, $2, T_U_16, $6, $8, 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_U_16 '[' T_U_NUMBER ',' T_NUMBER ']'
{
  set_tbf (TBF_U_16);
  check_required ($2);
  check_bound (T_U_16, $2, $6, $8);
  create_leaf (ptr_node, $2, T_U_16, $6, $8, 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_U_16 '[' T_NUMBER ',' T_U_NUMBER ']'
{
  set_tbf (TBF_U_16);
  check_required ($2);
  check_bound (T_U_16, $2, $6, $8);
  create_leaf (ptr_node, $2, T_U_16, $6, $8, 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_U_16 '[' T_U_NUMBER ',' T_U_NUMBER ']'
{
  set_tbf (TBF_U_16);
  check_required ($2);
  check_bound (T_U_16, $2, $6, $8);
  create_leaf (ptr_node, $2, T_U_16, $6, $8, 0, opt);
  check_uileaf (ptr_node, $2);
}

	/*** u_8 class ***/

|'(' T_STRING ')' T_U_8
{
  set_tbf (TBF_U_8);
  check_required ($2);
  create_leaf (ptr_node, $2, T_U_8, 0, 255, 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_CHAR
{
  set_tbf (TBF_CHAR);
  check_required ($2);
  create_leaf (ptr_node, $2, T_CHAR, -127, +127, 0, opt);
  check_uileaf (ptr_node, $2);
}

/** string class ***/


|'(' T_STRING ')' T_STR
{
  set_tbf (TBF_STR);
  check_required ($2);
  create_leaf (ptr_node, $2, T_STR, 0, 0, 0, opt);
  check_uileaf (ptr_node, $2);
}


|'(' T_STRING ')' T_STR T_REGEX
{
  int i = 0;
  char err_msg[80] = "";
  regex_t *p;

  check_required ($2);

  set_tbf (TBF_STR);
  set_tbf (TBF_REGEX);

  p = (regex_t *) malloc (sizeof (regex_t));

  if ((i = regcomp (p, $5, REG_EXTENDED)))
    {
      regerror (i, p, err_msg, 30);
      fprintf (stderr,
	       "%s:%d: tag (%s) regex `%s': %s; syntax error; exit forced.\n",
	       filein, lineno, $2, $5, err_msg);
      exit (1);
    }
  regfree (p);
  add_regex ($5);
  create_leaf (ptr_node, $2, T_STR, 0, 0, regno++, opt);
  check_uileaf (ptr_node, $2);
}


|'(' T_STRING ')' T_STR '[' T_NUMBER ',' T_NUMBER ']'
{
  set_tbf (TBF_STR);
  check_required ($2);
  create_leaf (ptr_node, $2, T_STR, $6, $8, 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_HOST
{
  set_tbf (TBF_HOST);
  check_required ($2);
  create_leaf (ptr_node, $2, T_HOST, 0, 255, 0, opt);
  check_uileaf (ptr_node, $2);
}

|'(' T_STRING ')' T_IPV4
{
  set_tbf (TBF_IPV4);
  check_required ($2);
  create_leaf (ptr_node, $2, T_IPV4, 0, 16, 0, opt);
  check_uileaf (ptr_node, $2);
}

;

%%yyerror (s)
     char * s;
{
  fatalerr ("%s:%d: %s", filein, lineno, s);
  return;
}

void
check_bound (int type, char *label, int minor, int major)
{

  /* order */

  if (major <= minor)
    fatalerr ("%s:%d: tag (%s)[%d,%d], bogus bound; [minor<major] failure",
	      filein, lineno, label, minor, major);

  /* unsigned limit */

  if (((type == T_U_32) || (type == T_U_16)) && ((minor < 0) || (major < 0)))
    fatalerr
      ("%s:%d: tag (%s)[%d,%d], bound err; unsigned limits are required",
       filein, lineno, label, minor, major);

  /* internal rappresentation */

  switch (type)
    {
    case T_SHORT:
      if ((minor < SHRT_MIN) || (minor > SHRT_MAX) || (major < SHRT_MIN)
	  || (major > SHRT_MAX))
	fatalerr
	  ("%s:%d: tag (%s)[%d,%d] overflow in short rappresentation. The largest range is [%d,%d]",
	   filein, lineno, label, minor, major, SHRT_MIN, SHRT_MAX);
      break;
    case T_U_16:
      if ((minor > SHRT_MAX) || (major > SHRT_MAX))
	fatalerr
	  ("%s:%d: tag (%s)[%d,%d] overflow in short rappresentation. The largest range is [%d,%d]",
	   filein, lineno, label, minor, major, 0, SHRT_MAX);
      break;

      break;

    }
  return;
}

void
check_required (char *p)
{
  
 opt=0;

  if (GETLSTC (p) == '$')
     { 
	SETLSTC (p, 0);
	opt|=0x01; /* no-optional token */

     }

}
