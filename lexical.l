%{
/* 
 *  $Id$
 *  %% apg: lexical analyzer. 
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

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "grammar.tab.h"

extern int lineno;
extern char filein[];

/* prototypes */
void fatalerr (char *, ...);

%}


DEC	[-+]?[0-9]+
HEX	0x[0-9a-f]+
OCT	0[0-7]+
REG	[^\"\n]*	
%%


int			{ return T_INT; 	}
short			{ return T_SHORT; 	}
char			{ return T_CHAR; 	}
u_32			{ return T_U_32; 	}
u_16			{ return T_U_16; 	}
u_8			{ return T_U_8; 	}
str			{ return T_STR; 	}
host			{ return T_HOST; 	}
ipv4			{ return T_IPV4; 	}


#.*\n			{ lineno++; 		} 
\n			{ lineno++; return EOL;	}
\\\n			{ lineno++; 		}

[ \t\r]+        	/* skip whitespace */



"\""{REG}"\""	        {
			    yytext++;
			    *(yytext+strlen(yytext)-1)=0;
                            yylval.regex = (char *) strdup(yytext);
                            return T_REGEX;

                        }

[:,=()\[\]\+\-\*\/]     { return yytext[0]; }


{DEC}			{ 

			if ( ((yylval.number= strtol(yytext,(char **)NULL,10)) == LONG_MAX) && (errno==ERANGE) )
				{
				if ( ((yylval.u_number= strtoul(yytext,(char **)NULL,10)) == ULONG_MAX) && (errno==ERANGE) ) 
					fatalerr("Warning: %s:%d %s",filein,lineno,strerror(errno));

				return T_U_NUMBER;
				}
 
		  	if ( yylval.number == LONG_MIN )
				fatalerr("Warning: %s:%d %s",filein,lineno,strerror(errno));

		 	 return T_NUMBER;   
			}


[^ :,=\t\r\n\"()\[\]\+\-\*\/]+ 	{ 
					yylval.string = (char *) strdup(yytext); 
					return T_STRING; 
				}


%%

/*
 * Also define a yywrap.  
int
yywrap()
{
        return 1;
}
 */

