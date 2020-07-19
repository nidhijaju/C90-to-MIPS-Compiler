%option noyywrap

%{
// Avoid error "error: `fileno' was not declared in this scope"
  extern "C" int fileno(FILE *stream);
  #include "c_parser.tab.hpp"

%}

IDENTIFIER [a-zA-Z][a-zA-Z0-9_]*

 //GENERAL
 //NOT A DIGIT
ND  [a-zA-Z_]
 //DIGIT 
D   [0-9]      
 //DIGIT SEQUENCE
DS  [{D}]+ 
HD  [0-9a-fA-F] 
OD  [0-7]      

 //FOR FLOATING CONSTANT
    //EXPONENT PART
EP  [eE][+-]?{D}+ 
    //FRACTIONAL CONST          
FRC {D}+[.]{D}*
    //FLOATINGPOINT SUFFIX     
FLS [flFL]                  

 //FOR INTEGER CONSTANT
INTEGER_SUFFIX ([uU][lL]?)|([lL][uU]?)
HEXADECIMAL_CONSTANT [0][xX][0-9a-fA-F]+
OCTAL_CONSTANT   [0][0-7]*
DECIMAL_CONSTANT [1-9][0-9]*

 //FOR CHARACTER CONSTANT
HEXADECIMAL_ESCAPE_SEQUENCE [\X]{HD}+
OCTAL_ESCAPE_SEQUENCE {OD}({OD}{OD}?)?
SIMPLE_ESCAPE_SEQUENCE [\'\"\?\\\a\b\f\n\r\t\v]
ESCAPE_SEQUENCE {HEXADECIMAL_ESCAPE_SEQUENCE}|{OCTAL_ESCAPE_SEQUENCE}|{SIMPLE_ESCAPE_SEQUENCE}
C_CHAR_SEQUENCE ([^(\'\\\N)]|ESCAPE_SEQUENCE)+


S_CHAR_SEQUENCE ({ESCAPE_SEQUENCE}|[^(\"\\\N])+

FRACTIONCONSTANT ([1-9][0-9]*\.([0-9]*)

NEWLINE (\r\n?|\n)

WHITESPACE [ ]

TAB \t

 //PREPROC

ALL .

%%

 /*Keywords*/
auto        {yylval.string = new std::string(yytext); return T_AUTO;}
break       {yylval.string = new std::string(yytext); return T_BREAK;}
case        {yylval.string = new std::string(yytext); return T_CASE;}
char        {yylval.string = new std::string(yytext); return T_CHAR;}
const       {yylval.string = new std::string(yytext); return T_CONST;}
continue    {yylval.string = new std::string(yytext); return T_CONTINUE;}
default     {yylval.string = new std::string(yytext); return T_DEFAULT;}
do          {yylval.string = new std::string(yytext); return T_DO;}
double      {yylval.string = new std::string(yytext); return T_DOUBLE;}
else        {yylval.string = new std::string(yytext); return T_ELSE;}
enum        {yylval.string = new std::string(yytext); return T_ENUM;}
extern      {yylval.string = new std::string(yytext); return T_EXTERN;}
float       {yylval.string = new std::string(yytext); return T_FLOAT;}
for         {yylval.string = new std::string(yytext); return T_FOR;}
goto        {yylval.string = new std::string(yytext); return T_GOTO;}
if          {yylval.string = new std::string(yytext); return T_IF;}
int         {yylval.string = new std::string(yytext); return T_INT;}
long        {yylval.string = new std::string(yytext); return T_LONG;}
register    {yylval.string = new std::string(yytext); return T_REGISTER;}
return      {yylval.string = new std::string(yytext); return T_RETURN;}
short       {yylval.string = new std::string(yytext); return T_SHORT;}
signed      {yylval.string = new std::string(yytext); return T_SIGNED;}
sizeof      {yylval.string = new std::string(yytext); return T_SIZEOF;}
static      {yylval.string = new std::string(yytext); return T_STATIC;}
struct      {yylval.string = new std::string(yytext); return T_STRUCT;}
switch      {yylval.string = new std::string(yytext); return T_SWITCH;}
typedef     {yylval.string = new std::string(yytext); return T_TYPEDEF;}
union       {yylval.string = new std::string(yytext); return T_UNION;}
unsigned       {yylval.string = new std::string(yytext); return T_UNSIGNED;}
void        {yylval.string = new std::string(yytext); return T_VOID;}
volatile    {yylval.string = new std::string(yytext); return T_VOLATILE;}
while       {yylval.string = new std::string(yytext); return T_WHILE;}

 /*Postfix Operators*/
\(		    {yylval.string = new std::string(yytext); return T_LBRACKET;}
\)		    {yylval.string = new std::string(yytext); return T_RBRACKET;}
\[		    {yylval.string = new std::string(yytext); return T_LSQBRACKET;}
\]		    {yylval.string = new std::string(yytext); return T_RSQBRACKET;}
\{		     {yylval.string = new std::string(yytext); return T_LCUBRACKET;}
\}		    {yylval.string = new std::string(yytext); return T_RCUBRACKET;}
\.		{yylval.string = new std::string(yytext); return T_OBJREFERENCE;} //MEMBER ACCESS OPERATOR
\,    {yylval.string = new std::string(yytext); return T_COMMA;} //ADDED COMMA
\-\>		{yylval.string = new std::string(yytext); return T_POINTER;}	//MEMBER ACCESS OPERATOR //Will this work, or is it or

 /*Unary Operators*/
\!		    {yylval.string = new std::string(yytext); return T_LOGNOT;}
\~ 		{yylval.string = new std::string(yytext); return T_BWNOT;}
\+         {yylval.string = new std::string(yytext); return T_PLUS;}
\-         {yylval.string = new std::string(yytext); return T_MINUS;}
\++		{yylval.string = new std::string(yytext); return T_INCREMENTER;}
\-\-		{yylval.string = new std::string(yytext); return T_DECREMENTER;}
\&         {yylval.string = new std::string(yytext); return T_AND;}
\*         {yylval.string = new std::string(yytext); return T_STAR;}
\/         {yylval.string = new std::string(yytext); return T_DIVIDE;}
\%         {yylval.string = new std::string(yytext); return T_MODULUS;}

 /*Bitwise Shift Operators*/
\<\<        {yylval.string = new std::string(yytext); return T_LEFTSHIFT;}
\>\>        {yylval.string = new std::string(yytext); return T_RIGHTSHIFT;}

 /*Relational Operators*/
\<         {yylval.string = new std::string(yytext); return T_LESSTHAN;}
\>         {yylval.string = new std::string(yytext); return T_GREATERTHAN;}
\<\=        {yylval.string = new std::string(yytext); return T_LESSEQUAL;}
\>\=        {yylval.string = new std::string(yytext); return T_GREATEREQUAL;}
\=\=        {yylval.string = new std::string(yytext); return T_EQUALTO;}
\!\=        {yylval.string = new std::string(yytext); return T_NOTEQUALTO;}

 /*Bitwise Operators*/
\^         {yylval.string = new std::string(yytext); return T_XOR;}
\|         {yylval.string = new std::string(yytext); return T_OR;}

 /*Logical Operators*/
\&\&        {yylval.string = new std::string(yytext); return T_LOGAND;}
\|\|        {yylval.string = new std::string(yytext); return T_LOGOR;}

 /*Assignment Operators*/
\=         {yylval.string = new std::string(yytext); return T_ASSIGN;}
\+\=        {yylval.string = new std::string(yytext); return T_PLUSSUM;}
\-\=        {yylval.string = new std::string(yytext); return T_MINUSSUM;}
\*\=        {yylval.string = new std::string(yytext); return T_PRODSUM;}
\/\=        {yylval.string = new std::string(yytext); return T_DIVSUM;}
\%\=        {yylval.string = new std::string(yytext); return T_REMAINDER;}
\&\=        {yylval.string = new std::string(yytext); return T_ANDASSIGN;}
\|\=        {yylval.string = new std::string(yytext); return T_ORASSIGN;}
\^\=        {yylval.string = new std::string(yytext); return T_XORASSIGN;}
\<\<\=       {yylval.string = new std::string(yytext); return T_LEFTSHIFTASSIGN;}
\>\>\=       {yylval.string = new std::string(yytext); return T_RIGHTSHIFTASSIGN;}

\.\.\.       {yylval.string = new std::string(yytext); return T_ELLIPSIS;}
\?         {yylval.string = new std::string(yytext); return T_QUESTIONMARK;}
\:         {yylval.string = new std::string(yytext); return T_COLON;}
\;         {yylval.string = new std::string(yytext); return T_SEMICOLON;}

{IDENTIFIER}       {yylval.string = new std::string(yytext); return IDENTIFIER;}

{FRC}{EP}?{FLS}?    {yylval.string = new std::string(yytext); return FLOATING_CONSTANT;}
{DS}{EP}{FLS}?      {yylval.string = new std::string(yytext); return FLOATING_CONSTANT;}

{DECIMAL_CONSTANT}{INTEGER_SUFFIX}?     {yylval.string = new std::string(yytext); return INTEGER_CONSTANT;}
{OCTAL_CONSTANT}{INTEGER_SUFFIX}?       {yylval.string = new std::string(yytext); return INTEGER_CONSTANT;}

{HEXADECIMAL_CONSTANT}{INTEGER_SUFFIX}? {yylval.string = new std::string(yytext); return INTEGER_CONSTANT;}



[L]?[']([^(\'\\\n)]|{ESCAPE_SEQUENCE})+[']     {yylval.string = new std::string(yytext); return CHARACTER_CONSTANT;}


[L]?[\"]{S_CHAR_SEQUENCE}?[\"] {yylval.string = new std::string(yytext); return STRING_LITERAL;}
([<][^(\n|>)]+[>])|(["][^(\n|")]+["]) {yylval.string = new std::string(yytext); return HEADER_NAME;}

{D}|([\.]{D})({D}|{ND}|[eE][+-]|[\.]) {yylval.string = new std::string(yytext); return PP_NUMBER;}

\/\/.* {;}
\/\*([^*]|[\n]|([\*]+([^*/]|[\n])))*[\*]+\/ {;}
[ \t\r\n]+  {;}

.           {fprintf(stderr, "Invalid token \n"); exit(1); }

%%

void yyerror (char const *s)
{
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
} 