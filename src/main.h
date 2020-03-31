#ifndef MAIN_H
#define MAIN_H

/*
 * main.h -- common declarations for all modules and C main file
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
 *   <stdlib.h>
 */
 
/* defined in main.c */

extern const char *prgname;


/* exit from program */
void leave(int returnvalue, FILE *file, const char *message);


/* no of cut current instruction */
extern int instruction;

/* scale factors for ASCII print-out, depending from material length and width */
extern int print_scale;  /* parameter read from config file */
extern int print_scale_divider_x; /* for usage in cut.c */
extern int print_scale_divider_y; /* for usage in cut.c */


/* maximal no of pieces and real no of pieces */
extern const int max_n_pieces;
extern int n_pieces;

/* verbose information */
extern int max_level;
extern int n_trials;
extern long n_trials_mio;

/* max length of piece number / piece ID */
extern const int max_piece_name_length;

/*** read by cf_parse() from config file */

/* search depth, limit to search steps per trial */
extern int max_fitted_trials;

/* name of raw material */
extern char *material_name;

/* no of directions: one or two (default: 1) */
extern int n_directions;

/* width of the saw */
extern int cutwidth;

/* additional space (width) to long cuts */
extern int cut_addwidth;

/* formfeed after "formfeed" lines; default: 0 = no formfeed */
extern int formfeed;

/* verbose level: 0 = no messages */
extern int verbose;


/* *** signal handler *** */
void signalhandler(int sig);


#endif /* #ifndef MAIN_H */
