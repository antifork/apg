/* 
 *  $Id$
 *  %% apg. 
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

#ifndef APG_COMMON_H
#define APG_COMMON_H

typedef struct _index_entry_
        {
        int chapter;
        int paragraph;
        int offset;
	int line;
        struct _index_entry_ *next;
        } ientry;

#define DB_LOAD		0x00001
#define DB_DISCARD	0x00002

/* option bitsfield */

#define OPT_FDIN        0x0001
#define OPT_FDOUT       0x0002
#define OPT_ADVAN	0x0004
#define OPT_ADVHELP	0x0008
#define OPT_DB		0x0010
#define OPT_RULES	0x0020
#define OPT_REPLABEL	0x0040
#define OPT_IF		0x0080

/* type bitsfiled */


#define TBF_INT		0x0001
#define TBF_U_32	0x0002
#define TBF_SHORT	0x0004
#define TBF_U_16	0x0008
#define TBF_CHAR	0x0010
#define TBF_U_8		0x0020
#define TBF_STR		0x0040
#define TBF_HOST	0x0080
#define TBF_IPV4	0x0100
#define TBF_REGEX	0x8000


#define add_cr(x) 			(fprintf(x,"\n"))
#define add_comment(fd,x)		(fprintf(fd,"/* %s */\n",x))
#define add_define (fd,x,y)	        (fprintf(fd,"#define %s %s\n",x,y))
#define add_include(fd,x)		(fprintf(fd,"#include \"%s\"\n",x))
#define add_linclude(fd,x)		(fprintf(fd,"#line 1 \"%s\"\n#include \"%s\"\n",apg_source,x))

#define create_table(fd,name,a,b)       (fprintf(fd,"static const int %s[][%d][%d] = {\n",name,a,b))
#define close_table(x)			(fprintf(x,"};\n\n"))


#endif /* APG_COMMON_H */
