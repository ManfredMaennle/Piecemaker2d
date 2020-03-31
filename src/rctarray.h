#ifndef RECTARRAY_H
#define RECTARRAY_H

/*
 * rctarray.h -- array of rectangulars
 * 
 * Copyright (c) 1999, 2020 Manfred Maennle, Karlsruhe/Kandel, Germany
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom tSoftware is furnished to do so, subject to
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
 *   "rect.h"
 */

typedef struct rectarraynodestruct rectarraynode; 
struct rectarraynodestruct {
  int  length;
  rect **rectangular;
  char **name;
};

typedef rectarraynode rectarray;


rectarray * newrectarray(int length);
void copyrectarray(rectarray *r1, const rectarray *r2);
void rectarray_dump(FILE *outfile, const rectarray *ra);
int rectarray_check(FILE *outfile, const rectarray *ra);



#endif /* #ifndef RECTARRAY_H */

