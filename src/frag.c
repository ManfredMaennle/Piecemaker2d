/* 
 * frag.c -- fragmentation of rectangulars
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

#include "revision.h"
#include "etc.h"
#include "rect.h"
#include "rctarray.h"
#include "intvect.h"
#include "cut.h"

#include "main.h"
#include "in_prs_h.h"

/* this file provides definitions for declarations in */
#include "frag.h"


int fragmentation(int level, const rect *original, intvect *piecevect,
		  cut **cuttree)
{
  int trial;
#ifdef SKIP_SAME_SIZE
  int last_x = 0;
  int last_y = 0;
#endif
  int rotated;
  int fitted_trials = 0;
  int shortcut = 0;
  int frag;
  int fit;
  rect *rest1 = newrect(0, 0);
  rect *rest2 = newrect(0, 0);
  rect *rest3 = newrect(0, 0);
  rect *rest4 = newrect(0, 0);
  rect *piece = newrect(0, 0);
  intvect *newpiecevect;
  cut *newcuttreeleft;
  cut *newcuttreeright;
  int min_frag = rect_surface(original);
  int min_trial = -n_pieces-1;
  intvect *min_piecevect = NULL;
  cut *min_cuttreeleft = NULL;
  cut *min_cuttreeright = NULL;
  char min_dir = '*';
  
  ++level;
  if (level > max_level) {
    max_level = level;
  }

  /* try both directions/rotations, if n_directions == 2 */
  for (rotated=0; rotated<n_directions; ++rotated) {
    trial = 0;
    /* try it for all pieces */
    while ( (! shortcut) && (trial<piecevect->length) ) {
      if (piecevect->el[trial] > 0) {
	if (rotated) {
	  piece->x = sizevector->rectangular[trial]->y;
	  piece->y = sizevector->rectangular[trial]->x;
	}
	else {
	  piece->x = sizevector->rectangular[trial]->x;
	  piece->y = sizevector->rectangular[trial]->y;
	}
	/* *** if new piece has same size as the last one; skip it *** */
#ifdef SKIP_SAME_SIZE
	if ( (piece->x == last_x) && (piece->y == last_y) ) {
	  ++trial;
	  continue;
	}
	last_x = piece->x;
	last_y = piece->y;
#endif	
	/* ok, investigate new trial */
	++n_trials;
	if ( (n_trials % 1000000) == 0 ) {
	  n_trials = 0;
	  ++n_trials_mio;
	  /* print progress information */
	  if ((verbose > 2) && (n_trials_mio % 10 == 0)) {
	    fprintf(stderr, "search depth: %d, instruction no %d, ", \
		    max_fitted_trials, instruction+1);
	    fprintf(stderr, "max_level: %d, n_trials: %ld mio.\n", \
		    max_level, n_trials_mio);
	  }
	}
	fit = rect_fit(original, piece, &rest1, &rest2, &rest3, &rest4);
	switch(fit) {
	case 0:
	  /* didn't fit, i.e., piece was too big */
	  break;
	case 1:
	  /* exact fit */
	  ++fitted_trials;
  	  newpiecevect = newcopyintvect(piecevect);
	  --newpiecevect->el[trial];
	  /* optimal possibility, frag == 0, no further trials */
	  min_frag = 0;
	  if (rotated) {
	    min_trial = trial - n_pieces;
	  }
	  else {
	    min_trial = trial;
	  }
	  cut_delete(min_cuttreeleft);
	  cut_delete(min_cuttreeright);
	  min_cuttreeleft = NULL;
	  min_cuttreeright = NULL;
	  min_dir = '#';
	  intvect_delete(min_piecevect);
	  min_piecevect = newpiecevect;
	  shortcut = 1;
	  break;
	case 2:
	case 3:
	  /* one dimension fits exactly, one cut necessary at the other */
	  ++fitted_trials;
	  newpiecevect = newcopyintvect(piecevect);
	  --newpiecevect->el[trial];
	  frag = fragmentation(level, rest1, newpiecevect, &newcuttreeleft);
	  if (frag < min_frag) {
	    min_frag = frag;
	    if (rotated) {
	      min_trial = trial - n_pieces;
	    }
	    else {
	      min_trial = trial;
	    }
	    cut_delete(min_cuttreeleft);
	    cut_delete(min_cuttreeright);
	    if (fit == 2) {
	      min_dir = '-';
	      min_cuttreeright = cut_insertback(NULL, NULL, '#', min_trial);
	      min_cuttreeleft = newcuttreeleft;
	    }
	    else {
	      min_dir = '|';
	      min_cuttreeleft = cut_insertback(NULL, NULL, '#', min_trial);
	      min_cuttreeright = newcuttreeleft;
	    }
	    intvect_delete(min_piecevect);
	    min_piecevect = newpiecevect;
	    if (fitted_trials >= 1) {
	      if (fitted_trials+level > max_fitted_trials) {
		shortcut = 1;
		break;
	      }
	    }
	  }
	  else {
	    cut_delete(newcuttreeleft);
	    intvect_delete(newpiecevect);
	  }	  
	  break;
	case 4:
	  /* two cuts necessary; two possibilities */
	  /* first possibility: first '|' then '-' */
	  ++fitted_trials;
	  newpiecevect = newcopyintvect(piecevect);
	  --newpiecevect->el[trial];
	  frag = fragmentation(level, rest1, newpiecevect, &newcuttreeleft)
	    + fragmentation(level, rest2, newpiecevect, &newcuttreeright);
	  if (frag < min_frag) {
	    min_frag = frag;
	    if (rotated) {
	      min_trial = trial - n_pieces;
	    }
	    else {
	      min_trial = trial;
	    }
	    cut_delete(min_cuttreeleft);
	    cut_delete(min_cuttreeright);
	    min_cuttreeright = newcuttreeright;
	    min_cuttreeleft = cut_insertback(NULL, NULL, '#', min_trial);
	    min_cuttreeleft = cut_insertback(newcuttreeleft, min_cuttreeleft,
					     '-', min_trial);
	    min_dir = '|';
	    intvect_delete(min_piecevect);
	    min_piecevect = newpiecevect;
	    if (fitted_trials >= 1) {
	      if (fitted_trials+level > max_fitted_trials) {
		shortcut = 1;
		break;
	      }
	    }
	  }
	  else {
	    cut_delete(newcuttreeright);
	    cut_delete(newcuttreeleft);
	    intvect_delete(newpiecevect);
	  }	  
	  /* second possibility: first '-' then '|' */
	  ++fitted_trials;
  	  newpiecevect = newcopyintvect(piecevect);
	  --newpiecevect->el[trial];
	  frag = fragmentation(level, rest3, newpiecevect, &newcuttreeright)
	    + fragmentation(level, rest4, newpiecevect, &newcuttreeleft);
	  if (frag < min_frag) {
	    min_frag = frag;
	    if (rotated) {
	      min_trial = trial - n_pieces;
	    }
	    else {
	      min_trial = trial;
	    }
	    cut_delete(min_cuttreeleft);
	    cut_delete(min_cuttreeright);
	    min_cuttreeleft = newcuttreeleft;
	    min_cuttreeright = cut_insertback(NULL, NULL, '#', min_trial);
	    min_cuttreeright = cut_insertback(min_cuttreeright, 
					      newcuttreeright, '|', min_trial);
	    min_dir = '-';
	    intvect_delete(min_piecevect);
	    min_piecevect = newpiecevect;
	    if (fitted_trials >= 1) {
	      if (fitted_trials+level > max_fitted_trials) {
		shortcut = 1;
		break;
	      }
	    }
	  }
	  else {
	    cut_delete(newcuttreeright);
	    cut_delete(newcuttreeleft);
	    intvect_delete(newpiecevect);
	  }	  
	  break;
	default:
	  fprintf(stderr, "%s: programming error: non valid return value\n", prgname);
	  leave(-1, NULL, NULL);
	} /* end switch(fit) */
      } /* end if (piecevect->el[k] > 0) */
      ++trial;
    } /* end while ( (! shortcut) || (trial<piecevect->length) ) */
  } /* end for (direction=0; direction<n_directions; ++direction) */

  /* return min */
  if (min_piecevect != NULL) {
    copyintvect(piecevect, min_piecevect);
    intvect_delete(min_piecevect);
    *cuttree = cut_insertback(min_cuttreeleft, min_cuttreeright, 
			      min_dir, min_trial);
  }
  else { 
    /* no piece fitted; return original as a rest (fragmentation) */
    *cuttree = cut_insertback(NULL, NULL, 'r', min_frag);
  }
  free(rest1); free(rest2); free(rest3); free(rest4); free(piece);
  return min_frag;
}
