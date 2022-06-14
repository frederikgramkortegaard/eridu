%{
#include "y.tab.h"
#include <string.h>
 
extern int lineno;
extern int fileno();
%}

%option noyywrap nounput noinput yylineno

%%

[\n]+                  {lineno ++;};

\/\/.*?


    /* Start of Type keywords */

"int"                  { return INT;       }
"str"                  { return STR;       }
"char"                 { return CHAR;      }
"float"                { return FLOAT;     }
"callable"             { return CALLABLE;  }

"define"               { return DEFINE;    }
"return"               { return RETURN;    }
"while"                { return WHILE;     }
"break"                { return BREAK;     }
"continue"             { return CONTINUE;  }
"if"                   { return IF;        }

"struct"               { return STRUCT;    }
"cast"                 { return CAST;      }
"print"                { return PRINT;     }
"sizeof"               { return SIZEOF;    }
"castable"             { return CASTABLE;  }
"allocate"             { return ALLOCATE;  }
"addressof"            { return ADDRESSOF; }
"dereference"          { return DEREFERENCE; }

"<="                   { return LEQ;    }
">="                   { return GEQ;    }
"!="                   { return NEQUAL; }
"=="                   { return EQUAL;  }
"="                    { return '=';    }
"+"                    { return '+';    }
"-"                    { return '-';    }
"<"                    { return '<';    }
">"                    { return '>';    }
"!"                    { return "!";    }
"*"                    { return STAR;   }
"/"                    { return '/';    }
"%"                    { return '%';    }
"."                    { return '.';    }
","                    { return ',';    }
";"                    { return ';';    }
"("                    { return '(';    }
")"                    { return ')';    }
"{"                    { return '{';    }
"}"                    { return '}';    }
"["                    { return '[';    }
"]"                    { return ']';    }


    /* Escape Characters*/
'(\\0|\\r|\\n|\\t)'    { yylval.charval = 0; return CHARVAL;}

'.?'                   { yylval.charval = yytext[1]; return CHARVAL; }

    /* Floating points and the (-val) variant */
[0-9]+\.[0-9]+         { yylval.floatval  = atof(yytext); return FLOATVAL; }
\(-[0-9]+\.[0-9]+\)         {
    // Strip Parens
    yytext = yytext + 1;
     yytext[strlen(yytext) - 1] = '\0';
    yylval.floatval  = atof(yytext);
    return FLOATVAL; }


\"[^\"]*\"             { yylval.stringval = (char *) malloc(strlen(yytext) + 1); sprintf(yylval.stringval, "%s", yytext); return STRINGVAL;}
[a-zA-Z_][a-zA-Z0-9_]* { yylval.stringval = (char *) malloc(strlen(yytext)+1); sprintf(yylval.stringval,"%s",yytext); return ID; }

    /* Integers points and the (-val) variant */
[0-9]+                 { yylval.intval    = atoi(yytext); return INTVAL;}
\(-[0-9]+\)            {

    // Strip Parens
    yytext = yytext + 1;
    yytext[strlen(yytext) - 1] = '\0';
    yylval.intval = atoi(yytext);
    return INTVAL;
}


.       /* ignore */;
[ \t]+  /* ignore */;
%%


