/*
 *  $Id$
 *  usage
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
#include <stdio.h>
#include <common.h>
#include <proto.h>
#include <global.h>

extern char *__progname;

void
usage()
{
	fprintf(stderr, "usage: %s [option]\n", __progname);
	fprintf(stderr,
		"   -t,  --template=NAME               file template\n"
		"   -s,  --source=NAME                 filename, default \'apg.par.c\'\n"
		"   -i,  --include=NAME                filename, default \'apg.tab.h\'\n"
		"   -f,  --if[=filename]               extract classic front-end, default \'apg.fe.c\'\n"
	        "   -a,  --advanced                    extract advanced api\n"
		"   -L,  --sepline=char                separator line, default \'=\'\n"
		"   -T,  --septoken=char               separator token, default \':\'\n"
		"   -v,  --version\n"
		"   -h,  --help[=hidden]               (show furter opts for developers)\n"
		);

	if (opt_bitsfield & OPT_ADVHELP)
		fprintf(stderr,
			"   -d,  --database=NAME               specify a local 'apg.db'\n"
			"   -r,  --rules=NAME                  specify a local \'apg.rules\'\n");

	exit(0);
}
