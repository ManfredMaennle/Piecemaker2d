
/*
 * cf_lex.l -- a lexical analyzer for the configuration file
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

#include "cf_lex.h"

/* declarations of the grammer parser */
#include "cf_parse.h" 

/* local declarations for this file */

%} /**************** analyzer declarations *********************************/

%option prefix="cf_"
%option noyywrap
%option nounput
%option stack 
%option lex-compat

%x COMMENT

%% /**************** analyzer rules and actions ****************************/
 
<INITIAL>"=" {
return T_EQ;
if (0) { /* just to avoid compiler warnings "label `find_rule' not used" and "unused functions" */
    yy_top_state();
    yy_flex_strlen("");
    input();
    goto find_rule;
  }
}

<INITIAL>"SUCHTIEFE" {
return T_MAX_FITTED_TRIALS;
}
<INITIAL>"MAX_TRIALS" {
return T_MAX_FITTED_TRIALS;
}

<INITIAL>"KONFIGURATIONSDATEI" {
return T_CONFIGFILENAME;
}
<INITIAL>"CONFIG_FILENAME" {
return T_CONFIGFILENAME;
}

<INITIAL>"EINGABEDATEI" {
return T_INFILENAME;
}
<INITIAL>"INPUT_FILENAME" {
return T_INFILENAME;
}

<INITIAL>"AUSGABEDATEI" {
return T_OUTFILENAME;
}
<INITIAL>"OUTPUT_FILENAME" {
return T_OUTFILENAME;
}

<INITIAL>"MATERIAL" {
return T_MATERIAL_NAME;
}
<INITIAL>"MATERIAL_NAME" {
return T_MATERIAL_NAME;
}

<INITIAL>"MATERIALLAENGE" {
return T_MATERIAL_X;
}
<INITIAL>"MATERIAL_LENGTH" {
return T_MATERIAL_X;
}

<INITIAL>"MATERIALBREITE" {
return T_MATERIAL_Y;
}
<INITIAL>"MATERIAL_WIDTH" {
return T_MATERIAL_Y;
}

<INITIAL>"RICHTUNGEN" {
return T_N_DIRECTIONS;
}
<INITIAL>"CUTTING_N_DIRECTIONS" {
return T_N_DIRECTIONS;
}

<INITIAL>"BLATTDICKE" {
return T_CUTWIDTH;
}
<INITIAL>"CUTTING_WIDTH" {
return T_CUTWIDTH;
}

<INITIAL>"SCHNITTZUGABE" {
return T_CUT_ADDWIDTH;
}
<INITIAL>"CUTTING_ALLOWANCE" {
return T_CUT_ADDWIDTH;
}

<INITIAL>"DRUCKSKALIERUNG" {
return T_PRINT_SCALE;
}
<INITIAL>"PRINT_SCALE" {
return T_PRINT_SCALE;
}

<INITIAL>"INFO" {
return T_VERBOSE;
}
<INITIAL>"VERBOSE" {
return T_VERBOSE;
}

<INITIAL>"SEITENVORSCHUB" {
return T_FORMFEED;
}
<INITIAL>"FORM_FEED" {
return T_FORMFEED;
}

<INITIAL>"AUFRUF" {
return T_PRGCALL;
}
<INITIAL>"PROGRAM_CALL_ON_EXIT" {
return T_PRGCALL;
}


<INITIAL>[0-9a-zA-Z_():/\\\.+-]+ {
#ifdef TRACE_LEX
  fprintf(stderr, "[T_STR: %s]\n", cf_text);
#endif
cf_lval = strnsave(cf_text, cf_leng);
return T_STR;
}

<INITIAL>\"[^"\n]+\" {
#ifdef TRACE_LEX
  fprintf(stderr, "[T_STR: %s]\n", cf_text);
#endif
cf_lval = strnsave(cf_text + 1, cf_leng - 2);
return T_STR;
}


<INITIAL>"/*" {
#ifdef TRACE_LEX
  fprintf(stderr, "[BEGIN COMMENT]\n/*");
#endif
yy_push_state(COMMENT);
}

<INITIAL><<EOF>> { 
#ifdef TRACE_LEX
  fprintf(stderr, "[T_EOF]\n");
#endif
return T_EOF; 
}

<INITIAL>\n /* eat up newlines */

<INITIAL>[ \t\r]+ /* eat up white space */

<INITIAL>^REM.*\n /* eat up comment lines */
<INITIAL>^Rem.*\n /* eat up comment lines */
<INITIAL>^rem.*\n /* eat up comment lines */


<COMMENT>[^*]+ { /* eat up anything that's not a '*' */
#ifdef TRACE_LEX
  fprintf(stderr, "%s", cf_text); 
#endif  
}

<COMMENT>"*"+[^*/] { /* eat up '*'s not followed by '/' */
#ifdef TRACE_LEX
  fprintf(stderr, "%s", cf_text); 
#endif  
}

<COMMENT>"*"+"/" {
#ifdef TRACE_LEX
  fprintf(stderr, "%s\n", cf_text); 
  fprintf(stderr, "[END COMMENT, return to state '%d']\n", yy_top_state()); 
#endif
yy_pop_state();
}


%% /**************** additional C code ************************************/

/* none */
