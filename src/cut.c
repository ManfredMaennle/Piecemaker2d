
/*
 * cut.c -- a tree to store cuts
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
#include <string.h>
#include "etc.h"
#include "rect.h"
#include "rctarray.h"
#include "intvect.h"
#include "in_prs_h.h"
#include "main.h"
#include "strres.h"

#include "cut.h"

#ifdef DOS
const char HC = 'Ä';
const char VC = '³';
const char UL = 'Ú';
const char LL = 'À';
const char UR = '¿';
const char LR = 'Ù';
const char HB = 'Ä';
const char VB = '³';
#else
/*
const char HC = '-';
const char VC = '|';
const char UL = ' ';
const char LL = ' ';
const char UR = ' ';
const char LR = ' ';
const char HB = ' ';
const char VB = ' ';
*/
const char HC = '-';
const char VC = '|';
const char UL = '*';
const char LL = '*';
const char UR = '*';
const char LR = '*';
const char HB = '-';
const char VB = '|';
#endif



cut * cut_insertback(cut *l, cut *r, char dir, int p)
{
  cut *newnode;
  newnode = pccmalloc(stderr, "in cut_insertback()", sizeof(cut));
  newnode->left = l;
  newnode->right = r;
  newnode->direction = dir;
  newnode->piece = p;
  return newnode;
}


extern void cut_delete(cut *tree)
{
  cut *l;
  cut *r;
  if (tree == NULL)
    return;
  l = tree->left;
  r = tree->right;
  free(tree);
  cut_delete(l);
  cut_delete(r);
  return;
}


int cut_treedepth(const cut *tree)
{
  if (tree == NULL)
    return 0;
  return max(cut_treedepth(tree->left), cut_treedepth(tree->left));
}


void cut_dump_level(FILE *outfile, int depth, const cut *tree)
{
  int k;
  int piece;
  char *space = "";
  if (tree == NULL) {
    return;
  }
  piece = tree->piece;
  if (piece < 0) {
    piece += n_pieces;
    if (piece<10) 
      space = "*0";
    else
      space = "*";
  }  
  else {  /* piece >= 0 */
    if (piece<10) 
      space = "=0";
    else
      space = "=";
  }
  fprintf(outfile, "%c%s%d ", tree->direction, space, piece);
  if (tree->right != NULL) {
    cut_dump_level(outfile, depth+1, tree->right);
  }
  if (tree->left != NULL) {
    fprintf(outfile, "\n");
    for(k=0; k<depth; ++k) {
      fprintf(outfile, "     ");
    }
    cut_dump_level(outfile, depth, tree->left);
  }
  return;
}
void cut_dump(FILE *outfile, const cut *tree)
{
  if (tree == NULL)
    return;  
  cut_dump_level(outfile, 0, tree); 
  return;
}



