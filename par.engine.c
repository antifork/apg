#line 2 "/usr/local/share/apg/apg.db"
/* $Id$ */
#line 202 "/usr/local/share/apg/apg.db"
/*
 *  $Id$
 *  apg.par.c
 *  Generated automatically by apg: autoparser generator.
 *
 *  Sources are available at 	
 *		ftp://ftp.antifork.org/apg/
 *		cvs -d :pserver:anoncvs@cvs.antifork.org:/CVS checkout apg
 *
 *  Copyright (c) 1999 Bonelli Nicola  <bonelli@antifork.org>
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

/* standard includes */

#line 230 "/usr/local/share/apg/apg.db"
#define _APG_PARSER_C
#line 1 "par.engine.c"
#include "tab.engine.h"
#line 232 "/usr/local/share/apg/apg.db"
/* mealy prototypes */

#line 233 "/usr/local/share/apg/apg.db"
static int ac_0 __P ((void));
static int ac_1 __P ((void));
static int ac_2 __P ((void));
static int ac_3 __P ((void));
static int ac_4 __P ((void));

/* static variables */

#line 240 "/usr/local/share/apg/apg.db"
static char *file_name;
static int apg_flags, apg_current_line;
static int apg_buff_line, apg_token;
static int apg_state_code, apg_input_code;

#ifdef APG_PERF
static int global_token;
#endif

static u_char *base_tokens;

static char *file_image;

static grill_t *apg_stream;
static grill_t *apg_arena;
static seg_t *head_seg;
static seg_t *tail_seg;

int apg_errno;

/* common defines and internal flags */

#line 261 "/usr/local/share/apg/apg.db"
#define APG_OPEN                0
#define APG_CLOSE               1

#define APG_ACK_CHR          0x06

/* general flags */

#define APG_PARSED_GRILL     0x01
#define APG_RESET_STREAM     0x02
#define APG_OCT_TOKEN        0x04
#define APG_HEX_TOKEN        0x08
#define APG_FATALERR	     0x10

/* msg error codes */

#define APG_TYPE_ERR         0x01
#define APG_NULL_ERR	     0x02
#define APG_OFFSET_ERR       0x03
#define APG_LIMIT_ERR        0x04
#define APG_ESC_ERR          0x05
#define APG_FEW_ERR	     0x06
#define APG_MANY_ERR 	     0x07
#define APG_IPV4_ERR         0x08
#define APG_HOST_ERR         0x09

/* macros */

#line 287 "/usr/local/share/apg/apg.db"
#if defined (__GNUC__) && !defined (__STRICT_ANSI__) && !defined (__ANSI__) && !defined(__cplusplus) 
int c_index[256] =
  {['#'] 1,[APG_SEPLINE] 2,[APG_SEPTOKEN] 3,['\''] 4,['\\'] 5,[' '] 6,
  ['\t'] 6,['\n'] 7
};
#else
int c_index[256];
#endif

#define APG_OPUSH(base_p, offset, obj_p ,t)\
\
( *(t *)((u_char *)base_p+offset+sizeof(u_long)) = (t)*obj_p )

#define APG_PPUSH(base_p, offset, ptr)\
\
( *(u_long *)((u_char *)base_p+offset+sizeof(u_long))=(u_long)ptr)

#define APG_ODIGIT(x)\
\
( '0' <= x && x <= '3' ? 2 : ( '4' <= x && x <= '7' ? 1 : 0 ) )

#define APG_CLIMIT(t,x,v,y)\
\
( (x|y) ? ((t == T_U_32) ? \
((u_int)x<=(u_int)v ) && ((u_int)v<=(u_int)y) : ((x<=v) && (v<=y))) : (1) )

#define APG_MSTRTOL(token,addr_endptr)\
\
( apg_flags &  (APG_OCT_TOKEN|APG_HEX_TOKEN) ? strtol(token,addr_endptr,0) :  strtol(token,addr_endptr,10))

/* private functions, mealy tables */

#line 318 "/usr/local/share/apg/apg.db"
static void
#if __STDC__
fatalerr (char *pattern, ...)
#else
fatalerr (pattern, va_alist)
	char *patter;
	va_dcl
