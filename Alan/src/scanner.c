/**
 * @file    scanner.c
 * @brief   The scanner for ALAN-2022.
 * @author  W.H.K. Bester (whkbester@cs.sun.ac.za)
 * @date    2022-08-03
 */

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "boolean.h"
#include "error.h"
#include "scanner.h"
#include "token.h"

/* --- type definitions and constants --------------------------------------- */

typedef struct {
	char      *word;                   /* the reserved word, i.e., the lexeme */
	TokenType  type;                   /* the associated token type           */
} ReservedWord;

/* --- global static variables ---------------------------------------------- */

static FILE *src_file;                 /* the source file pointer             */
static int   ch;                       /* the next source character           */
static int   column_number;            /* the current column number           */
static ReservedWord reserved[] = {
	{"and",TOKEN_AND},
	{"array", TOKEN_ARRAY},
	{"begin",TOKEN_BEGIN},
	{"boolean",TOKEN_BOOLEAN},
	{"call",TOKEN_CALL},
	{"do",TOKEN_DO},
	{"else",TOKEN_ELSE},
	{"elsif",TOKEN_ELSIF},
	{"end",TOKEN_END},
	{"false",TOKEN_FALSE},
	{"function",TOKEN_FUNCTION},
	{"get",TOKEN_GET},
	{"if",TOKEN_IF},
	{"integer",TOKEN_INTEGER},
	{"leave",TOKEN_LEAVE},
	{"not",TOKEN_NOT},
	{"or",TOKEN_OR},
	{"put",TOKEN_PUT},
	{"relax",TOKEN_RELAX},
	{"rem",TOKEN_REMAINDER},
	{"source",TOKEN_SOURCE},
	{"then",TOKEN_THEN},
	{"to",TOKEN_TO},
	{"true",TOKEN_TRUE},
	{"while",TOKEN_WHILE}
     };

	/* TODO: Populate this array with the appropriate pairs of reserved word
	 * strings and token types, sorted alphabetically by reserved word string.
	 */

#define NUM_RESERVED_WORDS     (sizeof(reserved) / sizeof(ReservedWord))
#define MAX_INITIAL_STRING_LEN (1024)

/* --- function prototypes -------------------------------------------------- */

static void next_char(void);
static void process_number(Token *token);
static void process_string(Token *token);
static void process_word(Token *token);
static void skip_comment(void);

/* --- scanner interface ---------------------------------------------------- */

void init_scanner(FILE *in_file)
{
	src_file = in_file;
	position.line = 1;
	position.col = column_number = 0;
	next_char();
}

void get_token(Token *token)
{
	/* remove whitespace */
	/* TODO: Skip all whitespace characters before the start of the token. */
    while (isspace(ch) || ch == '\n') {
		 next_char();
	}
	int colmis = 0;
	/* remember token start */
	position.col = column_number;

	/* get the next token */
	if (ch != EOF) {
		if (isalpha(ch) || ch == '_') {

			/* process a word */
			process_word(token);

		} else if (isdigit(ch)) {

			/* process a number */
			process_number(token);

		} else switch (ch) {

			/* process a string */
			case '"':
				position.col = column_number;
				next_char();
				process_string(token);
				//get_token(token);
				break;
			/* TODO: Process the other tokens, and trigger comment skipping. */
			case '>':
              next_char();
              if (ch == '=') {
              token->type = TOKEN_GREATER_EQUAL;
			  next_char();
			  break;
              } else {
                  token->type = TOKEN_GREATER_THAN;
				  next_char();
               break;
			   }
              break;
              case '<':
              next_char();
              if (ch == '=') {
             token->type = TOKEN_LESS_EQUAL;
			 next_char();
			 break;
              } else if (ch == '>') {
              token->type = TOKEN_NOT_EQUAL;
			  next_char();
			  break;
              }
              else {
                  token->type = TOKEN_LESS_THAN;
				  next_char();
				  break;
              }
              break;
              case '-':
                  token->type = TOKEN_MINUS;
				  next_char();
                  break;
              case '+':
                  token->type = TOKEN_PLUS;
				  next_char();
                 break;
              case '/':
                  token->type = TOKEN_DIVIDE;
				  next_char();
                  break;
              case '*':
               token->type = TOKEN_MULTIPLY;
			   next_char();
                  break;
              case ']':
                 token->type = TOKEN_CLOSE_BRACKET;
				 next_char();
				break;
              case ')':
                  token->type = TOKEN_CLOSE_PARENTHESIS;
				  next_char();
                  break;
              case ',':
                  token->type = TOKEN_COMMA;
				  next_char();
                  break;

				case '.':
                  token->type = TOKEN_CONCATENATE;
				  next_char();
				break;
              case ':':
			   colmis = column_number;
                  next_char();
                  if (ch != '=') {
					position.col = colmis;
				  leprintf("illegal character '%c' (ASCII #%d)",':',':');
				  }
                  token->type = TOKEN_GETS;
				  next_char();
                  break;
              case '[':
                  token->type = TOKEN_OPEN_BRACKET;
				  next_char();
                  break;
              case '(':
                  token->type = TOKEN_OPEN_PARENTHESIS;
				  next_char();
                  break;
              case ';':
                  token->type = TOKEN_SEMICOLON;
				  next_char();
                  break;
			 case '=':
			 token->type = TOKEN_EQUAL;
			 next_char();
			 break;
			 case '{':
			 skip_comment();
			 next_char();
			 get_token(token);
			 break;
				default:
				position.col = column_number;
				leprintf("illegal character '%c' (ASCII #%d)",ch,ch);
		}
	} else {
		token->type = TOKEN_EOF;
	}
}

/* --- utility functions ---------------------------------------------------- */

