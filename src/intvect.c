
/*
 * intvect.c -- array of integers
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
#include <limits.h>
#include "etc.h"
#include "rect.h"

#include "intvect.h"


intvect * newintvect(int len)
{
  /* int k; */
  intvect *newnode;
  int *newvector;
  newnode = pccmalloc(stderr, "in newintvect():1", sizeof(intvect));
  newnode->length = len;
  newvector =  pccmalloc(stderr, "in newintvect():2", len * sizeof(int));
  newnode->el = newvector;
  /*
  for (k=0; k<newnode->length; ++k) {
    newnode->el[k] = 0;
  }
  */
  return newnode;
}


intvect * newcopyintvect(const intvect* vector)
{
  int k; 
  intvect *newnode;
  int *newvector;
  if (vector == NULL)
    return NULL;
  newnode = pccmalloc(stderr, "in newcopyintvect():1", sizeof(intvect));
  newnode->length = vector->length;
  newvector =  pccmalloc(stderr, "in newcopyintvect():2", 
			 vector->length * sizeof(int));
  newnode->el = newvector;
  for (k=0; k<newnode->length; ++k) {
    newnode->el[k] = vector->el[k];
  }
  return newnode;
}


void copyintvect(intvect *v1, const intvect *v2)
{
  int k;
  if (v2 == NULL) {
    v1 = NULL;
  }
  else {
    for (k=0; k<v2->length; ++k) {
      v1->el[k] = v2->el[k];
    }
  }
  return;
}

void sub_intvect(intvect *v1, const intvect *v2, const intvect *v3)
{
  int k;
  if ((v2 == NULL) || (v3 == NULL)) {
    v1 = NULL;
  }
  else {
    for (k=0; k<v2->length; ++k) {
      v1->el[k] = v2->el[k] - v3->el[k];
    }
  }
  return;
}

int null_intvect(const intvect *v1) 
{
  int k;
  if (v1 == NULL) {
    return 1;
  }
  for (k=0; k<v1->length; ++k) {
    if (v1->el[k] != 0) {
      return 0;
    }
  }
  return 1;
}

int sum_intvect(const intvect *v1) 
{
  int k;
  int sum = 0;
  if (v1 == NULL) {
    return 0;
  }
  for (k=0; k<v1->length; ++k) {
    sum += v1->el[k];
  }
  return sum;
}

int  div_intvect(const intvect *v1, const intvect *v2) 
{
  int k;
  int d = INT_MAX;
  int min_d = INT_MAX; /* return negative value for undefined */
  if ((v1 == NULL) || (v2 == NULL)) {
    return -1;
  }
  for (k=0; k<v1->length; ++k) {
    if (v1->el[k] == 0) {
      if (v2->el[k] == 0) {
	/* ok */
      }
      else {
	d = -2; /* undefined: 0 / x with x!=0 */
      }
    }
    else {
      if (v2->el[k] == 0) {
	/* ok */
      }
      else {
	d = v1->el[k] / v2->el[k];
      }
    }
    if (d < min_d) {
      min_d = d;
    }
  }
  return min_d;
}

void mult_intvect(intvect *v1, const int i) 
{
  int k;
  if (v1 == NULL) {
    return;
  }
  else {
    for (k=0; k<v1->length; ++k) {
      v1->el[k] *= i;
    }
  }
  return;
}

void intvect_dump(FILE *outfile, const intvect *v)
{
  int k;
  fprintf(outfile, "[");
  if (v == NULL) {
    fprintf(outfile, "]");
    return;
  }
  if (v->length > 0) {
    fprintf(outfile, "%d", v->el[0]);
  }
  for (k=1; k<v->length; ++k) {
    fprintf(outfile, ", %d", v->el[k]);
  }
  fprintf(outfile, "]");
  return;
}


void intvect_delete(intvect *v)
{
  if (v == NULL)
    return;
  if (v->el != NULL) {
    free(v->el);
  }
  free(v);
  return;
}


/*
intvect * intvect_insertback(intvect *list, symblist *element)
{
  intvect *newnode;
  if (element == NULL)
    return list;
  newnode = pccmalloc(stderr, "in intvect_insertback()", sizeof(intvect));
  newnode->el = element;
  if (list == NULL)
    newnode->next = NULL;
  else {
    newnode->next = list->next;
    list->next = newnode;
  }
  return newnode;
}


extern void intvect_delete(intvect *list, int delete_elements)
{
  intvect *tail;
  if (list == NULL)
    return;
  list = list;
  tail = list->next;
  while (tail != NULL) {
    if (delete_elements)
      free(list->el);
    free(list);
    list = tail;
    tail = tail->next;
  }
  if (delete_elements)
    free(list->el);
  free(list);
  return;
}

*/
