%{
  /*
   * This is a comment
   */
%}

%option caseless

/*
The start token can be spelled BEGINING or BEGINNING
*/
START (BEGINING|BEGINNING)
/*
An assignment can take on two values, move or add
*/
ASSIGNMENT (MOVE|ADD)
/*
A declaration is marked by a size string of 1 or more consecutive Xs (followed by an identifier but this rule matches the size string only)
*/
DECLARATION X+
/* 
An identifier must start with at least one letter and can be followed by any combination of letters, numbers and -. 
If it is only 1 letter and the letter is X, it is considered a consecutive X and is therefore a declaration, not identifier
*/
IDENTIFIER [A-Z][A-Z0-9\-]*
/*
An integer can be 0-9 and have multiple characters in it
*/
INTEGER [0-9]+[0-9]*
/*
A literal string starts and ends with "". If the string contains " or \r\n, it is an invalid string. You can have an empty string as well
*/
STRING \"[^"\r\n]*\"
/*
Lines/statements end with the . character (End of Line)
*/
EOL (\.)
/*
A separator can separate multiple arguments in PRINT, INPUT
*/
SEPARATOR ;

%%
[\t \n\r]+ { /* ignore */ }

{START} { printf("%s: Marks the beginning of the program\n", yytext); }
BODY { printf("%s: Marks the body of the program\n", yytext); }
END { printf("%s: Marks the end of the program\n", yytext); }
{ASSIGNMENT} { printf("%s: Marks an assignment statement\n", yytext); }
TO { printf("%s: Marks the TO token of the assignment statement\n", yytext); }
INPUT { printf("%s: Marks an input statement\n", yytext); }
PRINT { printf("%s: Marks an output statement\n", yytext); } 
{SEPARATOR} { printf("%s: Marks a separator\n", yytext);}

{DECLARATION} { printf("%s: Marks an identifier declaration of size %ld\n", yytext, strlen(yytext)); }
{IDENTIFIER} { printf("%s: Marks an identifier\n", yytext); }
{INTEGER} { printf("%s: Marks an integer\n", yytext); }
{STRING} { printf("%s: Marks a literal string\n", yytext); }
{EOL} { printf("%s: Marks end of line\n\n", yytext); }

. { printf("%s: Invalid Token\n", yytext); }
%%

int yywrap() {}
int main(int argc, char **argv) {
  if (argc > 1) {
    FILE *fp = fopen(argv[1], "r");
    yyin = fp;
  }

  yylex();
}