void cut_print_level(const cut *tree, int *xg, int *yg, char ***graphic) {
  /* x and y of graphic */
  int xg_left = 0;
  int xg_right = 0;
  int yg_left = 0;
  int yg_right = 0;
  /* real x and y of piece in mm */
  int x_piece = 0;  
  int y_piece = 0;
  /* other variables */
  char **graphic_left = NULL;
  char **graphic_right = NULL;
  char *line = NULL;
  char *number = NULL;
  int piece;
  /* int rotated; */
  char separator = '?';
  int k, i;

  if (tree == NULL) {
    *xg = 0;
    *yg = 0;
    *graphic = NULL;
    return;
  }

  if ( (tree->direction == '#') || (tree->direction == 'r') ) { 
    /* print basic piece */
    piece = tree->piece;
    if (tree->direction == '#') { /* normal piece */
      number = pccmalloc(stderr, "in cuttree_print_level():1", 
			 5 * sizeof(char));
      if (piece < 0) {
	piece += n_pieces;
	/* rotated = 1; */
	separator = 'Z';
	x_piece = sizevector->rectangular[piece]->y;
	y_piece = sizevector->rectangular[piece]->x;
      }
      else {
	/* rotated = 0; */
	separator = 'N';
	x_piece = sizevector->rectangular[piece]->x;
	y_piece = sizevector->rectangular[piece]->y;
      }
      /* ***** v01: very old   
       *xg = 5 + intfloor(x_piece / 100 - 5); 
       *yg = 1 + intfloor(y_piece / 150 - 1); */
      /* ***** v02: as used in cut2d
      *xg = 5 + intfloor((x_piece-1) / 100);
      *yg = 1 + intfloor((y_piece+49) / 150); */
      /* ***** v03: as used in piecemaker2d fix 
      *xg = 5 + intfloor((x_piece-400) / 30);
      *yg = 3 + intfloor((y_piece-300) / 45); */
      /* ***** v03: as used in piecemaker2d adaptive */
      *xg = 1 + max_piece_name_length + intfloor((x_piece / print_scale_divider_x) - max_piece_name_length);
      *yg = 3 + intfloor((y_piece / print_scale_divider_y) - 2); 
    }
    else { /* rest */
#ifdef PRINT_REST
      *xg = 1;
      *yg = 1;
#else
      *xg = 0;
      *yg = 0;
#endif
    }
    *graphic=pccmalloc(stderr, "in cuttree_print_level():3", 
		       (*yg+1) * sizeof(char *));
    (*graphic)[*yg] = NULL;
    if (tree->direction == '#') {  
      /* print piece */
      line = pccmalloc(stderr, "in cuttree_print_level():4_0", 
		       (*xg+1) * sizeof(char));
      (*graphic)[0] = line;
      strnfill(line, ' ', *xg);
      line[*xg] = '\0';
      *(line++) = separator;
      strncpy(line, sizevector->name[piece], max_piece_name_length);
      if (*yg > 1) { 
	line = pccmalloc(stderr, "in cuttree_print_level():4_1", 
			 (*xg+1) * sizeof(char));
	(*graphic)[1] = line;
	strnfill(line, ' ', *xg);
	line[*xg] = '\0';
	if (*yg == 2) {
	  /* print either 3 lines (name, L, W) or only 1 (name) */ 
	  ;
	}
	else { /* if (*yg > 2) */
	  *(line++) = 'L';
	  sprintf(number, "%04d", x_piece);
	  strncpy(line, number, 4);
	}
      }
      if (*yg > 2) {
	line = pccmalloc(stderr, "in cuttree_print_level():4_2", 
			 (*xg+1) * sizeof(char));
	(*graphic)[2] = line;
	line[*xg] = '\0';
	strnfill(line, ' ', *xg);
	*(line++) = 'W';
	sprintf(number, "%04d", y_piece);
	strncpy(line, number, 4);
      }
      /* fill the rest lines with space */
      for (k=3; k<*yg; ++k) {
	line = pccmalloc(stderr, "in cuttree_print_level():4_3", 
			 (*xg+1) * sizeof(char));
	(*graphic)[k] = line;
	line[*xg] = '\0';
	strnfill(line, ' ', *xg);
      }
    } /* end if (tree->direction == '#') */
    else { /* rest */
#ifdef PRINT_REST
      line = pccmalloc(stderr, "in cuttree_print_level():4_4", 
		       2 * sizeof(char));
      (*graphic)[0] = line;
      line[0] = 'R';
      line[1] = '\0';
#endif
    }
    return;
  } /* end if ( (tree->direction == '#') || (tree->direction == 'r') ) */
    
  /* calculate left/lower and right/upper piece */
  cut_print_level(tree->left, &xg_left, &yg_left, &graphic_left);
  cut_print_level(tree->right, &xg_right, &yg_right, &graphic_right);
  
  /* concatenate the two subgraphs */
  if (tree->direction == '|') {
    /* concatenate two subgraphics horizontally */
    *xg = xg_left + xg_right + 1;
    *yg = max(yg_left, yg_right);
    *graphic = pccmalloc(stderr,"in cuttree_print_level():5", 
			 (*yg + 1) * sizeof(char *));
    (*graphic)[*yg] = NULL;
    for(k=0; k < *yg; ++k) {
      /* for all lines of new graphic */
      line = pccmalloc(stderr, "in cuttree_print_level():6", 
		       (*xg + 1) * sizeof(char));
      (*graphic)[k] = line;
      line[*xg] = '\0';
      if (k < yg_left) { /* copy left part */
	strncpy(line, graphic_left[k], xg_left);
      }
      else { /* extend left part with space or VC */
	if (yg_left < 1) {
	  strnfill(line, ' ', xg_left);
	}
	else {
	  for (i = 0; i < xg_left; ++i) {
	    if (graphic_left[yg_left - 1][i] == VC) {
#ifndef NDEBUG 
	      line[i] = '#';
#else
	      line[i] = VC;
#endif
	    }
	    else {
	      line[i] = ' ';
	    }
	  }
	}
      }
      /* write separation line */
      line += xg_left;
      if ( (k >= 0) && (k < *yg) ) {
	*(line++) = VC;
      }
      else {
	*(line++) = ' ';
      }
      if (k < yg_right) { /* copy right part */
	strncpy(line, graphic_right[k], xg_right);
      }
      else { /* extend right part with space or VC */
	if (yg_right < 1) {
	  strnfill(line, ' ', xg_right);
	}
	else {
	  for (i = 0; i < xg_right; ++i) {
	    if (graphic_right[yg_right - 1][i] == VC) {
#ifndef NDEBUG
	      line[i] = '+';
#else
	      line[i] = VC; 
#endif
	    }
	    else {
	      line[i] = ' ';
	    }
	  }
	}
      }
    } /* end for(k=0; k<*yg; ++k) */
  } /* end if (tree->direction == '|') */
  else if (tree->direction == '-') {
    /* concatenate two subgraphics vertically */
    *xg = max(xg_left, xg_right);
    *yg = yg_left + yg_right + 1;
    *graphic = pccmalloc(stderr,"in cuttree_print_level():7", 
			 (*yg + 1) * sizeof(char *));
    (*graphic)[*yg] = NULL;
    /* save upper graph */
    for(k=0; k < yg_right; ++k) {
      line = pccmalloc(stderr, "in cuttree_print_level():8", 
		       (*xg + 1) * sizeof(char));
      (*graphic)[k] = line;
      /* extend the line with space or HC respectively */
      line[*xg] = '\0';
      strnfill(line, ' ', *xg);
      strncpy(line, graphic_right[k], xg_right);
      if (graphic_right[k][xg_right-1] == HC) {
#ifndef NDEBUG	
	strnfill(line+xg_right, '@', *xg-xg_right);
#else
	strnfill(line+xg_right, HC, *xg-xg_right); 
#endif
      }
    }
    /* save horizontal cut line */
    line = pccmalloc(stderr, "in cuttree_print_level():9", 
		     (*xg + 1) * sizeof(char));
    (*graphic)[k] = line;
    line[*xg] = '\0'; 
    strnfill(line, HC, *xg);
    /* save lower graph */
    for(k=0; k < yg_left; ++k) {
      line = pccmalloc(stderr, "in cuttree_print_level():10", 
		       (*xg + 1) * sizeof(char));
      (*graphic)[k + yg_right + 1] = line;
      /* extend the line with space or HC respectively */
      line[*xg] = '\0';
      strnfill(line, ' ', *xg);
      strncpy(line, graphic_left[k], xg_left);
      if (graphic_left[k][xg_left-1] == HC) {
#ifndef NDEBUG	
	strnfill(line+xg_left, '$', *xg-xg_left);
#else
	strnfill(line+xg_left, HC, *xg-xg_left); 
#endif
      }
    }
  } /* end if (tree->direction == '-') */
  else {
    fprintf(stderr, "%s: programming error: `%c' is an invalid", prgname, tree->direction);
    fprintf(stderr, " value for tree->direction\n");
    leave(-1, NULL, NULL);
  }

  /* all done; free memory of the no more needed subgraphics */
  for(k=0; k < yg_left; ++k) {
    free(graphic_left[k]);
  }
  free(graphic_left);
  for(k=0; k < yg_right; ++k) {
    free(graphic_right[k]);
  }
  free(graphic_right);
  return;
}

