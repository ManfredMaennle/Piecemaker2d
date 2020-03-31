
/*
 * in_parse.y -- a parser for input files defining pieces
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


%{ /**************** C declarations ***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* NEW2018 */

#include "in_lex.h"

#include "rect.h"
#include "rctarray.h"
#include "intvect.h"
#include "strres.h"
#include "main.h"

/* this file provides definitions for the declarations in */
#include "in_prs_h.h"

rectarray *sizevector = NULL;
intvect *piecevector = NULL;

/*** local declarations for this file ***/

#ifdef YYDEBUG
  int in_debug=1;
#endif

int  in_error(char *);


%} /**************** parser declarations **********************************/

%token T_S    /* SEMIKOLON is separator */
%token T_EOL  /* END OF LINE */
%token T_STR
%token T_EOF


%% /***************** grammer rules and actions ************************/

input: empty {
#ifdef TRACE_YACC
  fprintf(stderr, "{input: empty}\n");
#endif
  fprintf(stderr, strRes(IN_PARSE_FILE_EMPTY));
  YYACCEPT;
} | lines T_EOF { 
#ifdef TRACE_YACC
  fprintf(stderr, "{input: lines T_EOF}\n");
#endif
  YYACCEPT; 
};
 
lines: empty {
#ifdef TRACE_YACC
  fprintf(stderr, "{lines: empty}\n");
#endif
  $$ = NULL; 
} | lines line {
#ifdef TRACE_YACC
  fprintf(stderr, "{lines: lines line}\n");
#endif
  $$ = NULL; 
};  

line: T_STR T_S T_STR T_S T_STR T_S T_STR T_S T_STR T_S T_STR T_S comment T_EOL {
int n = 0;
int xx = 0;
int yy = 0;
#ifdef TRACE_YACC
  fprintf(stderr, "{line: T_STR T_S T_STR T_S T_STR T_S T_STR T_S T_STR T_S T_STR T_S comment T_EOL}\n");
#endif
  if (strcmp(material_name, $5) != 0) {/* line contains raw material's name? */
    ; /* skip line*/
  }
  else { /* found line defining a piece */
    if (n_pieces >= max_n_pieces) {
      fprintf(stderr, strRes(IN_PARSE_ERROR_TOO_MANY_PIECES), prgname, max_n_pieces);
      return 3;
    }
    n = atoi($7);
    xx = atoi($9);
    yy = atoi($11);
    if ( (n==0) || (xx==0) || (yy==0) ) {
      fprintf(stderr, strRes(IN_PARSE_ERROR_ZERO_VALUES), prgname, in_lineno);
      return 3;
    }
    sizevector->name[n_pieces] = $1;
    piecevector->el[n_pieces] = n;
    sizevector->rectangular[n_pieces] = newrect(xx, yy);
    n_pieces++;
    if (verbose > 1) {
      fprintf(stderr, "%s %s %s %s %s %s\n", $1, $3, $5, $7, $9, $11); 
    }
  }
  $$ = NULL;
};

comment: empty {   /* rest of the line is treated as comment, i.e. everything but END_OF_LINE */
#ifdef TRACE_YACC
  fprintf(stderr, "{comment: empty}\n");
#endif
  $$ = NULL; 
} | T_S comment {
#ifdef TRACE_YACC
  fprintf(stderr, "{comment: T_S comment}\n");
#endif
  $$ = NULL; 
}  | T_STR comment {
#ifdef TRACE_YACC
  fprintf(stderr, "{comment: T_STR comment}\n");
#endif
  $$ = NULL; 
};

empty: /*** empty ***/
;


%% /****************** additional C code ************************/

int in_error(char *s) /* called by in_parse() on error */
{
  if (in_lval == NULL) {
    in_lval = "";
  }
  fprintf(stderr, strRes(IN_PARSE_PARSE_ERROR), prgname, in_lineno, in_lval);
  return 99;
}

