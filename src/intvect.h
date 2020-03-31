#ifndef INTVECT_H
#define INTVECT_H

/*
 * intvect.h -- array of integers
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


typedef struct intvectnodestruct intvectnode; 
struct intvectnodestruct {
  int length;
  int *el;
};

typedef intvectnode intvect;


intvect * newintvect(int length);
intvect * newcopyintvect(const intvect *vector);
void copyintvect(intvect *v1, const intvect *v2);

void sub_intvect(intvect *v1, const intvect *v2, const intvect *v3);
int  null_intvect(const intvect *v1);
int  sum_intvect(const intvect *v1);
int  div_intvect(const intvect *v1, const intvect *v2);
void mult_intvect(intvect *v1, const int i);

void intvect_dump(FILE *outfile, const intvect *vector);
void intvect_delete(intvect *v);



#endif /* #ifndef INTVECT_H */

