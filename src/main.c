/*
 * piecemaker2d -- find good cuts of a rectangle into a set of rectangulars
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

#include <stdio.h>   /* fprintf(), FILE *, ... */
#include <string.h>  /* strcmp(), strcpy(), ... */
#include <stdlib.h>  /* qsort(), malloc(), ... */
#include <limits.h>
#include <signal.h>

#include "revision.h"
#include "strres.h"
#include "etc.h"
#include "rect.h"
#include "rctarray.h"
#include "intvect.h"
#include "cut.h"
#include "bbsort.h"
#include "frag.h"

#include "in_lex.h"
#include "in_prs_h.h"
#include "cf_lex.h"
#include "cf_prs_h.h"

/* this file provides definitions for declarations in */
#include "main.h"



const char *prgname = "piecemaker2d";


const int max_n_cuts = 30000;

const int max_config_search_depth = 65000;

int max_level;
int instruction;

/* set print_scale between 20 and 200; the larger print_scale, the larger the ASCII print-out */
int print_scale = 50; /* default; may be overwritten from config file */
int print_scale_divider_x;
int print_scale_divider_y;

const int max_piece_name_length = 4;

const int max_n_pieces = 10000;
int n_pieces = 0;
 
long n_trials_mio = 0;
int n_trials = 0;


static void print_usage(FILE *errfile, char* configfilename);
static void print_revision(FILE *errfile);
static void print_license(FILE *errfile);


/* local declarations and definitions */
FILE *outfile;


