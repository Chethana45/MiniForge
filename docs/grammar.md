# MiniForge EBNF
```ebnf
program = declaration* ;
declaration = functionDecl | variableDecl | statement ;
functionDecl = "fn" IDENTIFIER "(" parameters? ")" block ;
variableDecl = "let" IDENTIFIER ("=" expression)? ";" ;
statement = "print" expression ";" | "return" expression? ";" |
            "if" "(" expression ")" statement ("else" statement)? |
            "while" "(" expression ")" statement | block | expression ";" ;
block = "{" declaration* "}" ;
expression = assignment ;
assignment = IDENTIFIER "=" assignment | logicalOr ;
logicalOr = logicalAnd ("or" logicalAnd)* ;
logicalAnd = equality ("and" equality)* ;
equality = comparison (("==" | "!=") comparison)* ;
comparison = term ((">" | ">=" | "<" | "<=") term)* ;
term = factor (("+" | "-") factor)* ;
factor = unary (("*" | "/" | "%") unary)* ;
unary = ("!" | "-") unary | call ;
call = primary ("(" arguments? ")" )* ;
primary = NUMBER | STRING | true | false | null | IDENTIFIER |
          "(" expression ")" | "[" arguments? "]" ;
```
Comments start with `//`. Statements end with semicolons.