#endif
{
  va_list ap;
#if __STDC__
  va_start (ap, pattern);
#else
  va_start (ap);
#endif

  vfprintf (stderr, pattern, ap);
  fprintf (stderr, "; exit forced.\n");
  va_end (ap);
  exit (-1);

}

/* xmalloc, xcalloc, xrealloc */

static void *
xmalloc ( AAUINT_ARG s)
     AAUINT_DECL(s)
{
  void *result;

  result = malloc (s);
  if (result == 0)
    fatalerr ("xmalloc: virtual memory exhausted");
  return result;
}


static void *
xrealloc (AAVOID$_ARG ptr, AAUINT_ARG s)
     AAVOID$_DECL(ptr)
     AAUINT_DECL(s)
{
  void *result;

  /* Some older implementations of realloc() don't conform to ANSI.  */
  result = ptr ? realloc (ptr, s) : malloc (s);
  if (result == 0)
    fatalerr ("xrealloc: virtual memory exhausted");
  return result;
}

static void *
xcalloc ( AAUINT_ARG nelem, AAUINT_ARG s)
     AAUINT_DECL(nelem)
     AAUINT_DECL(s)
{
  void *result = (char *) calloc (nelem, s);
  if (result == 0)
    fatalerr ("xcalloc: virtual memory exhausted");
  return result;
}


static char *
ioctl_buffer (AACHAR$_ARG fn, AAINT_ARG flag)
     AACHAR$_DECL(fn)
     AAINT_DECL(flag)
{
  int fd, sz;
  struct stat f_stat;

  switch (flag)
    {
    case APG_OPEN:

      file_name = (char *) xmalloc (strlen (fn) + 1);
      strcpy (file_name, fn);

      /* no TOCTOU */

      if (stat (fn, &f_stat) == -1)
	return (char *) NULL;

      sz = (int) f_stat.st_size;

      file_image = (char *) xrealloc (file_image, sz + 1);

      if ((fd = open (fn, O_RDONLY)) == -1)
	fatalerr ("err: %s", strerror (errno));

      if ((sz = read (fd, file_image, sz)) == -1)
	fatalerr ("err: %s", strerror (errno));

      *(file_image + sz) = 0;

      close (fd);
      return file_image;
      break;

    case APG_CLOSE:

      if (file_name)
	free (file_name);
      if (file_image == NULL)
	fatalerr ("err: apg_buffer already closed");

      free (file_image);
      return file_image = (char *) NULL;
      break;
    }

  return (char *) NULL;		/* unreachable */
}


static u_char *p_token, *p_token_0;
static u_char *p_stream;

static int
ac_0 ()
{
  if (*p_stream == '\\')
    p_stream++;
  if (*p_stream++ == '\n')
    apg_buff_line++;

  return 1;
}

static int
ac_1 ()
{
  *(p_token_0++) = *(p_stream++);

  return 1;
}

static int
ac_2 ()
{
  if (*p_stream == '\n')
    apg_buff_line++;
  *p_token_0++ = '\0';

  p_stream++;
  apg_token++;

  return 0;
}

#if defined (__GNUC__) && !defined (__STRICT_ANSI__) && !defined (__cplusplus)
char c_escape[256] =
  {['a'] '\a',['b'] '\b',['t'] '\t',['n'] '\n',['v'] '\v',['f'] '\f',
  ['r'] '\r'
};
#else
int c_escape[256];
#endif


static int
ac_3 ()
{
  register char c;

  p_stream++;

  if ((c = c_escape[*p_stream]))
    {
      /* escape */
      *p_token_0 = c;
    }
  else
    {
      switch (*p_stream)
	{
	case '\n':
	  memmove (p_stream, p_stream + 1,
		   strlen ((const char *) (p_stream + 1)));
	  *p_token_0 = *p_stream;
	  break;
	case '0':
	  if (*(p_stream + 1) == 'x' || *(p_stream + 1) == 'X')
	    apg_flags |= APG_HEX_TOKEN;
	  else
	    apg_flags |= APG_OCT_TOKEN;

	  *p_token_0 = APG_ACK_CHR;
	  p_stream--;
	  break;

	default:
	  *p_token_0 = *p_stream;
	  break;

	}
    }

  p_token_0++;
  p_stream++;

  return 1;

}

