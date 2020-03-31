#ifndef CUT_H
#define CUT_H

/*
 * cut.h -- common declarations of a tree to store cuts
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

typedef struct cutnodestruct cutnode; 
struct cutnodestruct {
  char direction;
  int piece;
  cutnode *left;   /* or upper */
  cutnode *right;  /* or lower */
};

typedef cutnode cut;

/* append element to existing list; return ptr to new node */
cut * cut_insertback(cut *left, cut *right, char direction, int piece);

/* delete list pointer structure and all elements */
void cut_delete(cut *tree);

int cut_treedepth(const cut *tree);

/* returns height of graphic */
int cut_print(FILE *outfile, const cut *tree);

void pieces_print(FILE *outfile, const rectarray *rct, const intvect *ivect);

void cut_dump(FILE *outfile, const cut *tree);

#endif /* #ifndef CUT_H */

