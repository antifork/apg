/* 
 *  $Id$
 *  engine.c 
 *
 *  Copyright (c) 1999,2000 Bonelli Nicola <bonelli@antifork.org>
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

#include <limits.h>
#include <locale.h>
#include <config.h>
#include <std.h>
#include <tree.h>
#include <db.h>
#include <common.h>
#include <proto.h>

/* note: don't change the order of the follow includes 
*/

#include "grammar.tab.h"
#define _HAVE_GRAMMAR_TAB_H
#include "tab.engine.h"

extern char *ptr_db, apg_source[], apg_header[], apg_if[],apg_rules[];
extern t_node *ptr_node, *root_node;
extern t_regex *root_regex;

extern char apg_separator[];

extern long type_bitsfield;
extern long opt_bitsfield;


static t_node *lp_node;
static t_leaf *lp_leaf;
static int node_c, leaf_c, max_leaf;


#define c_comm(fd, C) ( ( C && *C ) ? (add_comment(fd,C)) : ({}) )

/* function */

/* apg line labels */
void
fun_1 (FILE * fd, char *comm)
{
  lp_node = root_node->next_node;
  node_c = 1;

  c_comm (fd, comm);
  add_cr (fd);

  while (lp_node)
    {
      fprintf (fd, "#define %s_label\t%d\n", lp_node->label, node_c);
      node_c++;
      lp_node = lp_node->next_node;
    }
  add_cr (fd);
}

/* apg type_def grill_t */
void
fun_2 (FILE * fd, char *comm)
{
  lp_node = root_node->next_node;
  lp_leaf = NULL;

  node_c = 1;
  leaf_c = 1;

  c_comm (fd, comm);
  add_cr (fd);

  fprintf (fd, "#include <sys/types.h>\n\n"); 
  fprintf (fd, "typedef struct __type_line__ {\n");
  fprintf (fd, "  int\ttype_line;\n");
  fprintf (fd, "  union {\n");

  while (lp_node)
    {

      if (lp_node->next_leaf)
	{

	  fprintf (fd, "     struct {\n");
	  lp_leaf = lp_node->next_leaf;

	  while (lp_leaf)
	    {

	      switch (lp_leaf->type)
		{
		case T_INT:
		  fprintf (fd, "\tint %s;\n", lp_leaf->label);
		  break;
		case T_SHORT:
		  fprintf (fd, "\tshort %s;\n", lp_leaf->label);
		  break;
		case T_CHAR:
		  fprintf (fd, "\tchar %s;\n", lp_leaf->label);
		  break;
		case T_U_32:
		  fprintf (fd, "\tu_int %s;\n", lp_leaf->label);
		  break;
		case T_U_16:
		  fprintf (fd, "\tu_short %s;\n",
			   lp_leaf->label);
		  break;
		case T_U_8:
		  fprintf (fd, "\tu_char %s;\n",
			   lp_leaf->label);
		  break;
		default:
		  fprintf (fd, "\tchar *%s;\n", lp_leaf->label);
		  break;

		}
	      lp_leaf = lp_leaf->next_leaf;
	    }

	  fprintf (fd, "        } %s_line;\n", lp_node->label);
	}
      lp_node = lp_node->next_node;
      node_c *= 2;
    }
  fprintf (fd,
	   "    } line_dun;\n  struct __type_line__\t*next;\n} grill_t;\n\n");

  add_cr (fd);

}

/* symb link */
void
fun_3 (FILE * fd, char *comm)
{
  lp_node = root_node->next_node;
  lp_leaf = NULL;
  leaf_c = 1;

  c_comm (fd, comm);
  add_cr (fd);

  while (lp_node)
    {
      if (lp_node->next_leaf)
	{
	  lp_leaf = lp_node->next_leaf;

	  while (lp_leaf)
	    {
	      fprintf (fd, "#define %s_%s\t\tline_dun.%s_line.%s\n",
		       lp_node->label, lp_leaf->label,
		       lp_node->label, lp_leaf->label);
	      lp_leaf = lp_leaf->next_leaf;
	      leaf_c++;
	    }
	}
      lp_node = lp_node->next_node;
    }

  add_cr (fd);
}

#define MAX(a,b) ( a < b ? b :a )