static int
ac_4 ()
{
  char *eptr;

  p_stream--;

  if ((eptr = strchr ((const char *) p_stream, '\n')) != NULL)
    *eptr = '\0';

  while (*p_stream != '\n' && (char *) p_stream > (char *) file_image)
    p_stream--;

  fatalerr ("%s:%d: parse error near -> \"%s\" ", file_name,
	    apg_buff_line + 1, ++p_stream);

  return 1;
}

static const short apg_mealy_state_table[9][8] = {
  {1, 4, 6, 6, 6, 6, 0, 0},
  {1, 6, 2, 6, 6, 6, 7, 6},
  {3, 4, 6, 2, 5, 3, 2, 0},
  {3, 4, 6, 2, 5, 3, 8, 0},
  {4, 4, 4, 4, 4, 4, 4, 0},
  {5, 5, 5, 5, 3, 5, 5, 0},
  {6, 6, 6, 6, 6, 6, 6, 6},
  {6, 6, 2, 6, 6, 6, 7, 6},
  {6, 4, 6, 2, 6, 6, 8, 0}
};

static int (*apg_mealy_action_table[9][8]) () =
{
  { ac_1, ac_0, ac_4, ac_4, ac_4, ac_4, ac_0, ac_0} ,
  { ac_1, ac_4, ac_2, ac_4, ac_4, ac_4, ac_0, ac_4} ,
  { ac_1, ac_2, ac_4, ac_2, ac_0, ac_3, ac_0, ac_2} ,
  { ac_1, ac_2, ac_4, ac_2, ac_0, ac_3, ac_0, ac_2} ,
  { ac_0, ac_0, ac_0, ac_0, ac_0, ac_0, ac_0, ac_0} ,
  { ac_1, ac_1, ac_1, ac_1, ac_0, ac_3, ac_1, ac_2} ,
  { ac_4, ac_4, ac_4, ac_4, ac_4, ac_4, ac_0, ac_4} ,
  { ac_4, ac_4, ac_2, ac_4, ac_4, ac_4, ac_0, ac_4} ,
  { ac_4, ac_2, ac_4, ac_2, ac_4, ac_4, ac_0, ac_2} };

static char *
get_token ()
{
  register int reg_input, reg_state;

  apg_current_line = apg_buff_line;
  p_token = p_token_0;

	/*** load registers ***/

  reg_state = apg_mealy_state_table[apg_state_code][apg_input_code];
  reg_input = c_index[*p_stream];

  while ((*apg_mealy_action_table[reg_state][reg_input]) ())
    {
      if (*p_stream == 0)
	return (char *) NULL;	/* EOF */

      if ((reg_state = apg_mealy_state_table[reg_state][reg_input]) == 1)
	apg_token = 0;

      reg_input = c_index[*p_stream];
    }

	/*** save registers ***/

  apg_input_code = reg_input;
  apg_state_code = reg_state;

#ifdef APG_PERF
  global_token++;
#endif

  return (char *) p_token;

}

/* arena menagement */

#line 605 "/usr/local/share/apg/apg.db"
static grill_t *
alloc_segment (AAGRILL$_ARG p)
     AAGRILL$_DECL(p)
{

  grill_t *q;

  /* p reasonably points to the last grill_t. In case it doesn't ... */

  for (q = p; q != NULL; q = q->next)
    p = q;

  /* we must be sure the segment is empty: xcalloc() is required then. */

  q = (grill_t *) xcalloc (1, sizeof (grill_t));

  if (p == NULL)
    return (apg_arena = apg_stream = q);
  else
    return (apg_stream = p->next = q);

}

static
#ifdef __GNUC__
  __inline
#else
#endif
  int
