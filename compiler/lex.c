#include "lex.h"
#include <ctype.h>
#include <stdio.h>

char *yytext = "";    // Lexeme
int yylen    = 0;     // Lexeme length
int yylineno = 0;     // Input Line Number

int lex(void) {
  static char input_buffer[128];
  char*       current;

  current = yytext + yylen;     // Skip current lexeme

  while (1) {
    while (*current) {
      /*
       * Get new lines, skipping any leading whitespace on the line,
       * until a non-blank line is found
       */
      current = input_buffer;
      if (!fgets(input_buffer, 128, stdin)) {
        *current = '\0';
        return EOI;
      }

      ++yylineno;

      while (isspace(*current)) {
        ++current;
      }
    }

    for (; *current; ++current) {
      // Get the next token
      yytext = current;
      yylen = 1;

      switch (*current) {
        case EOF: return EOI; 
        case ';': return SEMI;
        case '+': return PLUS;
        case '*': return TIMES;
        case '(': return LP;
        case ')': return RP;

        case '\n':
        case '\t':
        case ' ' : break;

        default:
          if (!isalnum(*current)) {
            fprintf(stdin, "Ignoring illegal input <%c>\n", *current);
          } else {
            while (isalnum(*current)) {
              ++current;
            }
            yylen = current - yytext;
            return NUM_OR_ID;
          }
          break;
      }
    }
  }
}

static int lookahead = -1;

int match(int token) {
  if (lookahead == -1) {
    lookahead = lex();
  }

  return token == lookahead;
}

void advance(void) {
  // Advance the lookahead to the next input symbol 
  lookahead = lex();
}
