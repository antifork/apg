#line 2 "/usr/local/share/apg/apg.db"
/* $Id$ */
#line 4 "/usr/local/share/apg/apg.db"
/* 
 *  $Id$
 *  apg.tab.h
 *  Generated automatically by apg: autoparser generator.
 *
 *  Sources are available at 	
 *		ftp://ftp.antifork.org/apg/ 
 *		cvs -d :pserver:anoncvs@cvs.antifork.org:/CVS checkout apg
 *	
 *  Copyright (c) 1999 Bonelli Nicola  <bonelli@antifork.org>
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

#ifndef _APG_TAB_H
#define _APG_TAB_H

#ifndef _APG_PARSER_C
extern int apg_errno;
#endif

#define APG_MAXERR      3

#define APG_EOK         0
#define APG_EEMPTY	1
#define APG_ENULL       2
#define APG_EEOG        3

/* Use prototypes unless the compiler is old-fashioned.  */
#include <sys/types.h>
#include <sys/cdefs.h>

#if !defined (__P)
# ifdef __STDC__
# define __P(x) x
# else
# define __P(x) ()
# endif
#endif

#if defined(__ANSI__) || defined (__STRICT_ANSI__)
#define u_char  unsigned char
#define u_short unsigned short
#define u_int   unsigned int
#define u_long  unsigned long
#endif

/* functions declarations */

#if defined(__cplusplus) || defined(__ANSI__) || defined (__STRICT_ANSI__) 

/* g++ / ANSI */

#define AAINT_ARG       int
#define AAUINT_ARG	u_int
#define AASIZE_ARG      size_t 
#define AACHAR_ARG      char *
#define AAVOID_ARG      void *
#define AAGRILL_ARG     grill_t *
#define AAGRILL__ARG	grill_t **

#define AAINT_DECL(a)
#define AAUINT_DECL(a)   
#define AASIZE_DECL(a)    
#define AACHAR_DECL(a)
#define AAVOID_DECL(a)
#define AAGRILL_DECL(a)
#define AAGRILL__DECL(a)

#else
/* GNU c */

#define AAINT_ARG
#define AAUINT_ARG       
#define AASIZE_ARG       
#define AACHAR_ARG    
#define AAVOID_ARG      
#define AAGRILL_ARG     
#define AAGRILL__ARG 

#define AAINT_DECL(a)   int a;
#define AAUINT_DECL(a)   u_int a; 
#define AASIZE_DECL(a)   size_t a; 
#define AACHAR_DECL(a)  char *a;
#define AAVOID_DECL(a)   void *a;
#define AAGRILL_DECL(a)  grill_t *a;
#define AAGRILL__DECL(a) grill_t **a;

#endif


#define c_rule_label	1
#define h_rule_label	2
#define i_rule_label	3


typedef struct __type_line__ {
  int	type_line;
  union {
     struct {
	int chap;
	int parag;
	u_short bit_t;
	u_short bit_o;
	char *comm;
        } c_rule_line;
     struct {
	int chap;
	int parag;
	u_short bit_t;
	u_short bit_o;
	char *comm;
        } h_rule_line;
     struct {
	int chap;
	int parag;
	u_short bit_t;
	u_short bit_o;
	char *comm;
        } i_rule_line;
    } line_dun;
  struct __type_line__	*next;
} grill_t;



#define c_rule_chap		line_dun.c_rule_line.chap
#define c_rule_parag		line_dun.c_rule_line.parag
#define c_rule_bit_t		line_dun.c_rule_line.bit_t
#define c_rule_bit_o		line_dun.c_rule_line.bit_o
#define c_rule_comm		line_dun.c_rule_line.comm
#define h_rule_chap		line_dun.h_rule_line.chap
#define h_rule_parag		line_dun.h_rule_line.parag
#define h_rule_bit_t		line_dun.h_rule_line.bit_t
#define h_rule_bit_o		line_dun.h_rule_line.bit_o
#define h_rule_comm		line_dun.h_rule_line.comm
#define i_rule_chap		line_dun.i_rule_line.chap
#define i_rule_parag		line_dun.i_rule_line.parag
#define i_rule_bit_t		line_dun.i_rule_line.bit_t
#define i_rule_bit_o		line_dun.i_rule_line.bit_o
#define i_rule_comm		line_dun.i_rule_line.comm

