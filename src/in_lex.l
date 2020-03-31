
/*
 * in_lex.l -- a lexical analyzer
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
#include "etc.h"

#include "in_lex.h"

/* declarations of the grammer parser */
#include "in_parse.h" 

/* local declarations for this file */

%} /**************** analyzer declarations *********************************/

%option prefix="in_"
%option noyywrap
%option nounput
%option stack 
%option lex-compat

%% /**************** analyzer rules and actions ****************************/
 
<INITIAL>^[ \t\r]*\n /* eat up empty lines */

<INITIAL>";" { /* SEMICOLON is separator */
#ifdef TRACE_LEX
  fprintf(stderr, "[T_S]\n");
#endif
in_lval = ";";
return T_S;
if (0) { /* just to avoid compiler warnings "label `find_rule' not used" and "unused functions" */
	yy_push_state(INITIAL);
    yy_top_state();
    yy_pop_state();
	yy_flex_strlen("");
    input();
    goto find_rule;
  }
}

<INITIAL>\n { /* NEWLINE is END_OF_LINE */
#ifdef TRACE_LEX
  fprintf(stderr, "[T_EOL]\n");
#endif
in_lval = "\n";
return T_EOL;
}

<INITIAL>[^;\n]+ { /* any string without SEMICOLON and NEWLINE */
#ifdef TRACE_LEX
  fprintf(stderr, "[T_STR: %s]\n", in_text);
#endif
in_lval = strnsave(in_text, in_leng);
return T_STR;
}

<INITIAL><<EOF>> { 
#ifdef TRACE_LEX
  fprintf(stderr, "[T_EOF]\n");
#endif
in_lval = NULL;
return T_EOF; 
}





%% /**************** additional C code ************************************/

/* none */