/* apg table */
void
fun_4 (FILE * fd, char *comm)
{
  lp_node = root_node->next_node;
  lp_leaf = NULL;

  node_c = 0;
  leaf_c = 0;

  while (lp_node)
	{
	node_c++;
	lp_leaf= lp_node->next_leaf;
	while(lp_leaf)
		{
		leaf_c++;
		lp_leaf=lp_leaf->next_leaf;
		}
        max_leaf=MAX(max_leaf,leaf_c);
	leaf_c=0;

	lp_node=lp_node->next_node;
	}

  c_comm (fd, comm);
  add_cr (fd);

  fprintf(fd,"#define APG_MAXARG %d\n",max_leaf+1);
  create_table (fd, "apgtb", max_leaf+1,5);

  fprintf(fd,"{ ");
  for (leaf_c=0;leaf_c <= max_leaf;leaf_c++)
	fprintf(fd,"{         0,          0,          0,          0,          0},\n");
  fprintf(fd,"},\n");

  /* reset */

  lp_node = root_node->next_node;
  lp_leaf = NULL;
  node_c = 0;
  leaf_c = 0;

  while (lp_node)
    {
      node_c++;
      lp_leaf = lp_node->next_leaf;
	  fprintf(fd,"{{         0,          0,          0,          0,          0},\n");

	  while (lp_leaf)
	    {
	      fprintf (fd, "{%10d, %10d, %10d, %10d, %10d},\n",
		       lp_leaf->type - T_INT + 1,
		       lp_leaf->minor, lp_leaf->major,
                       lp_leaf->opt,
		       lp_leaf->regex);

              leaf_c++;
	      lp_leaf = lp_leaf->next_leaf;
              
	    }

          for (;leaf_c < max_leaf; leaf_c++)
      		fprintf(fd,"{         0,          0,          0,          0,          0},\n");

	  fprintf(fd,"},\n");

      lp_node = lp_node->next_node;
      leaf_c = 0;
    }

  close_table (fd);
  add_cr (fd);
}

static const int apg_types_size[] =
  { 0, sizeof(int), sizeof(short), sizeof(char),
  sizeof(long),sizeof(short), sizeof(char), sizeof(void *), sizeof(void *),
  sizeof(void *)  
};

/* apg table offset */

void
fun_5 (FILE * fd, char *comm)
{    
  register int offset=0, i;

  c_comm (fd, comm);
  add_cr (fd);

  lp_node = root_node->next_node;
  lp_leaf = NULL;


  fprintf(fd, "static int apg_offset[][APG_MAXARG]={\n");

  fprintf(fd,"{ ");
  for (leaf_c=0;leaf_c <= max_leaf; leaf_c++)
	fprintf(fd,"-1, ");	
        fprintf(fd,"},\n");
  leaf_c=0;

  while (lp_node)
        {
	offset=0;
	leaf_c=0;
      
        lp_leaf= lp_node->next_leaf;
	fprintf(fd,"{ -1, ");

        while(lp_leaf)
                {
		if (offset % apg_types_size[lp_leaf->type - T_INT + 1] )
		offset += ( apg_types_size[lp_leaf->type - T_INT + 1] - (offset % apg_types_size[lp_leaf->type - T_INT + 1])); /* magic padding*/

		fprintf(fd,"%d, ",offset);
		offset += apg_types_size[lp_leaf->type - T_INT + 1];
		leaf_c++;
                lp_leaf=lp_leaf->next_leaf;
                }
	for (i=leaf_c; i < max_leaf; i++) fprintf(fd,"-1, ");
	fprintf(fd, "}, \n");
        lp_node=lp_node->next_node;
        }

  fprintf(fd, "};\n");
  add_cr(fd);
}

/* apg line_list */

static int collision_vector[256];

void
chk_collision (char *p,int n)
{
  static int top = 0;
  register int i;

  for (i = 0; i < top; i++)
    {
      if (n == collision_vector[i])
          fatalerr ("fatalerr: the '%s' id comes into collision with a previous line. Since this happens with a reasonably NULL probability, please mail <bonelli@antifork.org> informing that it happened. Change the current line_id.",p);

    }
  collision_vector[top++] = n;
}

void
fun_6 (FILE * fd, char *comm)
{
  int j=0;
  lp_node = root_node->next_node;
  
  c_comm (fd, comm);
  add_cr (fd);

  fprintf (fd, "static const line_t line_v[] = { { 0, NULL }, ");

  while (lp_node)
    {
      fprintf (fd, "{ %uU, \"%s\" }, ", apg_one_at_a_time_hash(lp_node->label) ,lp_node->label);
      lp_node = lp_node->next_node;
      j++;
    }
  fprintf (fd, "};\n\n");

  fprintf (fd, "#define QMAX_ELEM\t%d\n",j);
  add_cr (fd);
}

/* apg regex_list */
void
fun_7 (FILE * fd, char *comm)
{
  t_regex *lp = (t_regex *) root_regex;

  c_comm (fd, comm);
  add_cr (fd);

  fprintf (fd, "static const char * const regex_list[] = { NULL, ");

  while (lp)
    {
      fprintf (fd, "\"%s\", ", lp->regex);
      lp = lp->next;
    }
  fprintf (fd, "\n};\n\n");

}

/* rep limits */
void
fun_8 (FILE * fd, char *comm)
{
  register int c = 0;
  lp_node = root_node->next_node;

  c_comm (fd, comm);
  add_cr (fd);

  fprintf (fd, "static long rep_limits[][2] = { {0,0},\n");

  while (lp_node)
    {
      fprintf (fd, "{ %5d, %5d },\n", lp_node->min,
	       lp_node->max);
      c++;
      lp_node = lp_node->next_node;
    }

  fprintf (fd, "};\n");

  add_cr (fd);

  {
    register int i = 0;

    fprintf (fd, "static long int rep_counter[] ={\n");

    for (; i < c; i++)
      fprintf (fd, " 0,");
    fprintf (fd, " 0 };\n");

  }
  add_cr (fd);
}

