/* $Id$ */
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

#define _APG_PARSER_C
#include "tab.engine.h"
/* mealy prototypes */

static int ac_0 P ((void));
static int ac_1 P ((void));
static int ac_2 P ((void));
static int ac_3 P ((void));
static int ac_4 P ((void));

/* static variables */

static char *file_name;
static int apg_flags, apg_current_line;
static int apg_buff_line, apg_token;
static int apg_state_code, apg_input_code;

#ifdef APG_PERF
static int global_token;
#endif

static unsigned char *base_tokens;

static char *file_image;

static grill_t *apg_stream;
static grill_t *apg_arena;
static seg_t *head_seg;
static seg_t *tail_seg;

int apg_errno;

/* common defines and internal flags */

#define APG_OPEN                0
#define APG_CLOSE               1

#define APG_ACK_CHR          0x06

/* token option */

#define TOKEN_REQUIRED	     0x01

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

#if defined (__GNUC__) && !defined (__STRICT_ANSI__)
int c_index[256] =
  {['#'] 1,[APG_SEPLINE] 2,[APG_SEPTOKEN] 3,['\''] 4,['\\'] 5,[' '] 6, ['\t'] 6,['\n'] 7
};
#else
int c_index[256];
#endif

#ifndef ub4
#define ub4     unsigned long
#define ub2     unsigned short
#define ub1     unsigned char
#endif

#define O_PUSH(base_P, offset, obj_P ,t)\
\
( *(t *)((ub1 *)base_P+offset+sizeof(ub4)) = (t)*obj_P )

#define P_PUSH(base_P, offset, ptr)\
\
( *(ub4 *)((ub1 *)base_P+offset+sizeof(ub4))=(ub4)ptr)

#define ISODIGIT(x)\
\
( '0' <= x && x <= '3' ? 2 : ( '4' <= x && x <= '7' ? 1 : 0 ) )

#define C_LIMIT(t,x,v,y)\
\
( (x|y) ? ((t == T_U_32) ? \
((unsigned int)x<=(unsigned int)v ) && ((unsigned int)v<=(unsigned int)y) : ((x<=v) && (v<=y))) : (1) )

#define M_STRTOL(token,addr_endptr)\
\
( apg_flags &  (APG_OCT_TOKEN|APG_HEX_TOKEN) ? strtol(token,addr_endptr,0) :  strtol(token,addr_endptr,10))

/* private functions, mealy tables */

/* This is similar to the rotating hash, but it actually mixes the internal 
state. It takes 9n+9 instructions and produces a full 4-byte result.
Preliminary analysis suggests there are no funnels.  */

