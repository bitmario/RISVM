import decimal
from ply import lex
#import sast

KEYWORDS = (
    "INT",

    "IF",
    "ELSE",
    "WHILE",
    # "FOR",
    "BREAK",
    "RETURN",
)

tokens = KEYWORDS + (
    # values
    "ID",
    "NUMBER",
    # "STRING",
    # operators
    "EQUALS",
    "PLUS",
    "MINUS",
    "MULTIPLY",
    "DIVIDE",
    "MODULO",
    "INCREMENT",
    "DECREMENT",
    # comparison operators
    "LT",
    "LE",
    "GT",
    "GE",
    "EQ",
    "NE",
    # logic
    "OR",
    "AND",
    "NOT",
    # bitwise
    "SHL",
    "SHR",
    "BAND",
    "BOR",
    "BXOR",
    "BNOT",
    # Delimeters ( ) [ ] { } , . ; :
    "LPAREN",
    "RPAREN",
    "RBRACE",
    "LBRACE",
    "SEMI",
    "COMMA",
)

t_ignore = " \t"


def t_NEWLINE(t):
    r"\n+"
    t.lexer.lineno += t.value.count("\n")


t_PLUS = r"\+"
t_MINUS = r"-"
t_MULTIPLY = r"\*"
t_DIVIDE = r"/"
t_MODULO = r"\%"
t_EQUALS = r"="
t_INCREMENT = r"\+\+"
t_DECREMENT = r"\-\-"

# bitwise
t_SHL = r"<<"
t_SHR = r">>"
t_BAND = r"&"
t_BOR = r"\|"
t_BXOR = r"\^"
t_BNOT = r"~"

# comparison
t_LT = r"<"
t_GT = r">"
t_LE = r"<="
t_GE = r">="
t_EQ = r"=="
t_NE = r"!="

# logic
t_OR = r"\|\|"
t_AND = r"&&"
t_NOT = r"!"

t_LPAREN = r"\("
t_RPAREN = r"\)"
t_LBRACE = r"\{"
t_RBRACE = r"\}"
t_SEMI = r";"
t_COMMA = r","

t_INT = r"int"

t_IF = r"if"
t_ELSE = r"else"
t_WHILE = r"while"
#t_FOR = r"for"
t_BREAK = r"break"
t_RETURN = r"return"
#t_PRINT = r"print"
#t_INPUT = r"input"
#t_STR = r"str"
#t_NUM = r"num"

reserved_map = {}
for r in KEYWORDS:
    reserved_map[r.lower()] = r


def t_ID(t):
    r"[A-Za-z_][\w_]*"
    t.type = reserved_map.get(t.value, "ID")
    return t


def t_NUMBER(t):
    r"[-+]?[0-9]+"
    t.value = int(t.value)
    if t.value > 2**31 - 1 or t.value < -2**31:
        raise ValueError("Invalid int32 {}".format(t.value))
    return t


# def t_STRING(t):
#     r"\"([^\\\n]|(\\.))*?\""
#     t.value = bytes(t.value[1:-1], "utf-8").decode("unicode_escape")
#     return t


def t_error(t):
    print("Illegal character %s" % repr(t.value[0]))
    t.lexer.skip(1)


lexer = lex.lex()
if __name__ == "__main__":
    lex.runmain(lexer)
