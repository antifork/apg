#include <stdlib.h>
#include <stdio.h>
#include "apg.tab.h"

int
main ()
{
  int k;
  grill_t *ptr;


  ptr = (grill_t *) apg_parser (1, "test");

  while ((k = apg_get_line (&ptr)))
    switch (k)
      {
      case line_1_label:
	printf ("%d %d %d %d\n", ptr->line_1_a, ptr->line_1_b, ptr->line_1_c,
		ptr->line_1_d);
	break;
      case line_2_label:
	printf ("%d %d %d\n", ptr->line_2_a, ptr->line_2_b, ptr->line_2_c);
	break;
      case line_3_label:
	printf ("[%c] [%c] [%c]\n", ptr->line_3_a, ptr->line_3_b,
		ptr->line_3_c);
	break;
      case line_4_label:
	printf ("{%d}\n", ptr->line_4_a);
	break;
      case line_5_label:
	printf ("[%s] [%s]\n", ptr->line_5_a, ptr->line_5_b);
	break;
      case line_6_label:
	printf ("[%s] [%s]\n", ptr->line_6_a, ptr->line_6_b);
	break;
      }

  apg_free_grill (ptr);
  apg_free_pragma ();

  exit (1);

}
