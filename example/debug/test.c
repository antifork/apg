#include <stdlib.h>
#include <stdio.h>
#include "apg.tab.h"

int main()
{
int k;
grill_t *ptr;


ptr = (grill_t *)apg_parser(1,"test");

	while( (k=apg_get_line(&ptr)) );

apg_free_grill(ptr);
apg_free_pragma();

exit(1);

}
