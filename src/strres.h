#ifndef STRRES_H
#define STRRES_H
/*
 * strres.h -- string resources
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
 */

/*
 * Method design taken from Jones, Nigel
 * "Support Multiple Languages" Embedded Systems Programming, February 2001.
 * https://barrgroup.com/Embedded-Systems/How-To/Firmware-Internationalization
 * 
 * for unicode support, see ICU4C or https://www.cprogramming.com/tutorial/unicode.html
 */

typedef enum 
{ 
	DEFAULT_LANGUAGE, /* contains the ID-string */
	ENGLISH, 
	GERMAN,
	LAST_LANGUAGE
} language_t;

language_t language;

/*
 * declare new string ressources here, then add translations in strres.c
 */
typedef enum
{
    DEFAULT_STRRES, 
    INVALID_LANGUAGE_OPTION,
    EMPTY_CONFIG_FILENAME,
    UNRECOGNIZED_OPTION,
    GET_USAGE_INFORMATION,
    ERROR_INVALID_PROGRAM_NAME,
    ERROR_CANNOT_READ_RC_FILE,
    ERROR_CONFIG_FILENAME_NOT_SPECIFIED,
    ERROR_CANNOT_READ_CONFIG_FILE,
    ERROR_CONFIG_SEARCH_DEPTH,
    ERROR_CONFIG_CONFIG_FILENAME,
    ERROR_CONFIG_OUTPUT_FILENAME,
    ERROR_CONFIG_MATERIAL_NAME,
    ERROR_CONFIG_MATERIAL_LENGTH_MISSING,
    ERROR_CONFIG_MATERIAL_LENGTH_INVALID,
    ERROR_CONFIG_MATERIAL_WIDTH_MISSING,
    ERROR_CONFIG_MATERIAL_WIDTH_INVALID,
    ERROR_CONFIG_N_DIRECTIONS_INVALID,
    ERROR_CONFIG_CUT_WIDTH,
    ERROR_CONFIG_CUT_ADD_WIDTH,
    ERROR_CONFIG_PRINT_SCALE,
    ERROR_CONFIG_VERBOSE,
    LICENSE,
    PRINT_USAGE_USAGE,
    PRINT_USAGE_OPTIONS,
    PRINT_USAGE_CONFIG_FILENAME,
    PRINT_USAGE_COPYRIGHT,
    PRINT_USAGE_HELP,
    PRINT_USAGE_LANGUAGE,
    PRINT_USAGE_VERSION,
    LEAVE_HIT_RETURN_TO_CONTINUE,
    SIGNALHANDLER_EXIT_QUESTION,
    SIGNALHANDLER_CONTINUE,
    ERROR_CANNOT_OPEN_INPUT_FILE,
    ERROR_CANNOT_OPEN_OUTPUT_FILE,
    OUTFILE_RAW_MATERIAL,
    OUTFILE_NAME_WIDTH_LENGTH,
    OUTFILE_CUTTING_ALLOWANCE,
    OUTFILE_CUTTING_WIDTH,
    OUTFILE_CUTTING_DIRECTIONS_1,
    OUTFILE_CUTTING_DIRECTIONS_2,
    OUTFILE_MAX_SEARCH_DEPTH,
    OUTFILE_SEARCH_DEPTH,
    OUTFILE_PRINT_SCALE,
    OUTFILE_BILL_OF_MATERIAL,
    ERROR_MAX_N_CUTS_EXCEEDED,
    OUTFILE_ATTENTION_PIECES_THAT_CANNOT_BE_CUT,
    OUTFILE_OVERALL_WASTE,
    OUTFILE_N_NEEDED_RAW_MATERIAL_PIECES,
    OUTFILE_CUTTING_INSTRUCTIONS_NO,
    OUTPUT_CUT_INSTR_WASTE,
    OUTPUT_CUT_INSTR_AMOUNT,
    OUTPUT_CUT_NAME,
    OUTPUT_CUT_LENGTH,
    OUTPUT_CUT_WIDTH,
    OUTPUT_CUT_NUMBER,
    CF_PARSE_FILE_EMPTY,
    CF_PARSE_PARSE_ERROR,
    IN_PARSE_FILE_EMPTY,
    IN_PARSE_ERROR_TOO_MANY_PIECES,
    IN_PARSE_ERROR_ZERO_VALUES,
    IN_PARSE_PARSE_ERROR,
    WARNING_PIECE_NAME_TOO_LONG,
    WARNING_PIECE_NAME_MISSING,
    EMPTY,
    STRRES_BOUNDARY_CHECK,
    LAST_STRRES
} strres_t;


/*
 * the function to be used to call string ressources
 */
char const * strRes(strres_t str_no);


#endif /* #ifndef STRRES_H */

