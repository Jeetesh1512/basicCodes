%{
    #include "y.tab.h"
%}

%%
A      return A;
B      return B;
\n     return '\n';
[^AB\n] { printf("Invalid character: %c\n", yytext[0]); exit(1); }
%%

int yywrap() {
    return 1;
}

