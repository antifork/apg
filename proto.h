/*
 *  $Id$ 
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


/* prototypes */

#ifndef APG_PROTO_H
#define APG_PROTO_H 

#include <std.h>
#include <tree.h>

t_node  *create_node    P((t_node *, char *, int, int));
t_leaf  *create_leaf    P((t_node *, char *, int, int, int, int, int));

void    check_uinode    P((t_node *, char *));
void    check_uileaf    P((t_node *, char *));
void    print_tree      P((t_node *));
void 	create_index    P((char *));
void 	tab_par_creator P((void));
void 	fatalerr 	P((char *, ...));
void 	add_regex 	P((char *));

void    check_bound     P((int, char *, int, int));
void    check_required  P((char *));
int 	extract_segment P((char *, FILE *, int, int, char *, int));

char 	*touch_db  	P((int ));


void    *xmalloc        P((unsigned int));
void    *xrealloc       P((char *,unsigned int));
void    *xcalloc        P((unsigned int, unsigned int));

void 	usage      	P((char *));

#endif  /* APG_PROTO_H */
