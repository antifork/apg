/* $Id$ */
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


#define i_rule_label	1
#define c_rule_label	2
#define h_rule_label	3


typedef struct __type_line__ {
  int	type_line;
  union {
     struct {
	int chap;
	int parag;
	unsigned short bit_t;
	unsigned short bit_o;
	char *comm;
        } i_rule_line;
     struct {
	int chap;
	int parag;
	unsigned short bit_t;
	unsigned short bit_o;
	char *comm;
        } c_rule_line;
     struct {
	int chap;
	int parag;
	unsigned short bit_t;
	unsigned short bit_o;
	char *comm;
        } h_rule_line;
    } line_dun;
  struct __type_line__	*next;
} grill_t;



#define i_rule_chap		line_dun.i_rule_line.chap
#define i_rule_parag		line_dun.i_rule_line.parag
#define i_rule_bit_t		line_dun.i_rule_line.bit_t
#define i_rule_bit_o		line_dun.i_rule_line.bit_o
#define i_rule_comm		line_dun.i_rule_line.comm
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

/* prototypes */

/* Use prototypes unless the compiler is old-fashioned.  */

#ifdef __STDC__
#define P(x) x
#else
#define P(x) ()
#endif

char *apg_strerror P ((int));
grill_t *apg_parser P ((int, ...));
int apg_get_line P ((grill_t **));
void apg_free_grill P ((grill_t *));
void apg_free_pragma P ((void));

/* parser header side */

#ifdef _APG_PARSER_C
/* includes */

#include <stdlib.h>

#ifndef _STDIO_H
#include <stdio.h>
#endif

#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>

#include <string.h>
#include <errno.h>
#include <ctype.h>

/* apg types */

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

static const int types_size[] =
  { 0, 	sizeof (int), 	sizeof (short), sizeof (char),
  	sizeof (long), 	sizeof (short), sizeof (char),
  	sizeof (void *),sizeof (void *),sizeof (void *)
};

/* apg type id */

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

typedef struct
{
  int hash;
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

static const line_t line_v[] = { { 0, NULL }, { 253405355U, "i_rule" }, { 2910986343U, "c_rule" }, { 4276732252U, "h_rule" }, };

#define QMAX_ELEM	3

/* custom delimiters */

#define APG_SEPLINE  '='
#define APG_SEPTOKEN ':'

#endif /* _APG_PARSER_C */
#endif /* _APG_TAB_H */

