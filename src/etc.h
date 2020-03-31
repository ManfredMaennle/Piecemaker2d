#ifndef ETC_H
#define ETC_H

/*
 * etc.h -- common declarations of diverse help funtions
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

#ifdef SHORT_CASE4
#ifndef SHORT_CASE3
#define SHORT_CASE3
#endif
#endif

/* maximum of two integers */
static inline int max(int a, int b)
{
  if (a > b)
    return a;
  return b;
}


/* floor function on integer: returns x if x > 0, otherwise 0 */
static inline int intfloor(int x) 
{
  if (x > 0) {
    return x;
  }
  return 0;
}


/* save str: copy str to allocated memory */
char *strnsave(const char *str, int n);


/* fill str n times with given char */
void strnfill(char *str, char c, int n);


/*
 * post condition checked malloc() 
 * post condition: return value != NULL 
 */
void *pccmalloc(FILE *errfile, const char *prgname, size_t size);


/* 
 * adapt ISO 8859-1 characters (with accentuated chars) for comparison 
 * treat upper case characters like lower case characters 
 * treat accentuated characters (e.g., umlauts) like plain characters 
 * treat {|}~ like space, treat ssharp like between 's' and 't'
 */
unsigned int to_cmp_iso(unsigned char c);


/* 
 * adapt ISO 8859-1 characters (with accentuated chars) for 'letter generation'
 * returns the corresponding upper letter, e.g., "A" for "a" or "umlaut a"
 * treat accentuated characters like plain characters, ssharp like 's' 
 * returns a space ' ' for all other characters 
 */
unsigned char to_letter_iso(unsigned char c);




#endif /* #ifndef ETC_H */