/* custom delimiters */

void
fun_9 (FILE *fd, char *comm)
{

  c_comm (fd, comm);
  add_cr (fd);

  fprintf(fd,"#define APG_SEPLINE  \'%c\'\n",apg_separator[1]);
  fprintf(fd,"#define APG_SEPTOKEN \'%c\'\n",apg_separator[2]);

  add_cr (fd);

}

/* classic if */

void
fun_10 (FILE *fd, char *comm)
{
  c_comm (fd, comm);
  add_cr (fd);
  add_linclude (fd, apg_header);
  add_cr (fd);
}

void
fun_11 (FILE * fd, char *comm)
{
  lp_node = root_node->next_node;

  while (lp_node)
    {
      fprintf (fd, "      case %s_label:\n", lp_node->label);
      fprintf (fd, "      break;\n");
      lp_node = lp_node->next_node;
    }
  add_cr (fd);
}

/* vector funs pointer */
static void (*fun_list[]) (FILE *, char *) =
{
NULL, fun_1, fun_2, fun_3, fun_4, fun_5, fun_6, fun_7,fun_8, fun_9, fun_10, fun_11};


void
tab_par_creator (void)
{

  struct stat lstat;
  FILE *fd_c, *fd_h, *fd_i;

  fprintf (stderr, "Creating %s, %s ", apg_source, apg_header);

  if (opt_bitsfield & OPT_IF)
	fprintf (stderr, "%s\n", apg_if);
  else
	fprintf (stderr, "\n");
  
  /* open apg.tab.h && apg.par.c */

  if (stat (apg_header, &lstat) != -1)
    fatalerr ("fatalerr: %s (tab.h) already exists", apg_header);

  if ((fd_h = fopen (apg_header, "w")) == NULL)
    fatalerr ("fatalerr: %s", strerror (errno));

  if (stat (apg_source, &lstat) != -1)
    fatalerr ("fatalerr: %s (par.c) already exists", apg_source);

  if ((fd_c = fopen (apg_source, "w")) == NULL)
    fatalerr ("fatalerr: %s", strerror (errno));

  if (opt_bitsfield & OPT_IF)
    {

      if (stat (apg_if, &lstat) != -1)
	fatalerr ("fatalerr: %s (if.c) already exists", apg_if);

      if ((fd_i = fopen (apg_if, "w")) == NULL)
	fatalerr ("fatalerr: %s", strerror (errno));

    }

/* parsing rules through its own api 
*/

#define H_TEST(p) ( (p)->h_rule_bit_o & opt_bitsfield && (p)->h_rule_bit_t & type_bitsfield )
#define C_TEST(p) ( (p)->c_rule_bit_o & opt_bitsfield && (p)->c_rule_bit_t & type_bitsfield )
#define I_TEST(p) ( (p)->i_rule_bit_o & opt_bitsfield && (p)->i_rule_bit_t & type_bitsfield )
  {
    grill_t *ptr;
    int i = 0;

    ptr = (grill_t *) apg_parser (1, apg_rules);


    while ((i = apg_get_line (&ptr)))
      switch (i)
	{

	case h_rule_label:
	  if (ptr->h_rule_chap == 0xff)
	    {
				/*** fun ***/
	      if (H_TEST (ptr))
		(*fun_list[ptr->h_rule_parag]) (fd_h, ptr->h_rule_comm);

	    }
	  else
	    {
				/*** segment ***/

	      if (H_TEST (ptr))
		extract_segment (ptr_db, fd_h, ptr->h_rule_chap, ptr->h_rule_parag, ptr->h_rule_comm);

	    }
	  break;

	case c_rule_label:

	  if (C_TEST (ptr))
	    {
	      extract_segment (ptr_db, fd_c, ptr->c_rule_chap, ptr->c_rule_parag, ptr->c_rule_comm);

	      if (ptr->c_rule_chap == 1 && ptr->c_rule_parag == 1)
		add_linclude (fd_c, apg_header);

	    }
	  break;

	case i_rule_label:

	  if (ptr->i_rule_chap == 0xff)
	    {
				/*** fun ***/
	      if (I_TEST (ptr))
		(*fun_list[ptr->i_rule_parag]) (fd_i, ptr->i_rule_comm);

	    }
	  else
	    {
				/*** segment ***/
	      if (I_TEST (ptr))
		extract_segment (ptr_db, fd_i, ptr->i_rule_chap, ptr->i_rule_parag, ptr->i_rule_comm);

	    }
	  break;

	}

    apg_free_grill (NULL);
    apg_free_pragma ();
  }


  fclose (fd_c);
  fclose (fd_h);

  if (opt_bitsfield & OPT_IF) fclose(fd_i);

}
