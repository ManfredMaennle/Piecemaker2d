
/*
 * strres.c -- string resources
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


#include "strres.h"

language_t language = DEFAULT_LANGUAGE;

typedef struct
{
    /*
     * Maximum length
     */
    /* int const len; */
    
    /*
     * Array of pointers to language-specific string
     */
    char const * const text[LAST_LANGUAGE]; 
    
} strres_translations_t;


static const strres_translations_t translations[LAST_STRRES] =
{ 
  {{	"DEFAULT_STRRES", 		/* ID string as in strres.h */
	"default string resource",	/* English */
	"Voreingestellter Text",	/* German */
    }},
  {{	"INVALID_LANGUAGE_OPTION",
	"%s: inalid language in option `%s'\n",
	"%s: ungueltige Sprache in Option `%s'\n",
    }},
  {{	"EMPTY_CONFIG_FILENAME",
	"%s: error: empty config file name at command line option --c=\n",
	"%s: Fehler: Leerer Dateiname fuer config Datei bei Option --c=\n",
    }},
  {{	"UNRECOGNIZED_OPTION",
	"%s: unrecognized option `%s'\n",
	"%s: unbekannte Option `%s'\n",
    }},
  {{	"GET_USAGE_INFORMATION",
	"\nType `%s -h' to get usage information.\n\n",
	"\nStarte `%s -h' fuer Benutzungshinweise.\n\n",
    }},
  {{	"ERROR_INVALID_PROGRAM_NAME",
	"%s: error: invalid program name `%s'\n",
	"%s: Fehler: unzulaessiger Programmname `%s'\n",
    }},
  {{	"ERROR_CANNOT_READ_RC_FILE",
	"%s: error: cannot open preconfig rc file `%s' for reading!\n",
	"%s: Fehler: kann Vorkonfigurations- rc-Datei `%s' nicht zum Lesen oeffnen!\n",
    }},
  {{	"ERROR_CONFIG_FILENAME_NOT_SPECIFIED",
	"%s: error in pre-configuration:\n Name of CONFIG_FILE must be specified!\n",
	"%s: Fehler in Vorkonfiguration:\n Name der KONFIGURATIONSDATEI muss angegeben werden!\n",
    }},
  {{	"ERROR_CANNOT_READ_CONFIG_FILE",
	"%s: error: cannot open config file `%s' for reading!\n",
	"%s: Fehler: kann Konfigurationsdatei `%s' nicht zum Lesen oeffnen!\n",
    }},
  {{	"ERROR_CONFIG_SEARCH_DEPTH",
	"%s: error in configuration `%s':\nSearch depth MAX_TRIALS must be of value between 1 and 7 (instead of `%d')!\n",
	"%s: Fehler in Konfiguration `%s':\nSUCHTIEFE muss ein Wert von 1 bis 7 sein (statt `%d')!\n",
    }},
  {{	"ERROR_CONFIG_CONFIG_FILENAME",
	"%s: error in configuration `%s':\nDefinition of CONFIG_FILENAME is missing!\n",
	"%s: Fehler in Konfiguration `%s':\nName der EINGABEDATEI fehlt!\n",
    }},
  {{	"ERROR_CONFIG_OUTPUT_FILENAME",
	"%s: error in configuration `%s':\nDefinition of OUTPUT_FILENAME is missing!\n",
	"%s: Fehler in Konfiguration `%s':\nName der AUSGABEDATEI fehlt!\n",
    }},
  {{	"ERROR_CONFIG_MATERIAL_NAME",
	"%s: error in configuration `%s':\nDefinition of MATERIAL_NAME is missing!\n",
	"%s: Fehler in Konfiguration `%s':\nBezeichnung fuer das MATERIAL fehlt!",
    }},
  {{	"ERROR_CONFIG_MATERIAL_LENGTH_MISSING",
	"%s: error in configuration `%s':\nDefinition of MATERIAL_LENGTH is missing!\n",
	"%s: Fehler in Konfiguration `%s':\nMATERIALLAENGE fehlt!\n",
    }},
  {{	"ERROR_CONFIG_MATERIAL_LENGTH_INVALID",
	"%s: error in configuration `%s':\n`%d' is invalid MATERIAL_LENGTH because <0 or >1000000!\n",
	"%s: Fehler in Konfiguration `%s':\n`%d' ist unzulaessige MATERIALLAENGE, da <0 oder >1000000!\n",
    }},
  {{	"ERROR_CONFIG_MATERIAL_WIDTH_MISSING",
	"%s: error in configuration `%s':\nDefinition of MATERIAL_WIDTH is missing!\n",
	"%s: Fehler in Konfiguration `%s':\nMATERIALBREITE fehlt!\n",
    }},
  {{	"ERROR_CONFIG_MATERIAL_WIDTH_INVALID",
	"%s: error in configuration `%s':\n`%d' is invalid MATERIAL_WIDTH because <0 or >1000000!\n",
	"%s: Fehler in Konfiguration `%s':\n`%d' ist unzulaessige MATERIALBREITE, da <0 oder >1000000!\n",
    }},
  {{	"ERROR_CONFIG_N_DIRECTIONS_INVALID",
	"%s: error in configuration `%s':\nN_DIRECTIONS must be 1 or 2 (instead of `%d')!\n",
	"%s: Fehler in Konfiguration `%s':\nnur 1 oder 2 RICHTUNGEN (statt `%d') zulaessig!\n",
    }},
  {{	"ERROR_CONFIG_CUT_WIDTH",
	"%s: error in configuration `%s':\nCUT_WIDTH must be a value between 0 and 100 (instead of `%d')!\n",
	"%s: Fehler in Konfiguration `%s':\nBLATTDICKE muss ein Wert von 0 bis 100 sein (statt `%d')!\n",
    }},
  {{	"ERROR_CONFIG_CUT_ADD_WIDTH",
	"%s: error in configuration `%s':\nCUT_ADD_WIDTH must be a value between 0 and 1000 (instead of `%d')!\n",
	"%s: Fehler in Konfiguration `%s':\nSCHNITTZUGABE muss ein Wert von 0 bis 1000 sein (statt `%d')!\n",
    }},
  {{	"ERROR_CONFIG_PRINT_SCALE",
	"%s: error in configuration `%s':\nPRINT_SCALE must be a value between 1 and 1000000 (instead of `%d')!\n",
	"%s: Fehler in Konfiguration `%s':\nDRUCKSKALIERUNG muss ein Wert von 1 bis 1000000 sein (statt `%d')!\n",
    }},
  {{	"ERROR_CONFIG_VERBOSE",
	"%s: error in configuration `%s':\nVERBOSE must be a value between 0 and 7 (instead of `%d')!\n",
	"%s: Fehler in Konfiguration `%s':\nINFO muss ein Wert von 0 bis 7 sein (statt `%d')!\n",
    }},
  {{    "LICENSE",       /* so-called MIT X11 open source license */
        "Copyright (c) 1999, 2020 Manfred Maennle, Karlsruhe/Kandel, Germany\n\n\
Permission is hereby granted, free of charge, to any person obtaining\n\
a copy of this software and associated documentation files (the\n\
\"Software\"), to deal in the Software without restriction, including\n\
without limitation the rights to use, copy, modify, merge, publish,\n\
distribute, sublicense, and/or sell copies of the Software, and to\n\
permit persons to whom the Software is furnished to do so, subject to\n\
the following conditions:\n\n\
The above copyright notice and this permission notice shall be\n\
included in all copies or substantial portions of the Software.\n\n\
THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n\
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n\
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND\n\
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE\n\
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION\n\
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION\n\
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.",
        "Copyright (c) 1999, 2020 Manfred Maennle, Karlsruhe/Kandel, Germany\n\n\
Jedem, der eine Kopie dieser Software und der zugehoerigen\n\
Dokumentationsdateien (die \"Software\") erhaelt, wird hiermit kostenlos\n\
die Erlaubnis erteilt, ohne Einschraenkung mit der Software zu handeln,\n\
einschliesslich und ohne Einschraenkung der Rechte zur Nutzung, zum\n\
Kopieren, Aendern, Zusammenfuehren, Veroeffentlichen, Verteilen,\n\
Unterlizenzieren und/oder Verkaufen von Kopien der Software, und\n\
Personen, denen die Software zur Verfuegung gestellt wird, dies unter\n\
den folgenden Bedingungen zu gestatten:\n\n\
Der obige Urheberrechtshinweis und dieser Genehmigungshinweis muessen\n\
in allen Kopien oder wesentlichen Teilen der Software enthalten sein.\n\n\
DIE SOFTWARE WIRD OHNE MAENGELGEWAEHR UND OHNE JEGLICHE AUSDRUECKLICHE\n\
ODER STILLSCHWEIGENDE GEWAEHRLEISTUNG, EINSCHLIESSLICH, ABER NICHT\n\
BESCHRAENKT AUF DIE GEWAEHRLEISTUNG DER MARKTGAENGIGKEIT, DER EIGNUNG\n\
FUER EINEN BESTIMMTEN ZWECK UND DER NICHTVERLETZUNG VON RECHTEN DRITTER,\n\
ZUR VERFUEGUNG GESTELLT. DIE AUTOREN ODER URHEBERRECHTSINHABER SIND IN\n\
KEINEM FALL HAFTBAR FUER ANSPRUECHE, SCHAEDEN ODER ANDERE\n\
VERPFLICHTUNGEN, OB IN EINER VERTRAGS- ODER HAFTUNGSKLAGE, EINER\n\
UNERLAUBTEN HANDLUNG ODER ANDERWEITIG, DIE SICH AUS, AUS ODER IN\n\
VERBINDUNG MIT DER SOFTWARE ODER DER NUTZUNG ODER ANDEREN GESCHAEFTEN\n\
MIT DER SOFTWARE ERGEBEN.",
    }},
  {{	"PRINT_USAGE_USAGE",
	"\nusage: %s [options]\n\n",
	"\nAufruf: %s [Optionen]\n\n",
    }},
  {{	"PRINT_USAGE_OPTIONS",
	"options:\n",	
	"Optionen:\n",	
    }},
  {{	"PRINT_USAGE_CONFIG_FILENAME",
	"  --c=FILE  Use configuration from FILE; do not load a preconfig rc file.\n",	
	"  --c=FILE  Nutze FILE als Konfigurationsdatei; eine Praekonfigurations rc-Datei wird nicht ausgewertet.\n",	
    }},
  {{	"PRINT_USAGE_COPYRIGHT",
	"  -C        print copyright and warranty information\n",	
	"  -C        zeige Copyright- und Garantieinformation\n",	
    }},
  {{	"PRINT_USAGE_HELP",
	"  -h        print this help\n",	
	"  -h        drucke diese Hilfe\n",	
    }},
  {{	"PRINT_USAGE_LANGUAGE",
	"  --l=LANG  set language: LANG must be out of ENGLISH, GERMAN\n",	
	"  --l=LANG  waehle Sprache: LANG kann sein: ENGLISH, GERMAN\n",	
    }},
  {{	"PRINT_USAGE_VERSION",
	"  -V        print version information and compiler settings\n",	
	"  -V        zeige Versions- und Kompilierinformation\n",	
    }},
  {{	"LEAVE_HIT_RETURN_TO_CONTINUE",
	"\nPlease hit <RETURN> to continue!\n",	
	"\nDruecken Sie bitte <RETURN> um fortzufahren!\n",	
    }}, 
  {{	"SIGNALHANDLER_EXIT_QUESTION",
	"Exit program? (y/n) [y] ",	
	"Programm verlassen? (j/n) [j] ",	
    }},	
  {{	"SIGNALHANDLER_CONTINUE",
	"Continue calculation...\n",	
	"Fahre Berechnung fort...\n",	
    }},	
  {{	"ERROR_CANNOT_OPEN_INPUT_FILE",
	"error: cannot open input file `%s' for reading!\n",
	"Fehler: kann Eingabedatei `%s' nicht zum Lesen oeffnen!\n",
    }},
  {{	"ERROR_CANNOT_OPEN_OUTPUT_FILE",
	"error: cannot open input file `%s' for reading!\n",
	"Fehler: kann Ausgabedatei `%s' nicht zum Schreiben oeffnen!\n",
    }},
  {{	"OUTFILE_RAW_MATERIAL",
	"raw material:\n",
	"Ausgangsmaterial:\n",
    }},
  {{	"OUTFILE_NAME_WIDTH_LENGTH",
	"name: \"%s\", length x: %d mm, width y: %d mm\n\n",
	"Name: \"%s\", Laenge x: %d mm, Breite y: %d mm\n\n",
    }},
  {{	"OUTFILE_CUTTING_ALLOWANCE",
	"cutting allowance: %d mm\n",
	"Schnittzugabe: %d mm\n",
    }},
  {{	"OUTFILE_CUTTING_WIDTH",
	"cutting width (of saw blade): %d mm\n",
	"Saegeblattstaerke: %d mm\n",
    }},
  {{	"OUTFILE_CUTTING_DIRECTIONS_1",
	"cutting directions: 1, i.e. no turning possible.\n",
	"Saegerichtung: 1, d.h. nur wie vorgegeben.\n",
    }},
  {{	"OUTFILE_CUTTING_DIRECTIONS_2",
	"cutting directions: 2, i.e. 90 degree turning is possible.\n",
	"Saegerichtung: 2, d.h. 90 Grad Drehung moeglich.\n",
    }},
  {{	"OUTFILE_MAX_SEARCH_DEPTH",
	"(mamimum search depth for solution search: %d)\n",
	"(Maximale Suchtiefe bei der Berechnung: %d)\n",
    }},
  {{	"OUTFILE_SEARCH_DEPTH",
	"(resulting search depth for solution search: %d)\n",
	"(Resultierende Suchtiefe bei der Berechnung: %d)\n",
    }},
  {{	"OUTFILE_PRINT_SCALE",
	"print scale: %d\n",
	"Druckskalierung: %d\n",
    }},
  {{	"OUTFILE_BILL_OF_MATERIAL",
	"bill of material from input file:\n",
	"Stueckliste aus der Eingabedatei:\n",
    }},
  {{	"ERROR_MAX_N_CUTS_EXCEEDED",
	"error: max number of cuts exceeded!",
	"Fehler: maximale Anzahl an Zuschnitten ueberschritten!",
    }},
  {{	"OUTFILE_ATTENTION_PIECES_THAT_CANNOT_BE_CUT",
	"ATTENTION! There are pieces that cannot be cut out:\n",
	"ACHTUNG! Es gibt nicht zuschneidbare Stuecke:\n",
    }},
  {{	"OUTFILE_OVERALL_WASTE",
	"overall waste: %d sqmm = %.2f %%\n",
	"Gesamtverschnitt: %d qmm = %.2f %%\n",
    }},
  {{	"OUTFILE_N_NEEDED_RAW_MATERIAL_PIECES",
	"overall number of needed raw material pieces: %d\n\n",
	"Gesamtzahl benoetigter Platten: %d\n\n",
    }},
  {{	"OUTFILE_CUTTING_INSTRUCTIONS_NO",
	"cutting instructions no. %d:\n",
	"Schnittvorlage Nr %d:\n",
    }},
  {{	"OUTPUT_CUT_INSTR_WASTE",
	"(waste: %d qmm = %.2f %%; %ld%06d search trials)\n",
	"(Verschnitt: %d qmm = %.2f %%; %ld%06d Versuche)\n",
    }},
  {{	"OUTPUT_CUT_INSTR_AMOUNT",
	"amount: %d\n\n",
	"Menge: %d\n\n",
    }},
  {{	"OUTPUT_CUT_NAME",
	"name: \"%s\", ",
	"Name: \"%s\", ",
    }},
  {{	"OUTPUT_CUT_LENGTH",
	"length: %04d mm, ",
	"Laenge: %04d mm, ",
    }},
  {{	"OUTPUT_CUT_WIDTH",
	"width: %04d mm, ",
	"Breite: %04d mm, ",
    }},
  {{	"OUTPUT_CUT_NUMBER",
	"amount: %d\n",
	"Anzahl: %d\n",
    }},
  {{	"CF_PARSE_FILE_EMPTY",
	"warning: config file empty\n",
	"Warnung: config Datei leer\n",
    }},
  {{	"CF_PARSE_PARSE_ERROR",
	"\n%s: in config file, line %d: parse error at or before`%s'\n",
	"\n%s: in config Datei, Zeile %d: ungueltige Daten (parse error) bei oder vor `%s'\n",
    }},
  {{	"IN_PARSE_FILE_EMPTY",
	"warning: input file empty\n",
	"Warnung: Eingabedatei leer\n",
    }},
  {{	"IN_PARSE_ERROR_TOO_MANY_PIECES",
	"%s: error in input file: too many pieces (> %d)\n",
	"%s: Fehler in Eingabedatei: Zu viele Stuecke (> %d)\n",
    }},
  {{	"IN_PARSE_ERROR_ZERO_VALUES",
	"%s: error in input file line `%d': n, x, or y == 0\n",
	"%s: Fehler in Eingabedatei Zeile `%d': n, x, oder y == 0\n",
    }},
  {{	"IN_PARSE_PARSE_ERROR",
	"\n%s: in input file, line %d: parse error at or before `%s'\n",
	"\n%s: in Eingabedatei, Zeile %d: ungueltige Daten (parse error) bei oder vor `%s'\n",
    }},
  {{	"WARNING_PIECE_NAME_TOO_LONG",
	"warning: piece number '%s' is too long, program uses first %d characters.\n",
	"Warnung: Teilenummer '%s' ist zu lang, das Programm nutzt die ersten %d Stellen.\n",
    }},
  {{	"WARNING_PIECE_NAME_MISSING",
	"warning: piece number is missing at position %d in bill of materials\n",
	"Warnung: Teilenummer fehlt an Position %d der Materialliste.\n",
    }},
  {{	"EMPTY",
	"",
	"",
    }},
  {{	"STRRES_BOUNDARY_CHECK ref error ID string",	/* ID string as in strres.h */
	"STRRES_BOUNDARY_CHECK ref error English",		/* English */
	"STRRES_BOUNDARY_CHECK ref error German",		/* German */
    }}
};


/*
 * the function to be used to call string ressources
 */
char const * strRes(strres_t str_no)
{
    return translations[str_no].text[language];
}



