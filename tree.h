/*
 *  $Id$
 *  Tree headers
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

#ifndef APG_TREE_H
#define APG_TREE_H

typedef struct s_leaf {
	int type, regex, opt;
	int minor, major;
	char *label;
	struct s_leaf *next_leaf;
}      t_leaf;


typedef struct s_node {
	char *label;
	int min, max;
	struct s_leaf *next_leaf;
	struct s_node *next_node;

}      t_node;

typedef struct s_regex {
	char *regex;
	struct s_regex *next;

}       t_regex;


#endif				/* APG_TREE_H */
