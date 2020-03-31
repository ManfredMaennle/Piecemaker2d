/* 
 * bbsort.c -- bubble sort
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

#include <stdio.h>   /* fprintf(), FILE *, ... */

#include "revision.h"
#include "rect.h"
#include "rctarray.h"
#include "intvect.h"

#include "main.h"

/* this file provides definitions for declarations in */
#include "bbsort.h"


int bbsort(rectarray *r, intvect *p) {
  int j, k;
  int xx, yy, nn;
  char* cc;
  if ( (r == NULL) || (p == NULL) )
    return 1;
  if (r->length != p->length)
    return -1;
  for (j=r->length-1; j>0; --j)
    for (k=0; k<j; ++k) {
      if (rect_lt(r->rectangular[k], r->rectangular[k+1])) { 
	/* swap the elements */
	xx = r->rectangular[k]->x;
	yy = r->rectangular[k]->y;
	cc = r->name[k];
	nn = p->el[k];
	r->rectangular[k]->x = r->rectangular[k+1]->x;
	r->rectangular[k]->y = r->rectangular[k+1]->y;
	r->name[k] = r->name[k+1];
	p->el[k] = p->el[k+1];
	r->rectangular[k+1]->x = xx;
	r->rectangular[k+1]->y = yy;
	r->name[k+1] = cc;
	p->el[k+1] = nn;
      }
    }
  return 0;
}
