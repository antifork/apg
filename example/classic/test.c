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
	printf ("line_1: %d %d %d %d\n", ptr->line_1_a, ptr->line_1_b, ptr->line_1_c,
		ptr->line_1_d);
	break;
      case line_2_label:
	printf ("line_2: %d %d %d\n", ptr->line_2_a, ptr->line_2_b, ptr->line_2_c);
	break;
      case line_3_label:
	printf ("line_3: [%c] [%c] [%c]\n", ptr->line_3_a, ptr->line_3_b,
		ptr->line_3_c);
	break;
      case line_4_label:
	printf ("line_4: {%d}\n", ptr->line_4_a);
	break;
      case line_5_label:
	printf ("line_5: [%s] [%s]\n", ptr->line_5_a, ptr->line_5_b);
	break;
      case line_6_label:
	printf ("line_6: [%s] [%s]\n", ptr->line_6_a, ptr->line_6_b);
	break;
        case line_7_label:
	printf ("line_7: [%s] [%s] [%s] %d\n", ptr->line_7_h, ptr->line_7_f, ptr->line_7_g,ptr->line_7_l);
	break;
      }

  apg_free_grill (ptr);
  apg_free_pragma ();

  exit (1);

}
