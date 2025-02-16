%{
    #include <stdio.h>
    #include <stdlib.h>

    int yylex();  
    void yyerror(const char *msg);
%}

%token A B

%%
S : A_seq B_seq '\n' { printf("Valid string\n"); exit(0); }
  ;

A_seq : A A_seq
      | A 
      ;

B_seq : B B_seq
      | B
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

