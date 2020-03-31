
/*
 * cf_parse.y -- a parser for the configuration file
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
#include "cf_lex.h"

#include "strres.h"
#include "main.h"

/* this file provides definitions for the declarations in */
#include "cf_prs_h.h"


/* name of config file */
char *configfilename = NULL;

/* search depth, limit to search steps per trial */
int max_fitted_trials = 3;

/* name of input and output file */
char *infilename = NULL;
char *outfilename = NULL;

/* name and size of raw material */
char *material_name = NULL;
int material_x = 0;
int material_y = 0;

/* name of program to call at the end of piecemaker2d */
char *prgcall = NULL;

/* no of directions: one or two (default: 1) */
int n_directions = 1;

/* width of the saw */
int cutwidth = 4;

/* additional space (width) to long cuts */
int cut_addwidth = 0;

/* formfeed: 0 == no formfeed */
int formfeed = 0;

/* verbose level: 0 == no output */
int verbose = 1;


/*** local declarations for this file ***/

#ifdef YYDEBUG
  int cf_debug = 1;
#endif

int  cf_error(char *);


%} /**************** parser declarations **********************************/

%token T_EQ   /* equal sign */
%token T_STR
%token T_EOF

%token T_MAX_FITTED_TRIALS
%token T_CONFIGFILENAME
%token T_INFILENAME
%token T_OUTFILENAME
%token T_MATERIAL_NAME
%token T_MATERIAL_X
%token T_MATERIAL_Y
%token T_N_DIRECTIONS
%token T_CUTWIDTH
%token T_CUT_ADDWIDTH
%token T_PRINT_SCALE
%token T_FORMFEED
%token T_VERBOSE
%token T_PRGCALL


%% /***************** grammer rules and actions ************************/

input: empty {
#ifdef TRACE_YACC
  fprintf(stderr, "{input: empty}\n");
#endif
  fprintf(stderr, strRes(CF_PARSE_FILE_EMPTY));
  YYACCEPT;
} | configs T_EOF { 
#ifdef TRACE_YACC
  fprintf(stderr, "{input: configs T_EOF}\n");
#endif
  YYACCEPT; 
};
 
configs: empty {
#ifdef TRACE_YACC
  fprintf(stderr, "{configs: empty}\n");
#endif
  $$ = NULL; 
} | configs config {
#ifdef TRACE_YACC
  fprintf(stderr, "{configs: config configs}\n");
#endif
  $$ = NULL; 
};  

config: T_MAX_FITTED_TRIALS T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_MAX_FITTED_TRIALS T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    max_fitted_trials = atoi($3);
  }
  $$ = NULL;
} | T_CONFIGFILENAME T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_CONFIGFILENAME T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    if (configfilename == NULL) {
      configfilename = $3;
    }
  }
  $$ = NULL; 
} | T_INFILENAME T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_INFILENAME T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    infilename = $3;
  }
  $$ = NULL;
} |  T_OUTFILENAME T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_OUTFILENAME T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    outfilename = $3;
  }
  $$ = NULL;
} | T_MATERIAL_NAME T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_MATERIAL_NAME T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    material_name = $3;
  }
  $$ = NULL;
} | T_MATERIAL_X T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_MATERIAL_X T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    material_x = atoi($3);
  }
  $$ = NULL;
} | T_MATERIAL_Y T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_MATERIAL_Y T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    material_y = atoi($3);
  }
  $$ = NULL;
} | T_N_DIRECTIONS T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_N_DIRECTIONS T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    n_directions = atoi($3);
  }
  $$ = NULL;
} | T_CUTWIDTH T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_CUTWIDTH T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    cutwidth = atoi($3);
  }
  $$ = NULL;
} | T_CUT_ADDWIDTH T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_CUT_ADDWIDTH T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    cut_addwidth = atoi($3);
  }
  $$ = NULL;
} | T_PRINT_SCALE T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_PRINT_SCALE T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    print_scale = atoi($3);
  }
  $$ = NULL;
} | T_FORMFEED T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_FORMFEED T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    formfeed = atoi($3);
  }
  $$ = NULL;
} | T_VERBOSE T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_VERBOSE T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    verbose = atoi($3);
  }
  $$ = NULL;
} | T_PRGCALL T_EQ T_STR {
#ifdef TRACE_YACC
  fprintf(stderr, "{config: T_PRGCALL T_EQ T_STR}\n");
#endif
  if ($3 != NULL) {
    prgcall = $3;
  }
  $$ = NULL;
};


empty: /*** empty ***/
;


%% /****************** additional C code ************************/

int cf_error(char *s) /* called by cf_parse() on error */
{
  if (cf_lval == NULL) {
    cf_lval = "";
  }
  fprintf(stderr, strRes(CF_PARSE_PARSE_ERROR), prgname, cf_lineno, cf_lval);
  return 99;
}