/* ******************** main() ******************** */
int main( int argc, char *argv[] ) { 

  /* local declarations and definitions */
  char* config_filename_from_command_line = NULL;
  int maximum_fitted_trials = 0;
  int optimal_fitted_trials = 0;
  int optimal_sum_originals = max_n_pieces + 1;

  const char formfeed_character = '';
  char *rcfilename = NULL;
  char *rcsuffix = ".rc";
  char *tmpstr = NULL;

  cut **computed_cuts;
  int *computed_frags;
  long *computed_n_trials_mio;
  int *computed_n_trials;
  int *computed_n_originals;

  /* command line and configuration variables */
  int commandline_error = 0;
  int config_error = 0;

  /* help variables to set scale factor depending from material_x and material_y */
  int scale_divider_x;
  int scale_divider_y;
  int scale_divider;

  /* help variables */
  intvect *scanned_pieces;
  rectarray *scanned_sizes;
  intvect *sorted_pieces;
  rectarray *sorted_sizes;
  rect *original_piece;
  int k;
  intvect *rest_pieces;
  intvect *used_pieces;
  int frag;
  int n_originals;
  int sum_frag = 0;
  double rel_frag = 0;
  int sum_originals = 0;
  cut *c = NULL;
 
  
  /* set default system language for string resources, see strres.h */
  language = ENGLISH; 
  /* language = GERMAN; */

  /* activate signal handler */
  signal(SIGINT, (*signalhandler));
  signal(SIGTERM, (*signalhandler));
  signal(SIGABRT, (*signalhandler));

  /* allocate memory */
  computed_cuts = pccmalloc(stderr, "in main():2", 
			    max_n_cuts * sizeof(cut*));
  computed_frags = pccmalloc(stderr, "in main():3", 
			     max_n_cuts * sizeof(int));
  computed_n_trials_mio = pccmalloc(stderr, "in main():4", 
				    max_n_cuts * sizeof(long));
  computed_n_trials = pccmalloc(stderr, "in main():5", 
				max_n_cuts * sizeof(int));
  computed_n_originals = pccmalloc(stderr, "in main():6", 
				   max_n_cuts * sizeof(int));

  /* scan command line for language setting*/
  k = 1;
  while ((commandline_error == 0) && (k < argc)) {
    if (strcmp(argv[k], "--l=ENGLISH") == 0) { 
      language = ENGLISH;
    }
    else if (strcmp(argv[k], "--l=GERMAN") == 0) { 
      language = GERMAN;
    }
    else if (strncmp(argv[k], "--l=", 4) == 0) { 
      fprintf(stderr, strRes(INVALID_LANGUAGE_OPTION), prgname, argv[k]);
      commandline_error = 1;
    }
    else { /* other options are scanned later */
    }
    ++k;
  }
  /* scan and process remaining command line */
  k = 1;
  while ((commandline_error == 0) && (k < argc)) {
    if (strcmp(argv[k], "-h") == 0) {
      print_usage(stderr, configfilename); 
      leave(1, NULL, NULL);
    }
    else if (strcmp(argv[k], "-C") == 0) { 
      print_license(stderr);
      leave(1, NULL, NULL);
    }
    else if (strcmp(argv[k], "-V") == 0) { 
      print_revision(stderr);
      leave(1, NULL, NULL);
    }
    else if (strncmp(argv[k], "--c=", 4) == 0) { 
      config_filename_from_command_line = argv[k] + 4;
      if (strlen(config_filename_from_command_line) > 0) { 
	/* ok, there seems to be a config file name */
	/* fprintf(stderr, "***config file --c=%s\n", config_filename_from_command_line); */
      }
      else {
	fprintf(stderr, strRes(EMPTY_CONFIG_FILENAME), prgname);
	commandline_error = 1;
      }
    }
    else if (strcmp(argv[k], "--l=ENGLISH") == 0) { 
      /* already processed, i.e. skip */ 
      /* fprintf(stderr, "***already processed --l=ENGLISH\n"); */
    }
    else if (strcmp(argv[k], "--l=GERMAN") == 0) { 
      /* already processed, i.e. skip */
      /* fprintf(stderr, "***already processed --l=GERMAN\n"); */
    }
    else { /* unknown option */
      fprintf(stderr, strRes(UNRECOGNIZED_OPTION), prgname, argv[k]);
      commandline_error = 1;
    }
    ++k;
  }
  /* print help and exit at error */
  if (commandline_error) {
    fprintf(stderr, strRes(GET_USAGE_INFORMATION), prgname);
    leave(-1, NULL, NULL);
  }

  if (config_filename_from_command_line == NULL) {
    /* open rc file */
    if (argv[0] == NULL) {
      fprintf(stderr, strRes(ERROR_INVALID_PROGRAM_NAME), prgname, argv[0]);
      leave(-1, NULL, NULL);
    }
    rcfilename = pccmalloc(stderr, "in main():7", (1+strlen(argv[0])+strlen(rcsuffix)) * sizeof(char) );
    strcpy(rcfilename, argv[0]);
    tmpstr = strstr(rcfilename, ".exe");
    if (tmpstr != NULL) {
      *tmpstr = '\0';
    }
    strcat(rcfilename, rcsuffix);
    /* fprintf(stderr, "rc: %s\n", rcfilename); return 0; */
    cf_in = fopen(rcfilename, "r");
    if (cf_in == NULL) {
      /* fprintf(stderr, "******** no rc file %s\n", rcfilename); */ 
      fprintf(stderr, strRes(ERROR_CANNOT_READ_RC_FILE), 
	      prgname, rcfilename);
      leave(-1, NULL, NULL);
    }
    /* parse rc file */
    if (cf_parse()) {
      /* exit on parse error */
      leave(-1, NULL, NULL);
    }
    /* check rc configuration */
    if ( configfilename == NULL ) {
      fprintf(stderr, strRes(ERROR_CONFIG_FILENAME_NOT_SPECIFIED), prgname);
      leave(-1, NULL, NULL);
    }
  }
  else {
    configfilename = config_filename_from_command_line;
  }

  /* read configuration file */
  if ( strcmp(configfilename, "(null)") != 0) {
    cf_in = fopen(configfilename, "r");
    if (cf_in == NULL) {
      fprintf(stderr, strRes(ERROR_CANNOT_READ_CONFIG_FILE), prgname, configfilename);
      leave(-1, NULL, NULL);
    }
    /* parse configuration file */
    if (cf_parse()) {
      /* exit on parse error */
      leave(-1, NULL, NULL);
    }
  }

  
  /* check configuration */
  if ( (max_fitted_trials < 1) || (max_fitted_trials > max_config_search_depth) ) {
    fprintf(stderr, strRes(ERROR_CONFIG_SEARCH_DEPTH), prgname, configfilename, max_fitted_trials);
    config_error = 1;
  }
  if ( infilename == NULL ) {
    fprintf(stderr, strRes(ERROR_CONFIG_CONFIG_FILENAME), prgname, configfilename);
    config_error = 1;
  }
  if ( outfilename == NULL ) {
    fprintf(stderr, strRes(ERROR_CONFIG_OUTPUT_FILENAME), prgname, configfilename);
    config_error = 1;
  }
  if ( material_name == NULL ) {
    fprintf(stderr, strRes(ERROR_CONFIG_MATERIAL_NAME), prgname, configfilename);
    config_error = 1;
  }
  if ( material_x == 0 ) {
    fprintf(stderr, strRes(ERROR_CONFIG_MATERIAL_LENGTH_MISSING), prgname, configfilename);
    config_error = 1;
  }
  if ( (material_x < 0) || (material_x > 1000000) ) {
    fprintf(stderr, strRes(ERROR_CONFIG_MATERIAL_LENGTH_INVALID), prgname, configfilename, material_x);
    config_error = 1;
  }
  if ( material_y == 0 ) {
    fprintf(stderr, strRes(ERROR_CONFIG_MATERIAL_WIDTH_MISSING), prgname, configfilename);
    config_error = 1;
  }
  if ( (material_y < 0) || (material_y > 1000000) ) {
    fprintf(stderr, strRes(ERROR_CONFIG_MATERIAL_WIDTH_INVALID), prgname, configfilename, material_y);
    config_error = 1;
  }
  if ( (n_directions < 1) || (n_directions > 2) ) {
    fprintf(stderr, strRes(ERROR_CONFIG_N_DIRECTIONS_INVALID), prgname, configfilename, n_directions);
    config_error = 1;
  }
  if ( (cutwidth < 0) || (cutwidth > 100) ) {
    fprintf(stderr, strRes(ERROR_CONFIG_CUT_WIDTH), prgname, configfilename, cutwidth);
    config_error = 1;
  }
  if ( (cut_addwidth < 0) || (cut_addwidth > 1000) ) {
    fprintf(stderr, strRes(ERROR_CONFIG_CUT_ADD_WIDTH), prgname, configfilename, cut_addwidth);
    config_error = 1;
  }
  if ( (print_scale < 1) || (print_scale > 1000000) ) {
    fprintf(stderr, strRes(ERROR_CONFIG_PRINT_SCALE), prgname, print_scale);
    config_error = 1;
  }
  if ( (verbose < 0) || (verbose > 7) ) {
    fprintf(stderr, strRes(ERROR_CONFIG_VERBOSE), prgname, configfilename, verbose);
    config_error = 1;
  }
  if (config_error > 0) {
    leave(-1, NULL, NULL);
  }


  /* set scale factor for ASCII print-out depending from material_x and material_y */
  scale_divider_x = material_x / print_scale;
  scale_divider_y = material_y / print_scale;
  if (scale_divider_y > scale_divider_x) {
    scale_divider = scale_divider_y;
  }
  else {
    scale_divider = scale_divider_x;  
  }
  if (scale_divider < 1) {
    scale_divider = 1;
  }
  print_scale_divider_x = scale_divider;
  print_scale_divider_y = (scale_divider * 3) / 2;
  /* fprintf(stderr, "print_scale_divider_x: %d\n", print_scale_divider_x); */
  /* fprintf(stderr, "print_scale_divider_y: %d\n", print_scale_divider_y); */


  /* **************** configuration ok; do the job **************** */

  original_piece = newrect(material_x, material_y);

  /* open input file, if not stdin */
  if (strcmp(infilename, "(stdin)") != 0) {
    in_in = fopen(infilename, "r");
    if (in_in == NULL) {
      fprintf(stderr, strRes(ERROR_CANNOT_OPEN_INPUT_FILE), infilename);
      leave(-1, NULL, NULL);
    }
  }
  else {
    in_in = stdin;
  }
  /* open output file, if not stdout */
  if (strcmp(outfilename, "(stdout)") != 0) {
    outfile = fopen(outfilename, "w");
    if (outfile == NULL) {
      fprintf(stderr, strRes(ERROR_CANNOT_OPEN_OUTPUT_FILE), outfilename);
      leave(-1, NULL, NULL);
    }
  }
  else {
    outfile = stdout;
  }

  /* parse input file of piece description */
  piecevector = newintvect(max_n_pieces);
  sizevector = newrectarray(max_n_pieces);
  if (in_parse()) {
    /* exit on parse error */
    leave(-1, NULL, NULL);
  }
  sizevector->length = n_pieces;
  piecevector->length = n_pieces;

  /* copy the scanned data */
  scanned_sizes = newrectarray(n_pieces);
  scanned_pieces = newintvect(n_pieces);
  copyrectarray(scanned_sizes, sizevector);
  copyintvect(scanned_pieces, piecevector);

  
  if ( (n_pieces < 1) || (rect_surface(original_piece) < 1) ) {
    /* invalid data scanned; output that data and quit */
    fprintf(outfile, strRes(OUTFILE_RAW_MATERIAL));
    fprintf(outfile, strRes(OUTFILE_NAME_WIDTH_LENGTH), material_name, material_x, material_y);
    fprintf(outfile, strRes(OUTFILE_CUTTING_ALLOWANCE), cut_addwidth);
    fprintf(outfile, strRes(OUTFILE_CUTTING_WIDTH), cutwidth);
    if (n_directions == 1) {
      fprintf(outfile, strRes(OUTFILE_CUTTING_DIRECTIONS_1));
    }
    else {
      fprintf(outfile, strRes(OUTFILE_CUTTING_DIRECTIONS_2));
    }
	fprintf(outfile, strRes(OUTFILE_MAX_SEARCH_DEPTH), maximum_fitted_trials);
    fprintf(outfile, strRes(OUTFILE_SEARCH_DEPTH), max_fitted_trials);
	fprintf(outfile, "\n\n");
    fprintf(outfile, strRes(OUTFILE_BILL_OF_MATERIAL));  /* bill of material */
    pieces_print(outfile, sizevector, piecevector);
    fprintf(outfile, "\n");
    leave(0, NULL, NULL);
  }

  /* test output */
  /*
  fprintf(outfile, "loaded input file:\n");
  fprintf(outfile, "original: ");
  rect_dump(outfile, original_piece);
  fprintf(outfile, "\n");
  fprintf(outfile, "sizevector: ");
  rectarray_dump(outfile, sizevector);
  fprintf(outfile, "\n");
  fprintf(outfile, "piecevector: ");
  intvect_dump(outfile, piecevector);
  fprintf(outfile, "\n\n");
  */


  /* sort sizevector */
  if (bbsort(sizevector, piecevector)) {
    fprintf(stderr, "%s: programming error when sorting the input sizevector!\n", prgname);
  }

  /* test output */
  /*
  fprintf(outfile, "sortierte Stueckliste aus der Eingabedatei:\n");
  pieces_print(outfile, sizevector, piecevector);
  fprintf(outfile, "\n");
  fprintf(outfile, "Ausgangsmaterial:\n");
  fprintf(outfile, "Name: \"%s\", Laenge: %d mm, Breite: %d mm\n",
	  material_name, material_x, material_y);
  fprintf(outfile, "\n");
  fprintf(outfile, "Schnittzugabe: %d mm\n", cut_addwidth);
  fprintf(outfile, "Saegeblattstaerke: %d mm\n", cutwidth);
  if (n_directions == 1) {
    fprintf(outfile, "Saegerichtung: nur wie vorgegeben.\n\n\n");
  }
  else {
    fprintf(outfile, "Saegerichtung: Drehung erlaubt.\n\n\n");
  }
  */

  /* add cut_addwidth to pieces' x and y */
  if (cut_addwidth != 0) {
    for (k=0; k<sizevector->length; ++k) {
      sizevector->rectangular[k]->x += cut_addwidth;
      sizevector->rectangular[k]->y += cut_addwidth;
    }
  }

  /* copy the sorted data */
  sorted_sizes = newrectarray(n_pieces);
  sorted_pieces = newintvect(n_pieces);
  copyrectarray(sorted_sizes, sizevector);
  copyintvect(sorted_pieces, piecevector);


  /* start search algorith */
  maximum_fitted_trials = max_fitted_trials;
  for(max_fitted_trials = 1; 
      max_fitted_trials <= maximum_fitted_trials;
      ++max_fitted_trials) {
    /* *** calculate cut instructions *** */
    rest_pieces = newintvect(piecevector->length);
    used_pieces = newintvect(piecevector->length);
    copyintvect(rest_pieces, piecevector);
    instruction = 0;
    sum_originals = 0;
    sum_frag = 0;
    while (! null_intvect(rest_pieces)) {
      if (instruction >= max_n_cuts) {
	leave(-1, stderr, strRes(ERROR_MAX_N_CUTS_EXCEEDED));
      }
      c = NULL;
      n_trials_mio = 0;
      n_trials = 0;
      max_level = 0;
      frag = fragmentation(0, original_piece, rest_pieces, &c);
      computed_frags[instruction] = frag;
      computed_cuts[instruction] = c;
      computed_n_trials_mio[instruction] = n_trials_mio;
      computed_n_trials[instruction] = n_trials;
      sub_intvect(used_pieces, piecevector, rest_pieces);
      if (sum_intvect(used_pieces) == 0) {
	fprintf(outfile, strRes(OUTFILE_ATTENTION_PIECES_THAT_CANNOT_BE_CUT));
	pieces_print(outfile, sizevector, piecevector);
	fprintf(outfile, "\n\n");
	break;
      }
      n_originals = div_intvect(piecevector, used_pieces);
      computed_n_originals[instruction] = n_originals;
      sum_originals += n_originals;
      sum_frag += n_originals * frag;
      if (verbose > 4) {
	fprintf(stderr, "cut instruction no. %d:\n", instruction+1);
	fprintf(stderr, "search depth: %d, instruction no %d, ", \
		max_fitted_trials, instruction+1);
	fprintf(stderr, "max_level: %d, n_trials: %ld mio and %d.\n", \
		max_level, n_trials_mio, n_trials);
	fprintf(stderr, "rest pieces: ");
	intvect_dump(stderr, piecevector);
	fprintf(stderr, "\n");
	fprintf(stderr, "used_pieces: ");
	intvect_dump(stderr, used_pieces);
	fprintf(stderr, "\n");
	fprintf(stderr, "n_originals: %d\n", n_originals);
	if (verbose > 6) {
	  cut_print(stderr, c);
	  fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
      }
      mult_intvect(used_pieces, n_originals);
      sub_intvect(piecevector, piecevector, used_pieces);
      copyintvect(rest_pieces, piecevector);
      /* cut_delete(c); */
      ++instruction;
    }
    if (verbose > 1) {
      fprintf(stderr, "max_fitted_trials = %d, sum_originals = %d\n",
	      max_fitted_trials, sum_originals);
    }
    if (sum_originals < optimal_sum_originals) {
      optimal_sum_originals = sum_originals;
      optimal_fitted_trials = max_fitted_trials;
    }
    sizevector = newrectarray(n_pieces);
    piecevector = newintvect(n_pieces);
    copyrectarray(sizevector, sorted_sizes);
    copyintvect(piecevector, sorted_pieces);
  } /* end for(max_fitted_trials = 1; ...) */

  /*
  fprintf(stderr, "***** optimal_fitted_trials: %d\n", optimal_fitted_trials);
  fprintf(stderr, "***** sum_originals: %d\n", sum_originals);
  fprintf(stderr, "sortierte Stueckliste aus der Eingabedatei:\n");
  pieces_print(stderr, sizevector, piecevector);
  fprintf(stderr, "sortierte Stueckliste aus der Eingabedatei:\n");
  pieces_print(stderr, sorted_sizes, sorted_pieces);
  */

  max_fitted_trials = optimal_fitted_trials;
  /* *** calculate cut instructions *** */
  rest_pieces = newintvect(piecevector->length);
  used_pieces = newintvect(piecevector->length);
  copyintvect(rest_pieces, piecevector);
  instruction = 0;
  sum_originals = 0;
  sum_frag = 0;
  while (! null_intvect(rest_pieces)) {
    if (instruction >= max_n_cuts) {
      leave(-1, stderr, strRes(ERROR_MAX_N_CUTS_EXCEEDED));
    }
    c = NULL;
    n_trials_mio = 0;
    n_trials = 0;
    max_level = 0;
    frag = fragmentation(0, original_piece, rest_pieces, &c);
    computed_frags[instruction] = frag;
    computed_cuts[instruction] = c;
    computed_n_trials_mio[instruction] = n_trials_mio;
    computed_n_trials[instruction] = n_trials;
    sub_intvect(used_pieces, piecevector, rest_pieces);
    if (sum_intvect(used_pieces) == 0) {
      fprintf(outfile, strRes(OUTFILE_ATTENTION_PIECES_THAT_CANNOT_BE_CUT));
      pieces_print(outfile, sizevector, piecevector);
      fprintf(outfile, "\n\n");
      break;
    }
    n_originals = div_intvect(piecevector, used_pieces);
    computed_n_originals[instruction] = n_originals;
    sum_originals += n_originals;
    sum_frag += n_originals * frag;
    if (verbose > 2) {
      fprintf(stderr, "cut instruction no. %d:\n", instruction+1);
      fprintf(stderr, "search depth: %d, instruction no %d, ", \
	      max_fitted_trials, instruction+1);
      fprintf(stderr, "max_level: %d, n_trials: %ld mio and %d.\n", \
	      max_level, n_trials_mio, n_trials);
      fprintf(stderr, "rest pieces: ");
      intvect_dump(stderr, piecevector);
      fprintf(stderr, "\n");
      fprintf(stderr, "used_pieces: ");
      intvect_dump(stderr, used_pieces);
      fprintf(stderr, "\n");
      fprintf(stderr, "n_originals: %d\n", n_originals);
      if (verbose > 3) {
	cut_print(stderr, c);
	fprintf(stderr, "\n");
      }
      fprintf(stderr, "\n");
    }
    mult_intvect(used_pieces, n_originals);
    sub_intvect(piecevector, piecevector, used_pieces);
    copyintvect(rest_pieces, piecevector);
    /* cut_delete(c); */
    ++instruction;
  }

  /* *** output of results *** */
  /* global result */
  fprintf(outfile, strRes(OUTFILE_N_NEEDED_RAW_MATERIAL_PIECES), sum_originals);
  if (sum_originals != 0) {
    rel_frag = 100.0 * (double)sum_frag;
    rel_frag = rel_frag / (sum_originals*(double)rect_surface(original_piece));
    fprintf(outfile, strRes(OUTFILE_OVERALL_WASTE), sum_frag, rel_frag);
  }

  /* control output: parameters */
  fprintf(outfile, strRes(OUTFILE_RAW_MATERIAL));
  fprintf(outfile, strRes(OUTFILE_NAME_WIDTH_LENGTH), material_name, material_x, material_y);
  fprintf(outfile, strRes(OUTFILE_CUTTING_ALLOWANCE), cut_addwidth);
  fprintf(outfile, strRes(OUTFILE_CUTTING_WIDTH), cutwidth);
  if (n_directions == 1) {
    fprintf(outfile, strRes(OUTFILE_CUTTING_DIRECTIONS_1));
  }
  else {
    fprintf(outfile, strRes(OUTFILE_CUTTING_DIRECTIONS_2));
  }
  fprintf(outfile, strRes(OUTFILE_MAX_SEARCH_DEPTH), maximum_fitted_trials);
  fprintf(outfile, strRes(OUTFILE_SEARCH_DEPTH), max_fitted_trials);
  fprintf(outfile, strRes(OUTFILE_PRINT_SCALE), print_scale); 
  fprintf(outfile, "\n");

  /* output scanned piece list */
  fprintf(outfile, strRes(OUTFILE_BILL_OF_MATERIAL));
  pieces_print(outfile, scanned_sizes, scanned_pieces);
  fprintf(outfile, "\n");
  rectarray_check(outfile, scanned_sizes);
  fprintf(outfile, "\n\n\n");

  /* output cuts */
  if (formfeed > 0) {
    fprintf(outfile, "%c", formfeed_character);
  }
  for (k=0; k<instruction; ++k) {
    int page_height;
    fprintf(outfile, strRes(OUTFILE_CUTTING_INSTRUCTIONS_NO), k+1);
    rel_frag = 100.0 * computed_frags[k]/(double)rect_surface(original_piece);
    fprintf(outfile, strRes(OUTPUT_CUT_INSTR_WASTE), 
	    computed_frags[k], rel_frag, computed_n_trials_mio[k], 
	    computed_n_trials[k]);
    fprintf(outfile, strRes(OUTPUT_CUT_INSTR_AMOUNT), computed_n_originals[k]);
    page_height = cut_print(outfile, computed_cuts[k]) + 6;
    fprintf(outfile, "\n\n");
    if (formfeed > 0) {
      int i;
      int additional_cuts = 1;
      while ( (k+additional_cuts < instruction) 
	      && (page_height <= formfeed) ) {
	/* cuts left to print */
	page_height += cut_print(NULL, computed_cuts[k+additional_cuts]) + 6;
	++additional_cuts;
      }
      --additional_cuts;
      for (i=0; i<additional_cuts; ++i) {
	++k;
	fprintf(outfile, strRes(OUTFILE_CUTTING_INSTRUCTIONS_NO), k+1);
	rel_frag = 100.0 * computed_frags[k]
	  / (double)rect_surface(original_piece);
	fprintf(outfile, strRes(OUTPUT_CUT_INSTR_WASTE), 
		computed_frags[k], rel_frag, computed_n_trials_mio[k], 
		computed_n_trials[k]);
	fprintf(outfile, strRes(OUTPUT_CUT_INSTR_AMOUNT), computed_n_originals[k]);
	cut_print(outfile, computed_cuts[k]);
	fprintf(outfile, "\n\n");
      }
      fprintf(outfile, "%c", formfeed_character);
    } /* end if (formfeed > 0) */
    /* cut_delete(computed_cuts[k]); */
  }

  /* leave regularly */
  leave(0, NULL, NULL);  
  /* just in case... */
  return -1; 
} /* ************************* end main() ************************* */


static void print_usage(FILE *errfile, char* configfilename) {
  fprintf(errfile, strRes(PRINT_USAGE_USAGE), prgname);
  /*
  fprintf(errfile, "Lese Konfiguration aus der Datei \"%s\"\n\n", 
	  configfilename);
  */
  fprintf(errfile, strRes(PRINT_USAGE_OPTIONS));
  fprintf(errfile, strRes(PRINT_USAGE_CONFIG_FILENAME));
  fprintf(errfile, strRes(PRINT_USAGE_COPYRIGHT));
  fprintf(errfile, strRes(PRINT_USAGE_HELP));
  fprintf(errfile, strRes(PRINT_USAGE_LANGUAGE));
  fprintf(errfile, strRes(PRINT_USAGE_VERSION));
  fprintf(errfile, "\n");
  return;
}

static void print_revision(FILE *errfile) {
  int none = 1;
  fprintf(errfile, "%s %s (%s)\n", prgname, VERSION, REVDATE);
  fprintf(errfile, "compiler directives: ");
#ifdef NDEBUG
  none = 0;
  fprintf(errfile, "NDEBUG ");
#endif
#ifdef TRACE_LEX
  none = 0;
  fprintf(errfile, "TRACE_LEX ");
#endif
#ifdef TRACE_YACC
  none = 0;
  fprintf(errfile, "TRACE_YACC ");
#endif
#ifdef YYDEBUG
  none = 0;
  fprintf(errfile, "YYDEBUG ");
#endif
#ifdef SKIP_SAME_SIZE
  none = 0;
  fprintf(errfile, "SKIP_SAME_SIZE ");
#endif
#ifdef DOS
  none = 0;
  fprintf(errfile, "DOS ");
#endif
  if (none) {
    fprintf(errfile, "(none)");
  }
  fprintf(errfile, "\n");
  return;
}

static void print_license(FILE *errfile) {
  fprintf(errfile,"%s %s (%s)\n\n", prgname, VERSION, REVDATE);
  if (! strcmp(strRes(LICENSE), "") == 0) { 
    fprintf(errfile, "%s\n", strRes(LICENSE)); 
  }
  return;
}


/* exit from program */
void leave(int returnvalue, FILE *file, const char *message) {
  if ( (file != NULL) && (message != NULL) ) {
    fprintf(file, "%s: %s", prgname, message);
  }
  if ( (returnvalue != 0) || (outfile == stdout) || (outfile == stderr) ) {
   fprintf(stderr, strRes(LEAVE_HIT_RETURN_TO_CONTINUE));
    fgetc(stdin);
  }
  if (prgcall != NULL) {
    system(prgcall);
  }
  /* ***** diff to version 2.6.1
  else {
    fprintf(stderr, "Kein weiterer Programmaufruf definiert. ");
    fprintf(stderr, "Beende %s.\n", prgname);
  }
  */
  exit(returnvalue);
}


/* signal handler */
void signalhandler(int sig) {
  if (sig == SIGINT) {
    int c = 0;
    int dummy;
    fprintf(stderr, "%s %s (%s), ", prgname, VERSION, REVDATE);
    fprintf(stderr, strRes(LICENSE));
	fprintf(stderr, "\n");
    while ( (c!='\n') && (c!=EOF) 
	    && (c!='j') && (c!='J') 
		&& (c!='y') && (c!='Y')
	    && (c!='n') && (c!='N') 
		) {
      fprintf(stderr, strRes(SIGNALHANDLER_EXIT_QUESTION));
      c = fgetc(stdin);
      /* fprintf(stderr, "`%c' `%d'\n", c, c); */
      dummy = c;
      while ( (dummy!='\n') && (dummy!=EOF) ){
	/* eat up the rest of the input line */
	dummy = getchar();
      }
    }
    if ( (c=='\n') || (c==EOF) || (c=='j') || (c=='J') || (c=='y') || (c=='Y')) {
      /* ***** diff to version 2.6.1
      fprintf(stderr, "Ok, verlasse %s...\n", prgname);
      */
      leave(0, NULL, NULL);
    }
    fprintf(stderr, strRes(SIGNALHANDLER_CONTINUE));
    return;
  }
  else if (sig == SIGTERM) {
    fprintf(stderr, "%s: Recieved signal SIGTERM.\n", prgname);
    leave(0, NULL, NULL);
  }
  else {
    fprintf(stderr, "%s: Recieved unexpected signal no. %d.", prgname, sig);
    leave(-1, NULL, NULL);
  }
  return;
}
