%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void yyerror(char* s);
int yylex();
%}

%token NUMBER EXIT
%left '+' '-'
%left '*' '/' '%'
%right '^'  
%left '(' ')'

%%
Input:
    | Input Line
    ;

Line:
    Expression '\n' { printf("Result: %d\n", $$); return 0; }
    | EXIT '\n' { printf("\nExiting...\n"); exit(0); }
    | '\n'  { return 0; }
    ;

Expression:
    Expression '+' Expression { $$ = $1 + $3; }
    | Expression '-' Expression { $$ = $1 - $3; }
    | Expression '*' Expression { $$ = $1 * $3; }
    | Expression '/' Expression {
        if ($3 == 0) {
            printf("Error: Division by zero.\n");
            YYERROR;
        } else {
            $$ = $1 / $3;
        }
    }
    | Expression '%' Expression {
        if ($3 == 0) {
            printf("Error: Modulo by zero.\n");
            YYERROR;
        } else {
            $$ = $1 % $3;
        }
    }
    | Expression '^' Expression { $$ = pow($1, $3); }
    | '(' Expression ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    ;

%%

int main() {
    printf("\nSimple Calculator using YACC & Lex:\n\n");
    printf("-------------------------------------------------\n");
    printf("|    Enter an expression like 2+5 or 3*6       |\n");
    printf("-------------------------------------------------\n");
    printf("|     To exit, enter 'exit' (case-insensitive)  |\n");
    printf("-------------------------------------------------\n");
    printf("|   Operators: +, -, *, /, %%, and ^            |\n");
    printf("-------------------------------------------------\n\n");

    while (1) {
        printf("$ ");
        yyparse();
    }

    return 0;
}

void yyerror(char* s) {
    printf("Error: Invalid Expression\n");
}