%{
#include<stdio.h>
%}

%%
1[.]*101	{printf("%s is a valid string",yytext);}
.*		{printf("%s is an invalid string",yytext);}
%%

int main()
{
	printf("Enter the string: ");
	yylex();
	return 0;
}

int yywrap()
{
	return 1;
}
