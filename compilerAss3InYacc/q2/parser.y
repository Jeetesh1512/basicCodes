%{
    #include <stdio.h>
    #include <stdlib.h>

    int yylex();  // Declare yylex()
    void yyerror(const char *msg);  // Declare yyerror()
%}

%token A B

%%
S : SDash '\n' {printf("Valid string.\n"); exit(0);}
  ;

SDash : A SDash B
      | A B
      ;
%%

void yyerror(const char *msg) {
    printf("Invalid string\n");
    exit(1);
}

int main() {
    printf("Enter a string of A's followed by B's:\n");
    yyparse();
    return 0;
}

