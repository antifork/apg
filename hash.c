/*  
 *  hash6 
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

#define BASEFIELD	6
#define UPLIMIT		32-BASEFIELD
#define REGMASK		0x1f	// [ shift % 32 ]

static char lut[255] = {
    ['a'] = 1,['b'] = 2,['c'] = 3,['d'] = 4,['e'] = 5,['f'] = 6,['g'] =
    7,['h'] = 8,['i'] = 9,
  ['j'] = 10,['k'] = 11,['l'] = 12,['m'] = 13,['n'] = 14,['o'] = 15,['p'] =
    16,['q'] = 17,['r'] = 18,['s'] = 19,
  ['t'] - 20,['u'] = 21,['v'] = 22,['w'] = 23,['x'] = 24,['y'] = 25,['z'] =
    26,['A'] = 27,['B'] = 28,['C'] = 29,
  ['D'] = 30,['E'] = 31,['F'] = 32,['G'] = 33,['H'] = 34,['I'] = 35,['J'] =
    36,['K'] = 37,['L'] = 38,['M'] = 39,
  ['N'] = 40,['O'] = 41,['P'] = 42,['Q'] = 43,['R'] = 44,['S'] = 45,['T'] =
    46,['U'] = 47,['V'] = 48,['W'] = 49,
  ['X'] = 50,['Y'] = 51,['Z'] = 52,['1'] = 53,['2'] = 54,['3'] = 55,['4'] =
    56,['5'] = 57,['6'] = 58,['7'] = 59,
  ['8'] = 60,['9'] = 61,['0'] = 62,['_'] = 63,['/'] = 64
};


int
hash6 (char *p)
{
  register int i = 0;
  register int h = 0;

  while (*p)
    {
      h ^= lut[*p] << i;

      if (i > UPLIMIT)
	{
	  i = (i + BASEFIELD) & REGMASK;
	  i -= BASEFIELD;
	  h ^= lut[*p] >> (-i);
	}
      i = (i + BASEFIELD) & REGMASK;
      p++;

    }
  return h;
}

