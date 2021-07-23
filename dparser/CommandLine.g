{
#include <stdio.h>
#include "nodes.h"
#include "mystring.h"

typedef struct My_ParseNode {
    int type;
    string_t *strvalue;
} My_ParseNode;

#define D_ParseNode_User My_ParseNode

}
commandline:
    (commandlineWithGlobalOptions | commandLineWithoutGlobalOptions)
    { $$.type = ntCommandLine; }
    ;

commandlineWithGlobalOptions:
    (option*
     (WSPACE+ command)*)
     WSPACE*
    { $$.type = ntCommandLineGlobal; }
    ;

commandLineWithoutGlobalOptions:
    (WSPACE* command (WSPACE+ command)*)
    WSPACE*
    { $$.type = ntCommandLineNonGlobal; }
    ;

command:
    name option*
    { $$.type = ntCommand; }
    ;

option:
    WSPACE* HYPHEN HYPHEN? name 
    (EQUALS WSPACE* (value | qvalue | sqvalue)
     (WSPACE* COMMA WSPACE* (value | qvalue | sqvalue))*
    )?
    { $$.type = ntOption; }
    ;

name:
    NAME+
    { $$.type = ntName; }
    ;

value:
    (NAME | VALUECHARS)+
    { $$.type = ntValue; }
    ;

qvalue:
    DQUOTE
    { $$.strvalue = string_new(); }
    (SLASH |
     NAME |
     VALUECHARS |
     COMMA |
     WSPACE |
     HYPHEN |
     EQUALS |
     NULL |
     ASCII_CONTROLCHARS |
     SQUOTE |
     DQUOTE |
     UTF_VALUECHARS |
     EXTRASCII_VALUECHARS
    { 
        $$.strvalue = string_append($$.strvalue, $n0.start_loc.s, $n0.end);
        $$.type = ntQvalue; 
    })*
    DQUOTE
    ;

sqvalue:
    SQUOTE
    { $$.strvalue = string_new(); }
    (SLASH |
     NAME |
     VALUECHARS |
     COMMA |
     WSPACE |
     HYPHEN |
     EQUALS |
     NULL |
     ASCII_CONTROLCHARS |
     DQUOTE |
     SQUOTE SQUOTE |
     UTF_VALUECHARS |
     EXTRASCII_VALUECHARS
    {
        $$.strvalue = string_append($$.strvalue, $n0.start_loc.s, $n0.end);
        $$.type = ntSqvalue;
    }
    )*
    SQUOTE
    ;

NAME:
    "[a-zA-Z0-9_]+"
    ;

SLASH:
    u+002f
    ;

VALUECHARS:
    "[\u0021\u0023-\u0026\u0028-\u002b\u002e\u003a-\u003c\u003e-\u0040\u005b-\u005e\u0060\u007b-\u007e]+"
    ;

EXTRASCII_VALUECHARS:
    "[\u0080-\u00ff]+"
    ;

UTF_VALUECHARS:
    "[\u0100-\uffff]+"
    ;

ASCII_CONTROLCHARS:
    "[\u0002-\u001f]+"
    ;

EQUALS:
    u+003d
    ;

HYPHEN:
    u+002d
    ;

COMMA:
    u+002c
    ;

NULL:
    u+0000
    ;

SQUOTE:
    u+0027
    ;

DQUOTE:
    u+0022
    ;

WSPACE:
    "[ \t]"
    ;
