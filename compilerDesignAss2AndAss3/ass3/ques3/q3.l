%{
    #include<stdio.h>
    #include<math.h>

    #define NUM     1
    #define ADD     2
    #define SUB     3
    #define MUL     4
    #define DIV     5
    #define MOD     6
    #define POW     7 
    #define EXIT    8
%}

%%
[0-9]+              {return NUM;}
"+"                 {return ADD;}
"-"                 {return SUB;}
"*"                 {return MUL;}
"/"                 {return DIV;}
"%"                 {return MOD;}
"^"                 {return POW;}
[eE][xX][iI][tT]    {return EXIT;}
[ \t\n]           ;
.              {printf("%s is invalid input.\n",yytext);
                    return 0;}
%%

int main()
{
    int n1,n2;
    char op;

    printf("\nSimple Calculator:\n\n");
    printf("-------------------------------------------------\n");
    printf("|    Enter the expression like 2+5 or 3*6       |\n");
    printf("-------------------------------------------------\n");
    printf("|     To exit enter exit(case-insensitive)      |\n");
    printf("-------------------------------------------------\n");
    printf("|   add(+), sub(-), mul(*), div(/), mod(%%) and  |\n|           pow(^) are supported                |\n");
    printf("-------------------------------------------------\n\n");

    while(1)
    {

        printf("$   ");

        int token=yylex();
        if(token==EXIT)
        {
            printf("\nExiting...\n");
            break;
        }
        if(token!=NUM)
        {
            printf("Error:Expected a number.\n");
            return 1;
        }

        n1=atoi(yytext);

        op=yylex();

        if(yylex()!=NUM)
        {
            printf("Error:Expected a number.\n");
            return 1;
        }
        n2=atoi(yytext);

        switch(op)
        {
            case ADD:
                printf("%d + %d = %d\n",n1,n2,n1+n2);
                break;
            case SUB:
                printf("%d - %d = %d\n",n1,n2,n1-n2);
                break;
            case MUL:
                printf("%d * %d = %d\n",n1,n2,n1*n2);
                break;
            case DIV:
                if(n2==0)
                {
                    printf("Error: Encountered Division by Zero.\n");
                }
                else
                {
                    printf("%d / %d = %d\n",n1,n2,n1/n2);
                }
                break;
            case MOD:
                if (n2==0)
                {
                    printf("Error: Modulo by zero.\n");
                } 
                else 
                {
                    printf("%d %% %d = %d\n", n1, n2, n1 % n2);    
                }
                break;

            case POW:
                if(n1==0 && n2==0)
                {
                    printf("Error: 0^0 is invalid\n");
                }
                else
                {
                    printf("%d ^ %d = %f\n", n1, n2, pow(n1, n2));
                }
                break;
            default:
                printf("Error:Invalid Operator.\n");
                break;
        }
    }
    return 0;
}

int yywrap()
{
    return 1;
}