#include <stdio.h>

int lineno;
void yyparse();

int main() {
    yyparse();
    return 0;
}
