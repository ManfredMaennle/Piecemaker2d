
/*
 * rectarray.c -- array of rectangulars
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
#include <string.h> /* NEW2018 */

#include "etc.h"
#include "strres.h"
#include "rect.h"
#include "main.h"

#include "rctarray.h"


rectarray * newrectarray(int len)
{
  int k;
  rectarray *newnode;
  rect **newrectpvector;
  char **newcharpvector;
  newnode = pccmalloc(stderr, "in newrectarray():1", sizeof(rectarray));
  newnode->length = len;
  newrectpvector =  pccmalloc(stderr, "in newrectarray():2", 
			      len * sizeof(rect *));
  newnode->rectangular = newrectpvector;
  newcharpvector =  pccmalloc(stderr, "in newrectarray():3", 
			      len * sizeof(char *));
  newnode->name = newcharpvector;
  for (k=0; k<newnode->length; ++k) {
    newnode->rectangular[k] = NULL;
    newnode->name[k] = NULL;
  }
  return newnode;
}


void copyrectarray(rectarray *r1, const rectarray *r2) {
  int k;
  int len;
  char *ncharvect;
  rect *nrect;
  if (r2 == NULL) {
    r1 = NULL;
  }
  if (r1->length != r2->length) {
    leave(-1, stderr, "prog. error in `copyrectarray': length1 != length2"); 
  }
  for (k=0; k<r2->length; ++k) {
    nrect = newrect(r2->rectangular[k]->x, r2->rectangular[k]->y);
    len = strlen(r2->name[k]) + 1;
    ncharvect = pccmalloc(stderr, "in copyrectarray():1", len);
    strcpy(ncharvect, r2->name[k]);
    r1->rectangular[k] = nrect;
    r1->name[k] = ncharvect;
  }
  return;
}

void rectarray_dump(FILE *outfile, const rectarray *ra)
{
  int k;
  if (ra == NULL) {
    fprintf(outfile, "[NULL]\n");
    return;
  }
  for (k=0; k<ra->length; ++k) {
    if (ra->name[k] != NULL) {
      fprintf(outfile, "<%s>", ra->name[k]);
    }
    else {
      fprintf(outfile, "<NULL>");
    }
    rect_dump(outfile, ra->rectangular[k]);
  }
  return;
}


int rectarray_check(FILE *outfile, const rectarray *ra)
{
  int k;
  int n_warnings = 0;
  if (ra == NULL) {
    return 0;
  }
  for (k=0; k<ra->length; ++k) {
    if ((ra->name[k] != NULL) && (strlen(ra->name[k]) > 0)) {
      if (strlen(ra->name[k]) > max_piece_name_length) {
	++n_warnings;
	fprintf(outfile, strRes(WARNING_PIECE_NAME_TOO_LONG), ra->name[k], max_piece_name_length);
	if (verbose > 1) {
	  fprintf(stderr, strRes(WARNING_PIECE_NAME_TOO_LONG), ra->name[k], max_piece_name_length);
	}
      }
    }
    else {
      ++n_warnings;
      fprintf(outfile, strRes(WARNING_PIECE_NAME_MISSING), k+1);
      if (verbose > 1) {
	fprintf(stderr, strRes(WARNING_PIECE_NAME_MISSING), k+1);
      }
    }
  }
  return n_warnings;
}