void next_char(void)
{
	/* TODO:
	 * - Get the next character from the source file.
	 * - Increment the line number if the previous character is EOL.
	 * - Reset the global column number when necessary.
	 * - DO NOT USE feof!!!
	 */
	static char last_read = '\0';
	ch = fgetc(src_file);
	if (last_read =='\n' && ch != EOF) {
		 column_number = 1;
		 position.line+=1;
	} else if (ch != EOF) {
	 column_number+=1;
	}
	 last_read = ch;
}
void process_number(Token *token)
{
    int val = 0;
    int v_new, d;
	/* TODO:
	 * - Build number up to the specified maximum magnitude.
	 * - Store the value in the appropriatetoken field.
	 * - Set the appropriate token type.
	 * - "Remember" the correct column number globally.
	 */
	int col = column_number;
	while (isdigit(ch) && ch !=' ' && ch != '\n' && ch != EOF && val<=
		 INT_MAX) {
			 /*if ch is a number store it in d and culculate v_new which is
			 the value scanned so far with d added to it
			 */
				d = ch - '0';
				v_new = 10*val + d;
				val=v_new;
				next_char();
       }
       if (val > INT_MAX || val < 0) {
		   position.col=col;
		leprintf("number too large");
	   }
	 token->type = TOKEN_NUMBER;
	 token->value = val;
}

void process_string(Token *token)
{
	size_t i, nstring = MAX_INITIAL_STRING_LEN;
	/* TODO:
	 * - Allocate heap space of the size of the maximum initial string length.
	 * - If a string is *about* to overflow while scanning it, double the amount
	 *   of space available.
	 * - *Only* printable ASCII characters are allowed; see man 3 isalpha.
	 * - Check the legality of escape codes.
	 * - Set the appropriate token type.
	 */
	 int colmn, ln;
	 i = 0;
	 char *str = (char *)malloc(nstring*sizeof(char));
	 /*This while loop runs until it finds the end of string or end of file
		if the string is closed
	 */
	 int start = column_number-1;
	 if (isprint(ch)) {
		while (1) {
		 /* if it is a back slash character get the next char and check if
		 it is a valid escape sequence
		 */
			if (ch == '\\') {
				char bck_slash = ch;
				colmn = column_number;
				ln = position.line;
				next_char();
				if (ch == 'n' || ch == 't' || ch == '"' || ch == '\\') {
					if (i == nstring) {
						nstring = nstring*2;
						str = (char *)realloc(str,nstring*sizeof(char));
					}
				str[i] = bck_slash;
				i+=1;
				if (i == nstring) {
					nstring = nstring*2;
					str = (char *)realloc(str,nstring*sizeof(char));
				}
				str[i] = ch;
				next_char();
				i+=1;
             } else {
				position.col = colmn;
				position.line = ln;
				leprintf("illegal escape code '\\%c' in string ",ch);
				return;
             }
           } else if ((isprint(ch) || ch == '\n') && ch != '"') {
				if (i == nstring) {
					nstring = nstring*2;
					str = (char *)realloc(str,nstring*sizeof(char));
				}
				str[i] = ch;
				next_char();
                i+=1;
         }
		 if (isprint(ch) == 0 && ch != EOF) {
             int ascii = ch;
			 position.col = column_number;
         leprintf("non-printable character (ASCII #%d) in string",ascii);
         }
         else if (ch == '"') {
			 next_char();
         break;
         }
         else if (ch == EOF) {
			position.col = start;
			leprintf("string not closed");
         }
       }
	 } else if (isprint(ch) == 0) {
		 position.col = column_number;
		 leprintf("non-printable character (ASCII #%d) in string",ch);
	 }
     token->type = TOKEN_STRING;
	 token->string = str;
}

void process_word(Token *token)
{
	char lexeme[MAX_ID_LENGTH+1];
	int i, cmp, low, mid, high;
    int fnd = 0;
	position.col = column_number;
    int col = column_number;
	/* check that the id length is less than the maximum */
	/* TODO */
	i = 0;
	while (ch != EOF) {
		if (i <= MAX_ID_LENGTH && (isalpha(ch) || ch == '_' || isdigit(ch))) {
			lexeme[i] = ch;
			i+=1;
			next_char();
		} else {
			break;
		}
	}
	if (i > MAX_ID_LENGTH) {
	   position.col = col;
		leprintf("identifier too long");
     }
	/* do a binary search through the array of reserved words */
	/* TODO */
	lexeme[i] = '\0';
	high = NUM_RESERVED_WORDS;
	low = 0;
   while (high >= low) {
		mid = low + (high-low)/2;
		char *str2 = reserved[mid].word;
		cmp = strcmp(lexeme,str2);
		if (cmp == 0) {
			fnd = 1;
			break;
		}
		if (cmp < 0) {
			high = mid - 1;
		} else if (cmp > 0) {
			low = mid + 1;
		}
   }
	if (fnd == 1) {
	//IT IS A RESERVED WORD
    token->type = reserved[mid].type;
	} else if (fnd == 0) {
	/* if id was not recognised as a reserved word, it is an identifier */
	token->type = TOKEN_ID;
	strcpy(token->lexeme, lexeme);
	}
}

void skip_comment(void)
{
	SourcePos start_pos;
    position.col = column_number;
	start_pos.line = position.line;
	start_pos.col = position.col;
	/* TODO:
	 * - Skip nested comments *recursively*, which is to say, counting
	 *   strategies are not allowed.
	 * - Terminate with an error if comments are not nested properly.
	 */
	next_char();
	if (ch == '}') {
		return;
    }
    while (ch != '}') {
		if (ch == '{') {
			skip_comment();
		}
		if (ch == EOF) {
			position = start_pos;
			leprintf("comment not closed");
		}
		next_char();
    }
}
