/*
 *  $Id$
 *  db module-
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
#include <common.h>
#include <db.h>
#include <std.h>
#include <proto.h>

void fatalerr(char *,...);
char *touch_db(int);


extern ientry *index_db;
extern char apg_db[];

char *
touch_db(int action)
{
	int fd;
	struct stat lstat;
	static char *ptr;

	/* DB_LOAD */
	if (action & DB_LOAD) {
		if (stat(apg_db, &lstat) == -1)
			fatalerr("fatalerr: %s no such file. apg is not installed properly",
				 apg_db);
		ptr = (char *) xrealloc(ptr, lstat.st_size + 1);
		fd = open(apg_db, O_RDONLY);
		read(fd, ptr, lstat.st_size);
		close(fd);
		return ptr;
	}
	/* DB_DISCARD */
	if (action & DB_DISCARD) {
		free(ptr);
		return (char *) NULL;
	}
	fatalerr("internal err: touch_db()");
	return (char *) NULL;	/* this is unreachable */
}


void
create_index(char *ptr)
{
	char *start = ptr;
	int chapter = 0, paragraph = 0, in_line = 1;
	ientry *entry_ptr = NULL;

	if (ptr == NULL)
		return;

	index_db = entry_ptr = (ientry *) xmalloc(sizeof(ientry));

	while (*ptr != '\0' && strstr(ptr, INDEX_MARK) != NULL) {
		while (ptr[0] && ptr[1] && ptr[2] && ptr[3]) {
			if (ptr[0] == '/' && ptr[1] == '/' && ptr[2] == '-' && ptr[3] == '[')
				break;
			if (ptr[0] == '\n')
				in_line++;
			ptr++;
		}

		if (ptr[0] && ptr[1] && ptr[2] && ptr[3]) {
			/* ok */
			in_line++;
			*(ptr) = '\0';
			ptr += 4;

		} else
			fatalerr("internal err: apg database maybe corrupted");

		/* Test for apg.db integrity */

		if (sscanf(ptr, "%d,%d", &chapter, &paragraph) < 2)
			fatalerr("internal err: apg database maybe corrupted");

		if ((ptr = strstr(ptr, "\n")) == NULL)
			fatalerr("internal err: apg database maybe corrupted");

		*(ptr++) = '\0';

		entry_ptr->offset = ptr - start;
		entry_ptr->chapter = chapter;
		entry_ptr->paragraph = paragraph;
		entry_ptr->line = in_line;
		entry_ptr->next = (ientry *) xmalloc(sizeof(ientry));
		entry_ptr = entry_ptr->next;
		entry_ptr->next = (ientry *) NULL;
	}
}


int
extract_segment(char *ptr_db, FILE * where, int chapter, int paragraph, char *comm, int l)
{
	char *char_ptr;
	int c = 0;

	ientry *entry_ptr;

	if (!entry_ptr)
		fatalerr("internal err: extract_segment() index_nb is a NULL pointer");

	for   (entry_ptr = index_db; entry_ptr->next ; entry_ptr = entry_ptr->next)
		if ((entry_ptr->chapter == chapter) && (entry_ptr->paragraph == paragraph)) {
			/* This points to the selected frame */
			if (comm != NULL && *comm != '\0') {
				add_comment(where, comm);
				add_cr(where);
			}
			char_ptr = (ptr_db + entry_ptr->offset);

			if (l != 0)
				fprintf(where, "#line %d \"%s\"\n", entry_ptr->line, APG_DB_FILE);

			fprintf(where, "%s", char_ptr);
			return 0;
		}
	}

	fatalerr("internal err: extract_segment() index=(%d,%d) not found", chapter, paragraph);
	return -1;
}
