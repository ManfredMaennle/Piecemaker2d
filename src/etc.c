
/*
 * etc.c -- diverse help functions
 *
 * Copyright (c) 1999, 2020 Manfred Maennle, Karlsruhe/Kandel, Germany
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * $Id:$
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#include "etc.h"



char *strnsave(const char *str, int n)
{
  char *s;
  if (str == NULL)
    return NULL;
  s = pccmalloc(stderr, "malloc", ((n+1) * (sizeof str[0])));
  strncpy(s, str, n);
  s[n] = '\0';
  return s;
}


void strnfill(char *str, char c, int n) {
  int k;
  for (k=0; k<n; ++k) {
    *(str++) = c;
  }
  return;
}


void *pccmalloc(FILE *errfile, const char *prgname, size_t size)
{
  void *memory = malloc(size);
  if (memory==NULL) {
    fprintf(errfile, "%s: fatal error: insufficient memory, malloc failed!\n", prgname);
    leave(-1, NULL, NULL);
  }
  return memory;
}


unsigned int to_cmp_iso(unsigned char c)
{
  /* shifts input by 256; so, shifted values are recognisable */
  if (c>255) /* do nothing with already treated characters */
    return c;
  if ( (c>64) && (c<91) )
    c += 32; /* to lower */
  if (c<123)
    return 256+2*c;
  if ( (c>122) && (c<127) ) /* treat '{', '|', '}', '~' like space */ 
    return 256+2*20; /* ' ' == 20 */
  /* c>127: ISO-8859-1 extension of ASCII code */
  if ( (c>191) && (c<223) )
    c += 32; /* to lower */
  if (c == 223) /* return value between 's' and 't' */
    return 256+2*115+1; /* 's' == 115 */
  if ( (c>223) && (c<231) )
    return 256+2*97+1; /* 'a' == 97 */
  if (c == 231) /* c cedille */
    return 256+2*99+1; /* 'c' == 99 */
  if ( (c>231) && (c<236) )
    return 256+2*101+1; /* 'e' == 101 */
  if ( (c>235) && (c<240) )
    return 256+2*105+1; /* 'i' == 105 */
  if (c == 240)
    return 256+2*100+1; /* 'd' == 100 */
  if (c == 241)
    return 256+2*110+1; /* 'n' == 110 */
  if ( (c>241) && (c<249) )
    return 256+2*111+1; /* 'o' == 111 */
  if ( (c>248) && (c<253) )
    return 256+2*117+1; /* 'u' == 117 */
  if ( (c>252) && (c<256) )
    return 256+2*121+1; /* 'y' == 121 */
  return 256+2*c;
}


unsigned char to_letter_iso(unsigned char c)
{
  if ((c>64) && (c<91))
    return c;
  if ((c>96) && (c<123))
    return (c-32);
  /* c>127: ISO-8859-1 extension of ASCII code */
  if ( (c>191) && (c<223) )
    c += 32; /* to lower */
  if (c == 223)
    return 'S';
  if ( (c>223) && (c<231) )
    return 'A';
  if (c == 231) /* c cedille */
    return 'C';
  if ( (c>231) && (c<236) )
    return 'E';
  if ( (c>235) && (c<240) )
    return 'I';
  if (c == 240)
    return 'D';
  if (c == 241)
    return 'N';
  if ( (c>241) && (c<249) )
    return 'O';
  if ( (c>248) && (c<253) )
    return 'U';
  if ( (c>252) && (c<256) )
    return 'Y';
  return ' ';
}