/* prototypes */

#line 108 "/usr/local/share/apg/apg.db"
/* We don't care about __THROW gcc 2.8.x optimizations */
#ifdef   __THROW
#undef   __THROW
#define  __THROW
#endif

char 	*apg_strerror __P ((int));
grill_t *apg_parser  __P ((int, ...));
int 	apg_get_line __P ((grill_t **));
void 	apg_free_grill __P ((grill_t *));
void 	apg_free_pragma __P ((void));

/* parser header side */

#line 123 "/usr/local/share/apg/apg.db"
#ifdef _APG_PARSER_C
/* header */

#line 125 "/usr/local/share/apg/apg.db"
#include <stdlib.h>

#ifndef _STDIO_H
#include <stdio.h>
#endif

#include <limits.h>
#include <sys/types.h>
#include <sys/cdefs.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <string.h>
#include <errno.h>
#include <ctype.h>

/* apg types */

#line 152 "/usr/local/share/apg/apg.db"
#ifndef _HAVE_GRAMMAR_TAB_H
#define T_INT   	0x01
#define T_SHORT 	0x02
#define T_CHAR 		0x03
#define T_U_32 		0x04
#define T_U_16 		0x05
#define T_U_8  		0x06
#define T_STR  		0x07
#define T_HOST  	0x08
#define T_IPV4 		0x09
#endif

/* apg type size */

#line 165 "/usr/local/share/apg/apg.db"
static const int types_size[] =
  { 0, sizeof (int), sizeof (short), sizeof (char),
  sizeof (long), sizeof (short), sizeof (char),
  sizeof (void *), sizeof (void *), sizeof (void *)
};

/* apg type id */

#line 172 "/usr/local/share/apg/apg.db"
static const char *const types_id[] =
  { NULL, "INT", "SHORT", "CHAR", "U_32", "U_16", "U_8", "STR", "HOST", "IPV4"
};

/* apg table */

#define APG_MAXARG 6
static const int apgtb[][6][5] = {
{ {         0,          0,          0,          0,          0},
{         0,          0,          0,          0,          0},
{         0,          0,          0,          0,          0},
{         0,          0,          0,          0,          0},
{         0,          0,          0,          0,          0},
{         0,          0,          0,          0,          0},
},
{{         0,          0,          0,          0,          0},
{         1, -2147483647, 2147483647,          1,          0},
{         1, -2147483647, 2147483647,          1,          0},
{         5,          0,      65535,          1,          0},
{         5,          0,      65535,          1,          0},
{         7,          0,          0,          0,          0},
},
{{         0,          0,          0,          0,          0},
{         1, -2147483647, 2147483647,          1,          0},
{         1, -2147483647, 2147483647,          1,          0},
{         5,          0,      65535,          1,          0},
{         5,          0,      65535,          1,          0},
{         7,          0,          0,          0,          0},
},
{{         0,          0,          0,          0,          0},
{         1, -2147483647, 2147483647,          1,          0},
{         1, -2147483647, 2147483647,          1,          0},
{         5,          0,      65535,          1,          0},
{         5,          0,      65535,          1,          0},
{         7,          0,          0,          0,          0},
},
};


/* apg offset */

static int apg_offset[][APG_MAXARG]={
{ -1, -1, -1, -1, -1, -1, },
{ -1, 0, 4, 8, 10, 12, }, 
{ -1, 0, 4, 8, 10, 12, }, 
{ -1, 0, 4, 8, 10, 12, }, 
};

/* typedef: seg_t and line_t */

#line 177 "/usr/local/share/apg/apg.db"
typedef struct
{
  u_long hash;
  char *id;
}
line_t;

typedef struct i_seg_t
{
  char *ptr;
  struct i_seg_t *next;
}
seg_t;

/* line_t list */

static const line_t line_v[] = { { 0, NULL }, { 1240268419U, "c_rule" }, { 3061006684U, "h_rule" }, { 4226064595U, "i_rule" }, };

#define QMAX_ELEM	3

/* custom delimiters */

#define APG_SEPLINE  '='
#define APG_SEPTOKEN ':'

#line 192 "/usr/local/share/apg/apg.db"
#endif /* _APG_PARSER_C */
#endif /* _APG_TAB_H */