b_search (AACHAR$_ARG key)
     AACHAR$_DECL(key)
{
  register int high, i, low;
  register unsigned long hash;

/* This is similar to the rotating hash, but it actually mixes the internal
state. It takes 9n+9 instructions and produces a full 4-byte result.
Preliminary analysis suggests there are no funnels.  */

  for (hash = 0, i = 0; key[i]; ++i)
    {
      hash += key[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  for (low = -1, high = QMAX_ELEM + 1; high - low > 1;)
    {
      i = (high + low) >> 1;
      if (hash <= line_v[i].hash)
	high = i;
      else
	low = i;
    }
  if (hash == line_v[high].hash)
    return (high);

  fatalerr ("%s:%d: `%s' unknown line label", file_name, apg_buff_line + 1,
	    key);

  return (-1);			/* unreachable */

}

/* token err interface */

#line 673 "/usr/local/share/apg/apg.db"
#define APGLT	"%s:%d: label=%s,token=%d {%s}"  /* label & token */
#define APGLTarg file_name, apg_current_line + 1, line_v[line_id].id, token_id, token
#define APGL	"%s: label=%s"			 /* label only */
#define APGLarg  file_name, line_v[line_id].id

static void
token_fatalerr (AAINT_ARG line_id, 
		AAINT_ARG token_id, 
		AAINT_ARG type, 
		AAINT_ARG errn0, 
		AAINT_ARG low, 
		AAINT_ARG high, 
		AACHAR$_ARG token)
     AAINT_DECL(line_id)
     AAINT_DECL(token_id)
     AAINT_DECL(type)
     AAINT_DECL(errn0)
     AAINT_DECL(low)
     AAINT_DECL(high)
     AACHAR$_DECL(token)
{
  char *p = token;


  while (p && (p = strchr (p, APG_ACK_CHR)))
    *p = '\\';

  switch (errn0)
    {
    case APG_TYPE_ERR:
      fprintf (stderr, APGLT " is designed to be a %s type;\n", APGLTarg , types_id[type]);
      break;
    case APG_NULL_ERR:
      fprintf (stderr, APGLT " isn't an optional argument;\n", APGLTarg );
      break;
    case APG_OFFSET_ERR:
      fprintf (stderr, APGLT " too many tokens;\n", APGLTarg );
      break;
    case APG_LIMIT_ERR:
      fprintf (stderr, APGLT " is designed to be a %s[%d,%d]. Out of range;\n", APGLTarg, types_id[type], low, high);
      break;
    case APG_ESC_ERR:
      fprintf (stderr, APGLT " bad escape sequence;\n", APGLTarg );
      break;
    case APG_MANY_ERR:
      fprintf (stderr, APGL " is designed to occur no more than %d time;\n", APGLarg , high);
      break;
    case APG_FEW_ERR:
      fprintf (stderr, APGL " is designed to occur at least %d time;\n",  APGLarg , low);
      break;
#line 733 "/usr/local/share/apg/apg.db"
    }

  apg_flags |= APG_FATALERR;
}

/* ymalloc & seg_t */

#line 739 "/usr/local/share/apg/apg.db"
static void
alloc_seg_t (AACHAR$_ARG r)
     AACHAR$_DECL(r)
{
  seg_t *p = tail_seg, *q;

  q = (seg_t *) xmalloc (sizeof (seg_t));
  q->ptr = r;
  q->next = NULL;

  if (p)
    p->next = tail_seg = q;
  else
    {
      head_seg = q;
      tail_seg = q;
    }
}

static void *
ymalloc (AASIZE_ARG s)
     AASIZE_DECL(s)
{
  void *p;

  p = (void *) xmalloc (s);
  alloc_seg_t ((char *)p);

  return p;
}

/* strings function */

#line 771 "/usr/local/share/apg/apg.db"
#ifdef __GNUC__
__inline
#else
#endif
static int
strholen (AACHAR$_ARG p)
     AACHAR$_DECL(p)
{
  register int c = 1, s = 0;
  register char *_p = p;

  if (*_p++ != '0')
    return 0;

  if (*_p == 'x' || *_p == 'X')
    {
      c++;
      _p++;
      while (isxdigit (*_p++) && c < 4)
	c++;
    }
  else
    {
      if ((s = APG_ODIGIT (*_p)))
	{
	  _p++;
	  c++;
	  while (APG_ODIGIT (*_p) && c < (2 + s))
	    {
	      _p++;
	      c++;
	    }
	}
    }
  return c;
}

/* apg type checks */

#line 809 "/usr/local/share/apg/apg.db"
#define APG_PROC_OBJECT(p) ( *p == APG_ACK_CHR ? (p+1) : (p)  )

#define APG_LTYPE(l,t)    apgtb[l][t][0]
#define APG_LLOW(l,t)     apgtb[l][t][1]
#define APG_LHIGH(l,t)    apgtb[l][t][2]
#define APG_LOPT(l,t)     apgtb[l][t][3]
#define APG_LREGEX(l,t)   apgtb[l][t][4]


#define APGE_ARG0 	line_id, token_id, APG_LTYPE (line_id, token_id), APG_OFFSET_ERR, \
			APG_LLOW (line_id, token_id), APG_LHIGH (line_id, token_id), token
#define APGE_ARG1 	line_id, token_id, APG_LTYPE (line_id, token_id), APG_LIMIT_ERR, \
			APG_LLOW (line_id, token_id), APG_LHIGH (line_id, token_id), token
#define APGE_ARG2 	line_id, token_id, APG_LTYPE (line_id, token_id), APG_TYPE_ERR, \
			APG_LLOW (line_id, token_id), APG_LHIGH (line_id, token_id), token
#define APGE_ARG3 	line_id, token_id, T_STR, APG_ESC_ERR, 0, 0, token
#define APGE_ARG4 	line_id, token_id, T_HOST, APG_HOST_ERR, APG_LLOW (line_id, token_id), \
			APG_LHIGH (line_id, token_id), token
#define APGE_ARG5 	line_id, token_id, T_IPV4, APG_IPV4_ERR, APG_LLOW (line_id, token_id), \
			APG_LHIGH (line_id, token_id), token
#define APGE_ARG6 	apg_stream->type_line, j, APG_LTYPE (apg_stream->type_line, j), \
			APG_NULL_ERR, APG_LLOW (apg_stream->type_line, j), \
			APG_LHIGH (apg_stream->type_line, j), NULL
#define APGE_ARG7 	apg_stream->type_line, 0, 0, APG_MANY_ERR, 0, \
			rep_limits[apg_stream->type_line][1], NULL
#define APGE_ARG8 	i, 0, 0, APG_FEW_ERR, rep_limits[i][0], 0, NULL

static void
token_analysis (AACHAR$_ARG token, AAINT_ARG line_id, AAINT_ARG token_id)
     AACHAR$_DECL(token)
     AAINT_DECL(line_id)
     AAINT_DECL(token_id)

{
  char *endptr, *pp = NULL;
  long sp;
  int offset = 0;


  /* first step */

  if ((offset = apg_offset[line_id][token_id]) == -1)
    token_fatalerr (APGE_ARG0);

  if (token && *token == '\0')
    return;			/* NULL token */

  switch (APG_LTYPE (line_id, token_id))
    {
    case T_STR:
    case T_HOST:
    case T_IPV4:

      pp = (char *) ymalloc (strlen (token) + 1);
      break;

    default:
      token = APG_PROC_OBJECT (token);
      break;
    }

  switch (APG_LTYPE (line_id, token_id))
    {

#line 874 "/usr/local/share/apg/apg.db"
    case T_INT:
    case T_U_32:
    case T_SHORT:
    case T_U_16:
    case T_CHAR:
    case T_U_8:

      if (APG_LTYPE (line_id, token_id) == T_CHAR && strlen (token) == 1)
	{
	  APG_OPUSH (apg_stream, offset, token, char);
	  return;
	}

      sp = APG_MSTRTOL (token, &endptr);

      if (*endptr=='\0')
	{

	  if ( APG_CLIMIT (	APG_LTYPE (line_id, token_id), 
				APG_LLOW  (line_id, token_id),
	       			sp, 
				APG_LHIGH (line_id, token_id)))
	    {
	      switch (APG_LTYPE (line_id, token_id))
		{
		case T_INT:
		  APG_OPUSH (apg_stream, offset, &sp, int);
		  break;
		case T_U_32:
		  APG_OPUSH (apg_stream, offset, &sp, u_int);
		  break;
		case T_SHORT:
		  APG_OPUSH (apg_stream, offset, &sp, short);
		  break;
		case T_U_16:
		  APG_OPUSH (apg_stream, offset, &sp, u_short);
		  break;
		case T_CHAR:
		  APG_OPUSH (apg_stream, offset, &sp, char);
		  break;
		case T_U_8:
		  APG_OPUSH (apg_stream, offset, &sp, u_char);
		  break;
		}
	      return;
	    }
	  else
	    token_fatalerr (APGE_ARG1);
	}
      else
	token_fatalerr (APGE_ARG2);

      return;
      break;

#line 930 "/usr/local/share/apg/apg.db"
    case T_STR:
      {
	strcpy (pp, token);

	if (strchr (token, APG_ACK_CHR))
	  {
	    char *ptr = pp, bufftemp[6] = "";

	    while ((ptr = (char *) strchr (ptr, APG_ACK_CHR)))
	      {
		int i = strholen (ptr + 1);
		int j = strlen (ptr + i);

		strncpy (bufftemp, ptr + 1, i);
		bufftemp[i] = 0;
		*ptr = (char) strtol (bufftemp, NULL, 0);

		if (*ptr == '\0' || i == 0)
		  token_fatalerr (APGE_ARG3);

		memmove (ptr + 1, ptr + i + 1, j);
		*(ptr + j + 1) = 0;
		ptr++;

	      }
	  }

#line 1000 "/usr/local/share/apg/apg.db"
	if (!(	APG_LLOW (line_id, token_id)
	    || 	APG_LHIGH (line_id, token_id))
	    || 	APG_CLIMIT (	APG_LTYPE (line_id, token_id),
			   	APG_LLOW (line_id, token_id), 
				(int)strlen (pp),
			   	APG_LHIGH (line_id, token_id)))
	  {
	    APG_PPUSH (apg_stream, offset, pp);
	    return;
	  }

	else
	  token_fatalerr (APGE_ARG1);

	return;
      }
      break;

#line 1136 "/usr/local/share/apg/apg.db"
    }
  return;
}

/* error api */

#line 1141 "/usr/local/share/apg/apg.db"
static char *err_table[] = {
  "apg: ok",
  "apg: grill arena is empty",
  "apg: NULL pointer",
  "apg: end of grill",
};

char *
apg_strerror (AAINT_ARG errnum)
     AAINT_DECL(errnum)
{
  static char *sb;
  int i = errnum;

  if (errnum < 1 || errnum > APG_MAXERR)
    i = 0;
  sb = (char *) xrealloc (sb, strlen (err_table[i]) + 1);
  strcpy (sb, err_table[i]);

  return sb;

}

/* standard api */

#line 1165 "/usr/local/share/apg/apg.db"
void
apg_free_grill (AAGRILL$_ARG p)
     AAGRILL$_DECL (p)
{
  grill_t *s;

  if (p == NULL)
    p = apg_arena;

  while (p != NULL)
    {
      s = p->next;
      free (p);
      p = s;

    }
}

void
apg_free_pragma ()
{
  register seg_t *p = head_seg, *q = p;

  while ((p = q))
    {
      if (p->ptr)
	free (p->ptr);
      q = p->next;
      free (p);
    }
  head_seg = NULL;
}


int
apg_get_line (AAGRILL$$_ARG apg_user_ptr)
     AAGRILL$$_DECL(apg_user_ptr)
{
  apg_errno = APG_EOK;

  if (!(apg_flags & APG_PARSED_GRILL))
    {
      apg_errno = APG_EEMPTY;
      return 0;
    }

  if (*apg_user_ptr == '\0')
    {
      apg_errno = APG_ENULL;
      return 0;
    }

  if (!(apg_flags & APG_RESET_STREAM))
    {
      apg_flags |= APG_RESET_STREAM;
      apg_stream = *apg_user_ptr;
      return apg_stream->type_line;
    }

  /* check if apg_stream != NULL */

  if (apg_stream && (*apg_user_ptr = apg_stream = apg_stream->next))
    return apg_stream->type_line;
  else
    {
      apg_errno = APG_EEOG;
      return 0;
    }

}

/* parser */

#line 1281 "/usr/local/share/apg/apg.db"
grill_t *
#if __STDC__
apg_parser (int q, ...)
#else
apg_parser (q, va_alist)
	int q;
	va_dcl
#endif
{
  register long shift_reg = 0;
  register int i = q, j = 0;

  va_list ap;
  char *b_stream = NULL, *tk = NULL;
  char *file;
#line 1302 "/usr/local/share/apg/apg.db"

#if __STDC__
  va_start (ap, q);
#else
  va_start (ap);
#endif
  /* cleaning apg_flags */

  apg_flags = 0;
  apg_errno = APG_EOK;


  file = va_arg (ap, char *);

  while (i-- && (b_stream = ioctl_buffer (file, APG_OPEN)) == NULL  )
    file = va_arg (ap, char *);

  if (b_stream == NULL)
    {
      i = q;
      va_start (ap, q);

      fprintf (stderr, "err: ");
      while (i--)
	fprintf (stderr, "%s ", va_arg (ap, char *));

      fprintf (stderr, "; no such file[s] to parse; exit forced.\n");

      exit (1);
    }

  va_end (ap);

/* ansicare vector setup */

#if !defined (__GNUC__) || defined (__STRICT_ANSI__) || defined (__ANSI__) || defined (__cplusplus)

  c_index['#'] = 1;
  c_index[APG_SEPLINE] = 2;
  c_index[APG_SEPTOKEN] = 3;
  c_index['\''] = 4;
  c_index['\\'] = 5;
  c_index[' '] = 6;
  c_index['\t'] = 6;
  c_index['\n'] = 7;

  c_escape['a'] = '\a';
  c_escape['b'] = '\b';
  c_escape['t'] = '\t';
  c_escape['n'] = '\n';
  c_escape['v'] = '\v';
  c_escape['f'] = '\f';
  c_escape['r'] = '\r';

#endif

  /* apg get token setup */

  if (p_token == NULL)
    {
      base_tokens = p_token = p_token_0 =
	(u_char *) xcalloc (strlen (b_stream), sizeof (char));

      p_stream = (u_char *) b_stream;
      apg_input_code = c_index[*p_stream];
    }

  while ((tk = get_token ()))
    {

      switch (apg_token)
	{

	case 0:		/* null */
	  break;

	case 1:		/* label line */

	  /* before parsing the new line, we check the shift register in
	     order to detect which token can be NULL */

	  if (apg_stream)
	    for (j = 1; j < APG_MAXARG; j++)
	      if (APG_LOPT (apg_stream->type_line, j)
		  && ~shift_reg & (1 << j))
		token_fatalerr (APGE_ARG6);


	  shift_reg = 0;	/* clear */

	  apg_stream = alloc_segment (apg_stream);
	  apg_stream->type_line = b_search (tk);

#line 1404 "/usr/local/share/apg/apg.db"
	  break;
	default:		/* token */

	  /* shift_reg setup: (apg_token == 1 ? line_id : token_id) */

	  if (*tk)
	    shift_reg |= 1 << (apg_token - 1);

	  token_analysis (tk, apg_stream->type_line, apg_token - 1);

	  break;
	}
    }

  /* now we check the last line */

  if (apg_stream)
    for (j = 1; j < APG_MAXARG; j++)
      if (APG_LOPT (apg_stream->type_line, j) && ~shift_reg & (1 << j))
	token_fatalerr (APGE_ARG6);


#line 1435 "/usr/local/share/apg/apg.db"
  free (base_tokens);
  ioctl_buffer (NULL, APG_CLOSE);

  apg_flags |= APG_PARSED_GRILL;

  if (apg_flags & APG_FATALERR)
    fatalerr ("apg_parser(): encontered some errors");

#line 1464 "/usr/local/share/apg/apg.db"

  return apg_arena;

}


