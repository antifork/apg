/*
 *  $Id$
 *  tree- 
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
#include <config.h>
#include <std.h>
#include <tree.h>
#include <grammar.tab.h>
#include <proto.h>


/*
 * global defs 
 */

t_node *root_node, ptr_node;
t_leaf *ptr_leaf;
t_regex *root_regex;

extern int lineno;
extern char filein[];

void
check_uinode (t_node * f_root_node, char *f_label)
{
  if (f_root_node)
    while (f_root_node->next_node)
      {
	if (!strcmp (f_root_node->label, f_label))
	  fatalerr ("%s:%d: node identifier {%s} must be unique", filein,
		    lineno, f_label);

	f_root_node = f_root_node->next_node;

      }
  return;
}

void
check_uileaf (t_node * f_node, char *f_label)
{
  register t_leaf *ptr_leaf = f_node->next_leaf;

  if (ptr_leaf)
    while (ptr_leaf->next_leaf)
      {
	if (!strcmp (ptr_leaf->label, f_label))
	  fatalerr ("%s:%d: leaf-identifier {%s} must be unique", filein,
		    lineno - 1, f_label);

	ptr_leaf = ptr_leaf->next_leaf;

      }
  return;
}


void
add_regex (char *r)
{
  t_regex *p_ptr = root_regex, *new_ptr;

  new_ptr = (t_regex *) xmalloc (sizeof (t_regex));
  new_ptr->regex = (char *) xmalloc (sizeof (r) + 1);

  strcpy (new_ptr->regex, r);

  new_ptr->next = NULL;

  for (; p_ptr != NULL && p_ptr->next != NULL; p_ptr = p_ptr->next);

  if (p_ptr)
    p_ptr->next = new_ptr;
  else
    root_regex = new_ptr;

}



t_node *
create_node (t_node * ptr, char *f_label, int min, int max)
{
  register t_node *lp = ptr;

  if (lp)
    {
      while (lp->next_node)
	lp = lp->next_node;

      lp->next_node = (t_node *) xmalloc (sizeof (t_node));
      lp = lp->next_node;

      lp->label = f_label;
      lp->min = min;
      lp->max = max;
      lp->next_node = NULL;
      lp->next_leaf = NULL;

    }
  else
    {

      lp = root_node = (t_node *) xmalloc (sizeof (t_node));

      root_node->label = f_label;
      root_node->min = min;
      root_node->max = max;
      root_node->next_node = NULL;
      root_node->next_leaf = NULL;
    }

  return lp;
}

t_leaf *
create_leaf (t_node * ptr, char *f_label, int f_type, int f_minor,
	     int f_major, int f_regex, int f_opt)
{
  register t_node *lp_node = ptr;
  register t_leaf *lp_leaf = NULL;

  if (lp_node == NULL)
    fatalerr ("internalerr: ptr_node is a NULL pointer");

  lp_leaf = lp_node->next_leaf;

  if (lp_leaf)
    {
      while (lp_leaf->next_leaf != NULL)
	lp_leaf = lp_leaf->next_leaf;

      lp_leaf->next_leaf = (t_leaf *) xmalloc (sizeof (t_leaf));

      lp_leaf = lp_leaf->next_leaf;

    }
  else
    lp_node->next_leaf = lp_leaf = (t_leaf *) xmalloc (sizeof (t_leaf));

  lp_leaf->type = f_type;
  lp_leaf->minor = f_minor;
  lp_leaf->major = f_major;
  lp_leaf->label = f_label;
  lp_leaf->regex = f_regex;
  lp_leaf->opt = f_opt;
  lp_leaf->next_leaf = NULL;

  return lp_leaf;
}


void
isort_tree (void)
{
  register t_node *p, *q, *r;
  int flag = 1;

  p = q = root_node->next_node;

  /* we dont' need to sort in case of 0 or 1 node */

  if (p == NULL || p->next_node == NULL)
    return;

  while (flag)
    {
      flag = 0;

      q = root_node->next_node;

      for (p = q; q && q->next_node; q = q->next_node)
	{

	  /* check */

	  if ( (unsigned)apg_one_at_a_time_hash (q->label) > (unsigned)apg_one_at_a_time_hash (q->next_node->label))
	    {
	      flag = 1;

	      if (q == root_node->next_node)
		{
		  /* first couple */
		  root_node->next_node = p->next_node;
		  p->next_node = q->next_node->next_node;
		  p = root_node->next_node;
		  p->next_node = q;

		}
	      else
		{
		  p->next_node = q->next_node;
		  q->next_node = q->next_node->next_node;
		  p->next_node->next_node = q;
		  q = p->next_node;
		}

	    }

	  p = q;
	}

    }

  return;
}


void
print_tree (t_node * f_root)
{
  register t_node *node_ptr = f_root;
  register t_leaf *leaf_ptr = NULL;

  while (node_ptr)
    {
      fprintf (stderr, "node: {%s} rep (%d,%d)\n", node_ptr->label,
	       node_ptr->min, node_ptr->max);

      if (node_ptr->next_leaf)
	{

	  /*
	   * this branch has some leafs 
	   */

	  leaf_ptr = node_ptr->next_leaf;

	  while (leaf_ptr)
	    {
	      fprintf (stderr, "  `--- {%s} %d\t[%d,%d] (%d)\n",
		       leaf_ptr->label,
		       leaf_ptr->type, leaf_ptr->minor, leaf_ptr->major,
		       leaf_ptr->regex);
	      leaf_ptr = leaf_ptr->next_leaf;
	    }
	}
      node_ptr = node_ptr->next_node;
    }

  return;
}
