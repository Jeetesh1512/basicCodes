%{
#include<stdio.h>
%}

%%
[A-Z]+([a-z]+|[0-9]*[.][0-9]+)#	{printf("%s is a valid string\n",yytext);}
.*	{printf("%s is an invalid string\n",yytext);}
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

