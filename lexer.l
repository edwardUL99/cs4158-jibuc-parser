%{
  #include "parser.tab.h"
  #include "jibuc.h"
  #include <string.h>
%}

%option caseless

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
An integer is ether multiple digits with the ffirst digit being 1-9 (no leading zeroes) or a single digit 0-9
*/
INTEGER [0-9]+
/*
A literal string starts and ends with "". If the string contains " or \r\n, it is an invalid string. You can have an empty string as well
*/
STRING \"[^"]*\"
/*
Lines/statements end with the . character (End of Line)
*/
EOL \.
/*
A separator can separate multiple arguments in PRINT, INPUT
*/
SEPARATOR ;

%%
[\t \n\r]+ { /* ignore */ }

BEGINING { return(BEGINING); }
BEGINNING { return(BEGINNING); }
BODY { return(BODY); }
END { return(END); }
MOVE { return(MOVE); }
ADD { return(ADD); }
TO { return(TO); }
INPUT { return(INPUT); }
PRINT { return(PRINT); } 
{SEPARATOR} { return(SEPARATOR); }

{DECLARATION} { 
  yylval.decl = strlen(yytext);
  return(DECLARATION); 
}
{IDENTIFIER} { 
  yylval.ident = identifier(yytext);
  return(IDENTIFIER); 
}
{INTEGER} {
  yylval.number = integer(yytext);

  return(INTEGER); 
}
{STRING} { return(STRING); }
{EOL} { return(EOL); }

. { return *yytext; }
%%
