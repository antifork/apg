#include <stdlib.h>
#include <stdio.h>
#include "apg.tab.h"

int main()
{
int k;
grill_t *ptr, *null=NULL;


/***** error condition *****/
apg_get_line(&ptr);
printf("[%s]\n",apg_strerror(apg_errno));  
/***************************/

ptr = (grill_t *)apg_parser(1,"test");

/***** error condition ******/
apg_get_line(&null); // error condition
printf("[%s]\n",apg_strerror(apg_errno)); 
/****************************/

while( (k=apg_get_line(&ptr)) );

/***********************/
printf("[%s]\n",apg_strerror(apg_errno)); // err output
/***********************/

apg_free_grill(ptr);
exit(1);

}