#ifdef __GNUC__
__inline
#else
#endif
unsigned long
one_at_a_time_hash (char *key)
{
  int hash, i;
  for (hash = 0, i = 0; key[i]; ++i)
    {
      hash += key[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return (hash);
}

static void
fatalerr (char *pattern, ...)
{
  va_list ap;
  va_start (ap, pattern);
  vfprintf (stderr, pattern, ap);
  fprintf (stderr, "; exit forced.\n");
  va_end (ap);
  exit (-1);

}

/* xmalloc, xcalloc, xrealloc */

static void *
xmalloc (size)
     unsigned int size;
{
  void *result = malloc (size);
  if (result == 0)
    fatalerr ("xmalloc: virtual memory exhausted");
  return result;
}


static void *
xrealloc (ptr, size)
     void *ptr;
     unsigned int size;
{
  void *result;

  /* Some older implementations of realloc() don't conform to ANSI.  */
  result = ptr ? realloc (ptr, size) : malloc (size);
  if (result == 0)
    fatalerr ("xrealloc: virtual memory exhausted");
  return result;
}

static void *
xcalloc (nelem, elsize)
     unsigned int nelem;
     unsigned int elsize;
{
  void *result = (char *) calloc (nelem, elsize);
  if (result == 0)
    fatalerr ("xcalloc: virtual memory exhausted");
  return result;
}


static char *
ioctl_buffer (char *fn, int flag)
{
  int fd,sz;
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

      if ((sz = read (fd, file_image, sz )) == -1 )
	fatalerr ("err: %s", strerror (errno));

      *(file_image + sz ) = 0;

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


static unsigned char *p_token, *p_token_0;
static unsigned char *p_stream;

static int
ac_0 (void)
{
  if (*p_stream   == '\\') p_stream++;
  if (*p_stream++ == '\n') apg_buff_line++;

  return 1;
}

static int
ac_1 (void)
{
  *(p_token_0++) = *(p_stream++);

  return 1;
}

static int
ac_2 (void)
{
  if (*p_stream == '\n') apg_buff_line++;
  *p_token_0++ = '\0';

  p_stream++;
  apg_token++;

  return 0;
}

#if defined (__GNUC__) && !defined (__STRICT_ANSI__)
char c_escape[256] =
  {['a'] '\a',['b'] '\b',['t'] '\t',['n'] '\n',['v'] '\v',['f'] '\f', ['r'] '\r'
};
#else
int c_escape[256];
#endif


static int
ac_3 (void)
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
	  memmove (p_stream, p_stream + 1, strlen ((const char *) (p_stream + 1)));
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
ac_4 (void)
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

static int (*apg_mealy_action_table[9][8]) (void) =
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
get_token (void)
{
  register int reg_input, reg_state;

  apg_current_line = apg_buff_line;
  p_token = p_token_0;

	/*** load registers ***/

  reg_state = apg_mealy_state_table[apg_state_code][apg_input_code];
  reg_input = c_index[*p_stream];

  while ((*apg_mealy_action_table[reg_state][reg_input]) ())
    {
      if (*p_stream == 0 ) return (char *) NULL;  /* EOF */

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

static grill_t *
alloc_segment (grill_t * p)
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

#ifdef __GNUC__
__inline
#else
#endif
int
b_search (key)
     char * key;
{
  register int high, i, low;
  register unsigned long hash;
  hash = one_at_a_time_hash (key);

  for (low = -1, high = QMAX_ELEM+1; high - low > 1;)
    {
      i = (high + low) >> 1;
      if (hash <= line_v[i].hash)
        high = i;
      else
        low = i;
    }
  if ( hash == line_v[high].hash )
    return (high);

  fatalerr ("%s:%d: `%s' unknown line label", file_name, apg_buff_line + 1, key);

  return (-1); /* unreachable */

}

/* token err interface */

static void
token_fatalerr (int line_id, int token_id, int type, int errn0, int low,
		int high, char *token)
{
  char *p = token;


  while (p && (p = strchr (p, APG_ACK_CHR)))
    *p = '\\';

  switch (errn0)
    {
    case APG_TYPE_ERR:
      fprintf (stderr,
	       "%s:%d: label=%s,token=%d -> {%s} is designed to be a %s type;\n",
	       file_name, apg_current_line + 1, line_v[line_id].id, token_id,
	       token, types_id[type]);
      break;
    case APG_NULL_ERR:
      fprintf (stderr,
	       "%s:%d: label=%s,token=%d -> {%s} isn't an optional argument;\n",
	       file_name, apg_current_line, line_v[line_id].id, token_id,
	       token);

      break;
    case APG_OFFSET_ERR:
      fprintf (stderr,
	       "%s:%d: label=%s,token=%d -> {%s} too many tokens;\n",
	       file_name, apg_current_line + 1, line_v[line_id].id, token_id,
	       token);

      break;
    case APG_LIMIT_ERR:
      fprintf (stderr,
	       "%s:%d: label=%s,token=%d -> {%s} is designed to be a %s[%d,%d]. Out of range;\n",
	       file_name, apg_current_line + 1, line_v[line_id].id, token_id,
	       token, types_id[type], low, high);

      break;
    case APG_ESC_ERR:
      fprintf (stderr,
	       "%s:%d: label=%s,token=%d -> {%s} bad escape sequence;\n",
	       file_name, apg_current_line + 1, line_v[line_id].id, token_id,
	       token);
      break;

    case APG_MANY_ERR:
      fprintf (stderr,
	       "%s: label=%s is designed to occur no more than %d time;\n",
	       file_name, line_v[line_id].id, high);
      break;

    case APG_FEW_ERR:
      fprintf (stderr,
	       "%s: label=%s is designed to occur at least %d time;\n",
	       file_name, line_v[line_id].id, low);
      break;

    }

  apg_flags |= APG_FATALERR;
}

/* ymalloc & seg_t */

static void
alloc_seg_t (char *r)
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
ymalloc (size_t size)
{
  void *p;

  p = (void *) xmalloc (size);
  alloc_seg_t (p);

  return p;
}

/* strings function */

#ifdef __GNUC__
__inline
#else
#endif
static int
strholen (char *p)
{
  register int c = 1, s = 0;
  register char *P = p;

  if (*P++ != '0')
    return 0;

  if (*P == 'x' || *P == 'X')
    {
      c++;
      P++;
      while (isxdigit (*P++) && c < 4)
	c++;
    }
  else
    {
      if ((s = ISODIGIT (*P)))
	{
	  P++;
	  c++;
	  while (ISODIGIT (*P) && c < (2 + s))
	    {
	      P++;
	      c++;
	    }
	}
    }
  return c;
}

/* apg type checks */

#define PROC_OBJECT(p) ( *p == APG_ACK_CHR ? (p+1) : (p)  )

#define L_TYPE(l,t)    apgtb[l][t][0]
#define L_LOW(l,t)     apgtb[l][t][1]
#define L_HIGH(l,t)    apgtb[l][t][2]
#define L_OPT(l,t)     apgtb[l][t][3]
#define L_REGEX(l,t)   apgtb[l][t][4]

static void
token_analysis (char *token, int line_id, int token_id)
{
  char *endptr, *pp = NULL;
  long sp;
  int offset = 0;


  /* first step */

  if ((offset = apg_offset[line_id][token_id]) == -1)
    token_fatalerr (line_id, token_id,
		    L_TYPE (line_id, token_id), APG_OFFSET_ERR,
		    L_LOW (line_id, token_id), L_HIGH (line_id, token_id),
		    token);


  if (token && !*token)
    return;			/* NULL token */

  switch (L_TYPE (line_id, token_id))
    {
    case T_STR:
    case T_HOST:
    case T_IPV4:
      
      pp = (char *) ymalloc (strlen (token) + 1);
      break;

    default:
      token = PROC_OBJECT (token);
      break;
    }

  switch (L_TYPE (line_id, token_id))
    {

    case T_INT:
    case T_U_32:
    case T_SHORT:
    case T_U_16:
    case T_CHAR:
    case T_U_8:

      if (L_TYPE (line_id, token_id) == T_CHAR && strlen (token) == 1)
	{
	  O_PUSH (apg_stream, offset, token, char);
	  return;
	}

      sp = M_STRTOL (token, &endptr);

      if (!*endptr)
	{

	  if (C_LIMIT (L_TYPE (line_id, token_id),L_LOW (line_id, token_id), sp, L_HIGH (line_id, token_id)))
	    {
	      switch (L_TYPE (line_id, token_id))
		{
		case T_INT:
		  O_PUSH (apg_stream, offset, &sp, int);
		  break;
		case T_U_32:
		  O_PUSH (apg_stream, offset, &sp, unsigned int);
		  break;
		case T_SHORT:
		  O_PUSH (apg_stream, offset, &sp, short);
		  break;
		case T_U_16:
		  O_PUSH (apg_stream, offset, &sp, unsigned short);
		  break;
		case T_CHAR:
		  O_PUSH (apg_stream, offset, &sp, char);
		  break;
		case T_U_8:
		  O_PUSH (apg_stream, offset, &sp, unsigned char);
		  break;
		}
	      return;
	    }
	  else
	    token_fatalerr (line_id, token_id,
			    L_TYPE (line_id, token_id), APG_LIMIT_ERR,
			    L_LOW (line_id, token_id),
			    L_HIGH (line_id, token_id), token);
	}
      else
	token_fatalerr (line_id, token_id,
			L_TYPE (line_id, token_id), APG_TYPE_ERR,
			L_LOW (line_id, token_id),
			L_HIGH (line_id, token_id), token);
      return;
      break;

    case T_STR:
      {
	strcpy ( pp, token );

	if ( strchr (token, APG_ACK_CHR) )
	{
	  char *ptr=pp, bufftemp[6] = "";

	  while ((ptr = (char *) strchr (ptr, APG_ACK_CHR)))
	    {
	      int i = strholen (ptr + 1);
	      int j = strlen (ptr + i);

	      strncpy (bufftemp, ptr + 1, i);
	      bufftemp[i] = 0;
	      *ptr = (char) strtol (bufftemp, NULL, 0);

	      if (!*ptr || !i)
		  token_fatalerr (line_id, token_id, T_STR, APG_ESC_ERR, 0, 0, token);

	      memmove (ptr + 1, ptr + i + 1, j);
	      *(ptr + j + 1) = 0;
	      ptr++;

	    }
	}

	if (! (L_LOW (line_id, token_id)
	    || L_HIGH (line_id, token_id))
	    || C_LIMIT (L_TYPE (line_id, token_id),L_LOW (line_id, token_id), strlen (pp), L_HIGH (line_id, token_id)))
	  {
	    P_PUSH (apg_stream, offset, pp);
	    return;
	  }

	else
	  token_fatalerr (line_id, token_id,
			  L_TYPE (line_id, token_id), APG_LIMIT_ERR,
			  L_LOW  (line_id, token_id),
			  L_HIGH (line_id, token_id), token);

	return;
      }
      break;

    }
  return;
}

/* error api */

static char *err_table[] = {
  "apg: ok",
  "apg: grill arena is empty",
  "apg: NULL pointer",
  "apg: end of grill",
};

char *
apg_strerror (int errnum)
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

void
apg_free_grill (grill_t * p_ptr)
{
  grill_t *s_ptr;

  if (!p_ptr)
    p_ptr = apg_arena;

  while (p_ptr != NULL)
    {
      s_ptr = p_ptr->next;
      free (p_ptr);
      p_ptr = s_ptr;

    }
}

void
apg_free_pragma (void)
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
apg_get_line (grill_t ** apg_user_ptr)
{
  apg_errno = APG_EOK;

  if (!(apg_flags & APG_PARSED_GRILL))
    {
      apg_errno = APG_EEMPTY;
      return 0;
    }

  if (!*apg_user_ptr)
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


grill_t *
apg_parser (int q, ...)
{
  register long shift_reg = 0;
  register int i = q, j = 0;

  va_list ap;
  char *b_stream = NULL, *tk = NULL;
  char *file;

  va_start (ap, q);

  /* cleaning apg_flags */

  apg_flags = 0;
  apg_errno = APG_EOK;


  file = va_arg (ap, char *);

  while (i-- && !(b_stream = ioctl_buffer (file, APG_OPEN)))
    file = va_arg (ap, char *);

  if (!b_stream)
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

#if !defined (__GNUC__) || defined (__STRICT_ANSI__)

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
      base_tokens = p_token = p_token_0 = (unsigned char *) xcalloc (strlen (b_stream), sizeof (char));

      p_stream = (unsigned char *) b_stream;
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
	      if ( L_OPT (apg_stream->type_line, j) && ~shift_reg & (1 << j))
		token_fatalerr (apg_stream->type_line, j,
				L_TYPE (apg_stream->type_line, j),
				APG_NULL_ERR,
				L_LOW (apg_stream->type_line, j),
				L_HIGH (apg_stream->type_line, j), NULL);


	  shift_reg = 0;	/* clear */

	  apg_stream = alloc_segment (apg_stream);
	  apg_stream->type_line = b_search (tk);

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
      if (L_OPT (apg_stream->type_line, j) && ~shift_reg & (1 << j))
	token_fatalerr (apg_stream->type_line, j,
			L_TYPE (apg_stream->type_line, j),
			APG_NULL_ERR,
			L_LOW (apg_stream->type_line, j),
			L_HIGH (apg_stream->type_line, j), NULL);


  free (base_tokens);
  ioctl_buffer (NULL, APG_CLOSE);

  apg_flags |= APG_PARSED_GRILL;

  if (apg_flags & APG_FATALERR)
    fatalerr ("apg_parser(): encontered some errors");


  return apg_arena;

}


