%{
#include<stdio.h>
#include<stdlib.h>
#include "y.tab.h"

#define NUMBER 258
#define EXIT 259

extern int yylval;
%}

%%

[0-9]+   { yylval = atoi(yytext); return NUMBER; }
[+\-*/%^()] { return yytext[0]; }
[eE][xX][iI][tT] { return EXIT; }
[ \t]    ;   
[\n]     { return '\n'; } 
.        { printf("'%s' is invalid input.\n", yytext); return 0; }

%%

int yywrap() { return 1; }
