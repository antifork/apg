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


#ifndef GLOBAL_H
#define GLOBAL_H
#include <locale.h>
#include <config.h>
#include <std.h>
#include <tree.h>
#include <common.h>
#include <proto.h>

#ifdef 	GLOBAL_C
#define EXTERN
#define INIT(x...) =x
#else
#define EXTERN extern
#define INIT(x...)
#endif

EXTERN char apg_separator[8]    INIT("#=:'\\ \t\n");
EXTERN char apg_source[BUFFLEN] INIT("apg.par.c");
EXTERN char apg_header[BUFFLEN] INIT("apg.tab.h");
EXTERN char apg_if[BUFFLEN]     INIT("apg.fe.c");
EXTERN char filein[BUFFLEN];
EXTERN char apg_db[BUFFLEN];
EXTERN char apg_rules[BUFFLEN];
EXTERN char *ptr_db;

EXTERN ientry *index_db;
EXTERN FILE *yyin;

EXTERN long lineno INIT(1);
EXTERN long regno INIT(1);
EXTERN long opt_bitsfield;
EXTERN long type_bitsfield;

EXTERN t_node *ptr_node;
EXTERN t_node *root_node;
EXTERN t_regex *root_regex;

EXTERN char *sep_des[] INIT ({
        "comment",
        "line delimiter",
        "token delimiter",
        "string protection",
        "slash",
        "SPACE",
        "HT",
        "LF",
        NULL,});

#undef EXTERN
#undef INIT

#endif				/* GLOBAL_H */
