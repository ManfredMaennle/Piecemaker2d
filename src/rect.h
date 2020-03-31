#ifndef RECT_H
#define RECT_H

/*
 * rect.h -- rectangulars
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

/*
 * needs to have included before:
 *   <stdio.h>
 */

typedef struct rectnodestruct rectnode; 
struct rectnodestruct {
  int x;   /* length; horizontal expansion */
  int y;   /* width; vertical expansion */
};

typedef rectnode rect;

rect * newrect(int xx, int yy);
void copyrect(rect *rectangular, int xx, int yy);
int rect_surface(const rect *r);
void rect_dump(FILE *outfile, const rect *r);
void rect_transpose(rect *r);
int rect_gt(rect *r1, rect *r2); /* greater than */
int rect_lt(rect *r1, rect *r2); /* less than */

/* fit piece in original; return >0 if possible, 0 else; return rests */
int rect_fit(const rect *original, const rect *piece, 
	     rect **rest1, rect **rest2, rect **rest3, rect **rest4);


#endif /* #ifndef RECT_H */