int cut_print(FILE *outfile, const cut* tree) {
  char **graphic = NULL;
  char *line;
  int xg = 0;
  int yg = 0;
  if (tree == NULL) {
    return 0;
  }
  cut_print_level(tree, &xg, &yg, &graphic);
  if (graphic == NULL) {
    return 0;
  }
  if (outfile == NULL) {
    return xg+2;
  }
  line = pccmalloc(stderr, "in cuttree_print_level():11", 
		   (xg+3) * sizeof(char));
  line[xg+2] = '\0';
  strnfill(line, HB, xg+2);
  line[0] = UL;
  line[xg+1] = UR;
  fprintf(outfile, "%s\n", line);
  while (*graphic != NULL) {
    fprintf(outfile, "%c%s%c\n", VB, *graphic, VB);
    graphic++;
  }
  line[0] = LL;
  line[xg+1] = LR;
  fprintf(outfile, "%s\n", line);
  free(line);
  return xg+2;
}


void pieces_print(FILE *outfile, const rectarray *rct, const intvect *ivect) {
  int k;
  if ( (rct == NULL) || (ivect == NULL) ) {
    return;
  }
  for (k = 0; k < rct->length; ++k) {
    if (ivect->el[k] != 0) {
      fprintf(outfile, strRes(OUTPUT_CUT_NAME), rct->name[k]);
      fprintf(outfile, strRes(OUTPUT_CUT_LENGTH), rct->rectangular[k]->x);
      fprintf(outfile, strRes(OUTPUT_CUT_WIDTH), rct->rectangular[k]->y);
      fprintf(outfile, strRes(OUTPUT_CUT_NUMBER), ivect->el[k]);
    }
  }
  return;
}
