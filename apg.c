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

#include <locale.h>
#include <config.h>
#include <std.h>
#include <tree.h>
#include <common.h>
#include <proto.h>

static const char ID[] = "$Id$";

char filein[129], *ptr_db;

char apg_source[128] = "apg.par.c",
     apg_header[128] = "apg.tab.h", 
     apg_if[128]     = "apg.fe.c" ,
     apg_db[128], apg_rules[128];

char apg_separator[8]="#=:'\\ \t\n";

static char *sep_des[]= {
     "comment",
     "line delimiter",
     "token delimiter",
     "string protection",
     "slash",
     "SPACE",
     "HT",
     "LF",
     NULL, };


ientry *index_db;

FILE *yyin;

long lineno = 1, regno = 1, opt_bitsfield, type_bitsfield;

extern t_node *ptr_node, *root_node;

void
fatalerr (char *pattern, ...)
{
  va_list ap;
  va_start (ap, pattern);

  vfprintf (stderr, pattern, ap);
  fprintf (stderr, "; exit forced.\n");

  va_end (ap);

  exit (-1);

}

void
check_separator(void)
{
register int i,j;

  for (i=0;i<7;i++)
        for(j=i+1;j<8;j++)
          if (apg_separator[i]==apg_separator[j])    
             fatalerr("delimiters must differ: %s = %s = [%c]",
			sep_des[i],
			sep_des[j],
			apg_separator[i]);
}

static struct option long_options[] = {
  {"version", no_argument, NULL, 'v'},
  {"advanced", no_argument, NULL, 'a'},

  {"if", optional_argument, NULL, 'f'},
  {"help", optional_argument, NULL, 'h'},

  {"template", required_argument, NULL, 't'},
  {"source", required_argument, NULL, 's'},
  {"include", required_argument, NULL, 'i'},

  {"sepline", required_argument, NULL, 'L'},
  {"septoken", required_argument, NULL, 'T'},

  {"database", required_argument, NULL, 'd'},
  {"rules", required_argument, NULL, 'r'},
  {0, 0, 0, 0}
};

int
main (int argc, char **argv)
{

  if (argc < 2)
    fatalerr ("No arguments given");
  /*
   * default verbose option 
   */

  {
    int c = 0;

    while ((c = getopt_long (argc, argv, "vaf::t:s:i:h::d:r:L:T:", long_options, 0)) != EOF)
      switch (c)
	{
	case 'h':
	  if (optarg && !strcmp (optarg, "hidden"))
	    opt_bitsfield |= OPT_ADVHELP;

	  usage (argv[0]);
	  break;
	case 'v':
	  fprintf (stderr, "%s on %s\n", VERSION, CPU_MFR_OPSYS);
	  if ( ~opt_bitsfield & OPT_DB )
    	  	strcpy (apg_db, APG_DB_FILE);
	  if ( ~opt_bitsfield & OPT_RULES )
    	  	strcpy (apg_rules, APG_AUTORULES);
  	  ptr_db = touch_db (DB_LOAD);
  	  create_index (ptr_db);
          extract_segment (ptr_db, stderr, 0, 0, NULL,0);
	  touch_db (DB_DISCARD);
	  exit (0);
	  break;
	case 'a':
	  opt_bitsfield |= OPT_ADVAN;
	  break;
	case 't':
	  opt_bitsfield |= OPT_FDIN;
	  strncpy (filein, optarg, 127);
	  break;
	case 'f':
          if (optarg !=NULL)
		strncpy (apg_if, optarg, 127); 
	  opt_bitsfield |= OPT_IF;
	  break;
	case 's':
	  strncpy (apg_source, optarg, 127);
	  break;
	case 'i':
	  strncpy (apg_header, optarg, 127);
	  break;
	case 'd':
	  opt_bitsfield |= OPT_DB;
	  strcpy (apg_db, optarg);
	  break;
	case 'r':
	  opt_bitsfield |= OPT_RULES;
	  strcpy (apg_rules, optarg);
	  break;
	case 'L':
          apg_separator[1]=*optarg;
	  break;
	case 'T':
	  apg_separator[2]=*optarg;
	  break;

	}

  }

  if (~opt_bitsfield & OPT_DB)
    strcpy (apg_db, APG_DB_FILE);

  if (~opt_bitsfield & OPT_RULES)
    strcpy (apg_rules, APG_AUTORULES);

  if (~opt_bitsfield & OPT_FDIN)
    fatalerr ("No input file given. Type apg -h");

  fprintf (stderr, "%s on %s %s\n", VERSION, CPU_MFR_OPSYS, ID);
  fprintf (stderr, "processing %s\n", filein);

  check_separator();

  if ((yyin = fopen (filein, "r")) == NULL)
    fatalerr ("%s: file not found", filein);

  /*
   * internal error check: this tryes to create a leaf without a node
   * create_leaf(NULL,"err",0,0,0); 
   */

  ptr_node = root_node = create_node (root_node, "root", -1, -1);
  yyparse ();

  isort_tree (root_node);
  print_tree (root_node);

  ptr_db = touch_db (DB_LOAD);
  create_index (ptr_db);
  tab_par_creator ();

  (void)touch_db (DB_DISCARD);
  
  exit (0);
}
