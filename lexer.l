%{
  /*
   * This is a comment
   */
%}

%option caseless

%%
[\t ]+ { /* ignore */ }

BEGINNING { printf("%s: Marks the beginning of the program\n", yytext); }
BODY { printf("%s: Marks the body of the program\n", yytext); }
END { printf("%s: Marks the end of the program\n", yytext); }
MOVE |
ADD { printf("%s: Marks an assignment statement\n", yytext); }
INPUT { printf("%s: Marks an input statement\n", yytext); }
PRINT { printf("%s: Marks an output statement\n", yytext); } 
; { printf("%s: Marks a separator\n", yytext);}
TO { printf("%s: Marks the TO token of the assignment statement\n", yytext); }

X+ { printf("%s: Marks an identifier declaration of size %ld\n", yytext, strlen(yytext)); }
[A-Z][A-Z0-9\-]* { printf("%s: Marks an identifier\n", yytext); }
[0-9]+ { printf("%s: Marks an integer\n", yytext); }
\"[^;"\r\n.]+\" { printf("%s: Marks a literal string\n", yytext); }
(\r\n)\. |
\n\. |
\. { printf("%s: Marks end of line\n", yytext); }
\n |
\r\n { printf(": Marks new line\n"); }
. { printf("%s: Unknown value\n", yytext); }
%%

int yywrap() {}
int main(int argc, char **argv) {
  if (argc > 1) {
    FILE *fp = fopen(argv[1], "r");
    yyin = fp;
  }

  yylex();
}
