#include <stdlib.h>
#include <stdio.h>
#include "apg.tab.h"

int main()
{
int k;
grill_t *ptr;

ptr = (grill_t *)apg_parser(1,"test");

 while ((ptr = (grill_t*) apg_seek(line_6_label,ptr)))
	printf("[%s] [%s]\n",ptr->line_6_a,ptr->line_6_b);

apg_free_grill(ptr);
//apg_free_pragma();
exit(1);

}
