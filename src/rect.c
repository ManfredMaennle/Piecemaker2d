
/*
 * rect.c -- rectangulars
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
#include "etc.h"
#include "main.h"

#include "rect.h"

rect * newrect(int xx, int yy)
{
  rect *newnode;
  newnode = pccmalloc(stderr, "in newrect()", sizeof(rect));
  newnode->x = xx;
  newnode->y = yy;
  return newnode;
}


void copyrect(rect *r, int xx, int yy)
{
  r->x = xx;
  r->y = yy;
  return;
}


int rect_surface(const rect *r)
{
  if (r == NULL)
    return 0;
  else
    return (r->x * r->y);
}

void rect_dump(FILE *outfile, const rect *r)
{
  if (r == NULL)
    fprintf(outfile, "[NULL]");
  else
    fprintf(outfile, "[%dx%d]", r->x, r->y);
  return;
}

/* transposes its argument! */
void rect_transpose(rect *r)
{
  int tmp;
  if (r == NULL)
    return;
  tmp = r->x;
  r->x = r->y;
  r->y = tmp;
  return;
} 

/* returns 1 if r1 greater than r2 */
int rect_gt(rect *r1, rect *r2) {
  if (r1->x > r2->x)
    return 1;
  if ( (r1->x == r2->x) && (r1->y > r2->y) )
    return 1;
  return 0;
}
 
/* returns 1 if r1 less than r2 */
int rect_lt(rect *r1, rect *r2) {
  if (r1->x < r2->x)
    return 1;
  if ( (r1->x == r2->x) && (r1->y < r2->y) )
    return 1;
  return 0;
}
 

/* fit a rectangular piece in a rectangular original; 
   return rests in rest1..rest4; also return
   0 if no doesn't fit, 
   1 if fits exactly,
   2 if one horizontal cut needed,
   3 if one vertical cut needed,
   4 if a horizontal and a vertical cut needed (two possibilities)
*/
int rect_fit(const rect *original, const rect *piece, 
	     rect **rest1, rect **rest2, rect **rest3, rect **rest4)
{
  int dx = original->x - piece->x;
  int dy = original->y - piece->y;
  if ( (dx < 0) || (dy < 0) ) {
    return 0;
  }
  if (dx == 0) {
    if (dy == 0) {
      /* exact fit */
      return 1;
    }
    else { /* dy > 0 */
      /* need a horizontal cut */
      copyrect(*rest1, original->x, intfloor(dy-cutwidth));
      return 2;
    }
  }
  else { /* dx > 0 */
    if (dy == 0) {
      /* need a vertical cut */
      copyrect(*rest1, intfloor(dx-cutwidth), original->y);
      return 3;
    }
    else { /* dy > 0 */
      /* need a horizontal AND a vertical cut */
      /* first possibility: first cut vertically */
      copyrect(*rest1, piece->x, intfloor(dy-cutwidth));
      copyrect(*rest2, intfloor(dx-cutwidth), original->y);
      /* second possibility: first cut horizontally */
      copyrect(*rest3, intfloor(dx-cutwidth), piece->y);
      copyrect(*rest4, original->x, intfloor(dy-cutwidth));
      return 4;
    }
  }
  return -1;
}

