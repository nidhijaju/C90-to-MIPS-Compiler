%code requires{
  #include <cassert>
  #include <string>
  #include "ast.hpp"
  
  extern BaseNode* g_root; // A way of getting the AST out

  extern FILE *yyin;

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);
}

%union{
  BaseNode* expr;
  std::string* string;
}


%token T_AUTO T_BREAK T_CASE T_CHAR T_CONST T_CONTINUE
%token T_DEFAULT T_DO T_DOUBLE T_ELSE T_ENUM T_EXTERN
%token T_FLOAT T_FOR T_GOTO T_IF T_INT T_LONG T_REGISTER
%token T_RETURN T_SHORT T_SIGNED T_SIZEOF T_STATIC T_STRUCT
%token T_SWITCH T_TYPEDEF T_UNION T_UNSIGNED T_VOID T_VOLATILE T_WHILE

%token T_LBRACKET T_RBRACKET T_LSQBRACKET T_RSQBRACKET
%token T_LCUBRACKET T_RCUBRACKET T_OBJREFERENCE T_COMMA T_POINTER

%token T_LOGNOT T_BWNOT T_PLUS T_MINUS T_INCREMENTER T_DECREMENTER
%token T_AND T_STAR T_DIVIDE T_MODULUS T_LEFTSHIFT T_RIGHTSHIFT
%token T_LESSTHAN T_GREATERTHAN T_LESSEQUAL T_GREATEREQUAL T_EQUALTO
%token T_NOTEQUALTO T_XOR T_OR T_LOGAND T_LOGOR T_CONDITIONAL T_ASSIGN
%token T_PLUSSUM T_MINUSSUM T_PRODSUM T_DIVSUM T_REMAINDER T_ANDASSIGN
%token T_ORASSIGN T_XORASSIGN T_LEFTSHIFTASSIGN T_RIGHTSHIFTASSIGN
%token T_ELLIPSIS T_QUESTIONMARK T_COLON T_SEMICOLON
%token IDENTIFIER
%token FLOATING_CONSTANT INTEGER_CONSTANT CHARACTER_CONSTANT
%token STRING_LITERAL HEADER_NAME PP_NUMBER

%nonassoc T_RBRACKET
%nonassoc T_ELSE


%type <string> unary_operator assignment_operator storage_class_specifier type_specifier struct_or_union type_qualifier IDENTIFIER 
              CHARACTER_CONSTANT INTEGER_CONSTANT FLOATING_CONSTANT STRING_LITERAL
              T_LOGNOT T_BWNOT T_PLUS T_MINUS T_INCREMENTER T_DECREMENTER
              T_AND T_STAR T_DIVIDE T_MODULUS T_LEFTSHIFT T_RIGHTSHIFT
              T_LESSTHAN T_GREATERTHAN T_LESSEQUAL T_GREATEREQUAL T_EQUALTO
              T_NOTEQUALTO T_XOR T_OR T_LOGAND T_LOGOR T_CONDITIONAL T_ASSIGN
              T_PLUSSUM T_MINUSSUM T_PRODSUM T_DIVSUM T_REMAINDER T_ANDASSIGN
              T_ORASSIGN T_XORASSIGN T_LEFTSHIFTASSIGN T_RIGHTSHIFTASSIGN
              T_AUTO T_BREAK T_CASE T_CHAR T_CONST T_CONTINUE
              T_DEFAULT T_DO T_DOUBLE T_ELSE T_ENUM T_EXTERN
              T_FLOAT T_FOR T_GOTO T_IF T_INT T_LONG T_REGISTER
              T_RETURN T_SHORT T_SIGNED T_SIZEOF T_STATIC T_STRUCT
              T_SWITCH T_TYPEDEF T_UNION T_UNSIGNED T_VOID T_VOLATILE T_WHILE T_QUESTIONMARK


%type <expr> primary_expression postfix_expression argument_expression_list unary_expression cast_expression
            multiplicative_expression additive_expression shift_expression relational_expression equality_expression and_expression
            exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression
            assignment_expression expression constant_expression declaration declaration_specifiers init_declarator_list init_declarator
            struct_or_union_specifier struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator enum_specifier
            enumerator_list enumerator direct_declarator pointer type_qualifier_list parameter_type_list parameter_list parameter_declaration
            identifier_list type_name abstract_declarator direct_abstract_declarator initialiser statement labeled_statement compound_statement
            declaration_list statement_list expression_statement iteration_statement jump_statement translation_unit external_declaration
            function_definition initialiser_list selection_statement declarator struct_declarator_list

%start translation_unit

%%

primary_expression: 
    IDENTIFIER                                                            { $$ = new Prime_Exp("ident", *($1)); } 
  | FLOATING_CONSTANT                                                     { $$ = new Prime_Exp("float", *($1)); } 
  | INTEGER_CONSTANT                                                      { $$ = new Prime_Exp("int", *($1)); } 
  | CHARACTER_CONSTANT                                                    { $$ = new Prime_Exp("char", *($1)); } 
  | STRING_LITERAL                                                        { $$ = new Prime_Exp("sl", *($1)); }          
  | T_LBRACKET expression T_RBRACKET                                      { $$ = $2; }             
;

postfix_expression: 
    primary_expression                                                      { $$ = $1; }  
  | postfix_expression T_LSQBRACKET expression T_RSQBRACKET                 { $$ = new Postfix_Exp($1, $3,"e"); }
  | postfix_expression T_LBRACKET T_RBRACKET                                { $$ = new Postfix_Exp($1, NULL,"empty"); }
  | postfix_expression T_LBRACKET argument_expression_list T_RBRACKET       { $$ = new Postfix_Exp($1, $3,"ael"); }
  | postfix_expression T_OBJREFERENCE IDENTIFIER                            { $$ = new Postfix_Exp($1, *($3)); }
  | postfix_expression T_INCREMENTER                                        { $$ = new BinaryOperator($1,*($2),NULL); }
  | postfix_expression T_DECREMENTER                                        { $$ = new BinaryOperator($1,*($2),NULL); }
;

argument_expression_list: 
    assignment_expression                                                   { $$ = new List($1,"ael"); }
  | argument_expression_list T_COMMA assignment_expression                  { $1->push($3); $$ = $1; }
;

unary_expression: 
    postfix_expression                            { $$ = $1; }
  | T_INCREMENTER unary_expression                { $$ = new BinaryOperator(NULL,*($1), $2); }
  | T_DECREMENTER unary_expression                { $$ = new BinaryOperator(NULL,*($1), $2); }
  | unary_operator cast_expression                { $$ = new BinaryOperator(NULL,*($1), $2); }
  | T_SIZEOF unary_expression                     { $$ = new BinaryOperator(NULL,*($1), $2); }
  | T_SIZEOF T_LBRACKET type_name T_RBRACKET      { $$ = new BinaryOperator(NULL,*($1), $3); }
;

unary_operator: 
    T_AND                     { $$ = $1; }
  | T_STAR                    { $$ = $1; }
  | T_PLUS                    { $$ = $1; }
  | T_MINUS                   { $$ = $1; }
  | T_BWNOT                   { $$ = $1; }
  | T_LOGNOT                  { $$ = $1; }
;

cast_expression: 
    unary_expression                                          { $$ = $1; }
  | T_LBRACKET type_name T_RBRACKET cast_expression           { $$ = $4; }
;

multiplicative_expression: 
    cast_expression                                           { $$ = $1; }
  | multiplicative_expression T_STAR cast_expression          { $$ = new BinaryOperator($1,*($2),$3); }
  | multiplicative_expression T_DIVIDE cast_expression        { $$ = new BinaryOperator($1,*($2),$3); }
  | multiplicative_expression T_MODULUS cast_expression       { $$ = new BinaryOperator($1,*($2),$3); }
;

additive_expression: 
    multiplicative_expression                                 { $$ = $1; }
  | additive_expression T_PLUS multiplicative_expression      { $$ = new BinaryOperator($1,*($2),$3); }
  | additive_expression T_MINUS multiplicative_expression     { $$ = new BinaryOperator($1,*($2),$3); }
;

shift_expression: 
    additive_expression                                       { $$ = $1; }
  | shift_expression T_LEFTSHIFT additive_expression          { $$ = new BinaryOperator($1,*($2),$3); }
  | shift_expression T_RIGHTSHIFT additive_expression         { $$ = new BinaryOperator($1,*($2),$3); }
;

relational_expression: 
    shift_expression                                          { $$ = $1; }
  | relational_expression T_LESSTHAN shift_expression         { $$ = new BinaryOperator($1,*($2),$3); }
  | relational_expression T_GREATERTHAN shift_expression      { $$ = new BinaryOperator($1,*($2),$3); }
  | relational_expression T_LESSEQUAL shift_expression        { $$ = new BinaryOperator($1,*($2),$3); }
  | relational_expression T_GREATEREQUAL shift_expression     { $$ = new BinaryOperator($1,*($2),$3); }
;

equality_expression: 
    relational_expression                                     { $$ = $1; }
  | equality_expression T_EQUALTO relational_expression       { $$ = new BinaryOperator($1,*($2),$3); }
  | equality_expression T_NOTEQUALTO relational_expression    { $$ = new BinaryOperator($1,*($2),$3); }
;

and_expression: 
    equality_expression                                       { $$ = $1; }
  | and_expression T_AND equality_expression                  { $$ = new BinaryOperator($1,*($2),$3); }
;

exclusive_or_expression: 
    and_expression                                            { $$ = $1; }
  | exclusive_or_expression T_XOR and_expression              { $$ = new BinaryOperator($1,*($2),$3); }
;

inclusive_or_expression: 
    exclusive_or_expression                                   { $$ = $1; }
  | inclusive_or_expression T_OR exclusive_or_expression      { $$ = new BinaryOperator($1,*($2),$3); }
;

logical_and_expression: 
    inclusive_or_expression                                   { $$ = $1; }
	| logical_and_expression T_LOGAND inclusive_or_expression   { $$ = new BinaryOperator($1,*($2),$3); }
;

logical_or_expression: 
    logical_and_expression                                    { $$ = $1; }
	| logical_or_expression T_LOGOR logical_and_expression      { $$ = new BinaryOperator($1,*($2),$3); }
;

conditional_expression: 
    logical_or_expression                                                           { $$ = $1; }
	| logical_or_expression T_QUESTIONMARK expression T_COLON conditional_expression  { $$ = new Selection(*($2),$1,$3,$5); }
;

assignment_expression: 
    conditional_expression                                      { $$ = $1; }
	| unary_expression assignment_operator assignment_expression  { $$ = new BinaryOperator($1,*($2),$3); }
;

assignment_operator:  
    T_ASSIGN                    { $$ = $1; }
  | T_PLUSSUM                   { $$ = $1; }
  | T_MINUSSUM                  { $$ = $1; }
  | T_PRODSUM                   { $$ = $1; }
  | T_DIVSUM                    { $$ = $1; }
  | T_REMAINDER                 { $$ = $1; }
  | T_ANDASSIGN                 { $$ = $1; }
  | T_ORASSIGN                  { $$ = $1; }
  | T_XORASSIGN                 { $$ = $1; }
  | T_LEFTSHIFTASSIGN           { $$ = $1; }
  | T_RIGHTSHIFTASSIGN          { $$ = $1; }
;

expression: 
    assignment_expression                     { $$ = new List($1,"e"); }
  | expression T_COMMA assignment_expression  { $1->push($3); $$ = $1; }
;

constant_expression: 
    conditional_expression            { $$ = $1; }
;

declaration: 
    declaration_specifiers T_SEMICOLON                        { $$ = new Declaration($1, NULL); }
  | declaration_specifiers init_declarator_list T_SEMICOLON   { $$ = new Declaration($1,$2); }
;

declaration_specifiers: 
    storage_class_specifier                           { $$ = new Declaration_Specifier(*($1), NULL); }
  | storage_class_specifier declaration_specifiers    { $$ = new Declaration_Specifier(*($1), $2); }
  | type_specifier                                    { $$ = new Declaration_Specifier(*($1), NULL); }  
  | type_specifier declaration_specifiers             { $$ = new Declaration_Specifier(*($1), $2); }    
  | type_qualifier                                    { $$ = new Declaration_Specifier(*($1), NULL); }
  | type_qualifier declaration_specifiers             { $$ = new Declaration_Specifier(*($1), $2); }
;

init_declarator_list: 
    init_declarator                                 { $$ = new List($1,"idl"); }
  | init_declarator_list T_COMMA init_declarator    { $1->push($3); $$ = $1; }
;

init_declarator: 
    declarator                                      { $$ = new Init_dec($1, NULL); }             
  | declarator T_ASSIGN initialiser                 { $$ = new Init_dec($1,$3); }
;

storage_class_specifier: 
    T_TYPEDEF                                       { $$ = $1; }
  | T_EXTERN                                        { $$ = $1; }
  | T_STATIC                                        { $$ = $1; }
  | T_AUTO                                          { $$ = $1; }
  | T_REGISTER                                      { $$ = $1; }
;

type_specifier: 
    T_VOID                                          { $$ = $1; }
  | T_CHAR                                          { $$ = $1; }
  | T_SHORT                                         { $$ = $1; }
  | T_INT                                           { $$ = $1; }
  | T_LONG                                          { $$ = $1; }
  | T_FLOAT                                         { $$ = $1; }
  | T_DOUBLE                                        { $$ = $1; }
  | T_SIGNED                                        { $$ = $1; }
  | T_UNSIGNED                                      { $$ = $1; }
;

struct_or_union_specifier: 
    struct_or_union IDENTIFIER T_LCUBRACKET struct_declaration_list T_RCUBRACKET                              { $$ = new Struct_Union(*($1), *($2), $4); }
  | struct_or_union T_LCUBRACKET struct_declaration_list T_RCUBRACKET                                         { $$ = new Struct_Union(*($1), "", $3); }
  | struct_or_union IDENTIFIER                                                                                { $$ = new Struct_Union(*($1), *($2), NULL); }
;

struct_or_union: 
    T_STRUCT                            { $$ = $1; }
  | T_UNION                             { $$ = $1; }
;

struct_declaration_list: 
    struct_declaration                                   { $$ = new List($1,"sdl1"); }
  | struct_declaration_list struct_declaration           { $1->push($2); $$ = $1; }
;

struct_declaration: 
    specifier_qualifier_list struct_declaration_list T_SEMICOLON     { $$ = new Declaration($1, $2); }
;

specifier_qualifier_list: 
    type_specifier specifier_qualifier_list                         { $2->push(*($1)); $$ = $2; }
	| type_specifier                                                  { $$ = new SList(*($1)); }
	| type_qualifier specifier_qualifier_list                         { $2->push(*($1)); $$ = $2;  }
	| type_qualifier                                                  { $$ = new SList(*($1)); }
;

struct_declarator_list: 
    struct_declarator                                   { $$ = new List($1,"sdl2"); }
	| struct_declarator_list T_COMMA struct_declarator    { $1->push($3); $$ = $1; }
;

struct_declarator: 
    declarator                                             { $$ = $1; }
  | T_COLON constant_expression                            { $$ = $2; }
  | declarator T_COLON constant_expression                 { $$ = new Declaration($1, $3); }
;

enum_specifier: 
    T_ENUM T_LCUBRACKET enumerator_list T_RCUBRACKET                   { $$ = new Enumerator("", $3); }
  | T_ENUM IDENTIFIER T_LCUBRACKET enumerator_list T_RCUBRACKET        { $$ = new Enumerator(*($2), $4); }
  | T_ENUM IDENTIFIER                                                  { $$ = new Enumerator(*($2), NULL); }
;

enumerator_list: 
    enumerator                                            { $$ = new List($1,"el"); }
  | enumerator_list T_COMMA enumerator                    { $1->push($3); $$ = $1; }
;

enumerator: 
    IDENTIFIER                                            { $$ = new Enumerator(*($1),NULL); }
  | IDENTIFIER T_ASSIGN constant_expression               { $$ = new Enumerator(*($1),$3); }
;

type_qualifier: 
    T_CONST                                               { $$ = $1; }
  | T_VOLATILE                                            { $$ = $1; }
;

declarator: 
    pointer direct_declarator                 { $$ = new Declaration($1,$2); }
  | direct_declarator                         { $$ = $1; }
;

direct_declarator: 
    IDENTIFIER                                                          { $$ = new Prime_Exp("ident", *($1));}      
  | T_LBRACKET declarator T_RBRACKET                                    { $$ = $2; }
  | direct_declarator T_LSQBRACKET constant_expression T_RSQBRACKET     { $$ = new Direct_Declarator($1,$3,"c"); }
  | direct_declarator T_LSQBRACKET T_RSQBRACKET                         { $$ = $1; }
  | direct_declarator T_LBRACKET parameter_type_list T_RBRACKET         { $$ = new Direct_Declarator($1,$3,"p"); }
  | direct_declarator T_LBRACKET identifier_list T_RBRACKET             { $$ = new Direct_Declarator($1,$3,"i"); }
  | direct_declarator T_LBRACKET T_RBRACKET                             { $$ = new Direct_Declarator($1,NULL,"empty"); }
;

pointer: 
    T_STAR                                  { $$ = new Pointer(NULL); }
  | T_STAR type_qualifier_list              { $$ = new Pointer($2); }
  | T_STAR pointer                          { $2->push(NULL); }
  | T_STAR type_qualifier_list pointer      { $3->push($2); }
;

type_qualifier_list: 
    type_qualifier                                  { $$ = new SList(*($1)); } 
  | type_qualifier_list type_qualifier              { $1->push(*($2)); $$ = $1; }
;

parameter_type_list: 
    parameter_list                                    { $$ = $1; }
  | parameter_list T_COMMA T_ELLIPSIS                 { $$ = $1; }
;

parameter_list: 
    parameter_declaration                             { $$ = new List($1,"pl"); }
  | parameter_list T_COMMA parameter_declaration      { $1->push($3); $$ = $1;  }
;

parameter_declaration: 
    declaration_specifiers declarator                       { $$ = new Declaration($1,$2); }
	| declaration_specifiers abstract_declarator              { $$ = new Declaration($1,$2); }
	| declaration_specifiers                                  { $$ = $1; }
;

identifier_list: 
    IDENTIFIER                            { $$ = new List(new Prime_Exp("ident", *($1)),"il"); }       
	| identifier_list T_COMMA IDENTIFIER    { $1->push(new Prime_Exp("ident", *($3))); $$ = $1;  }
;

type_name: 
    specifier_qualifier_list                          { $$ = new List($1,"tn"); }
	| specifier_qualifier_list abstract_declarator      { $$ = $1; $1->push($2); }
;

abstract_declarator: 
    pointer                                 { $$ = new Abs_dec($1, NULL); }
	| direct_abstract_declarator              { $$ = new Abs_dec(NULL, $1); }
	| pointer direct_abstract_declarator      { $$ = new Abs_dec($1, $2); }
;

direct_abstract_declarator: 
    T_LBRACKET abstract_declarator T_RBRACKET                                { $$ = $2; }               
  | T_LSQBRACKET T_RSQBRACKET                                                { $$ = NULL;}
  | T_LSQBRACKET constant_expression T_RSQBRACKET                            { $$ = $2; }
  | direct_abstract_declarator T_LSQBRACKET T_RSQBRACKET                     { $$ = new List(NULL,"dad"); }
  | direct_abstract_declarator T_LSQBRACKET constant_expression T_RSQBRACKET { $1->push($3); $$ = $1; }
  | T_LBRACKET T_RBRACKET                                                    { $$ = NULL;}
  | T_LBRACKET parameter_type_list T_RBRACKET                                { $$ = new List($2,"dad"); }
  | direct_abstract_declarator T_LBRACKET T_RBRACKET                         { $1->push(NULL); $$ = $1; }
  | direct_abstract_declarator T_LBRACKET parameter_type_list T_RBRACKET     { $1->push($3); $$ = $1; } 
; 

initialiser: 
    assignment_expression                                  { $$ = $1; }
  | T_LSQBRACKET initialiser_list T_RSQBRACKET             { $$ = $2; }
  | T_LSQBRACKET initialiser_list T_COMMA T_RSQBRACKET     { $$ = $2; }
;

initialiser_list: 
    initialiser                               { $$ = new List($1,"inl"); }
  | initialiser_list T_COMMA initialiser      { $1->push($3); $$ = $1; }
;

statement: 
    labeled_statement           { $$ = $1; }
  | compound_statement          { $$ = $1; }
  | expression_statement        { $$ = $1; }
  | selection_statement         { $$ = $1; }
  | iteration_statement         { $$ = $1; }
  | jump_statement              { $$ = $1; }
;

labeled_statement: 
    IDENTIFIER T_COLON statement                      { $$ = new Label(*($1),$3); }
  | T_CASE constant_expression T_COLON statement      { $$ = new Label(*($1),$2,$4); }
  | T_DEFAULT T_COLON statement                       { $$ = new Label(*($1),$3); }
;

compound_statement: 
    T_LCUBRACKET T_RCUBRACKET                                     { $$ = new Compound(NULL,NULL); }
  | T_LCUBRACKET statement_list T_RCUBRACKET                      { $$ = new Compound($2, NULL); }
  | T_LCUBRACKET declaration_list T_RCUBRACKET                    { $$ = new Compound(NULL, $2); }
  | T_LCUBRACKET declaration_list statement_list T_RCUBRACKET     { $$ = new Compound($3, $2); }
;

declaration_list: 
    declaration                                                   { $$ = new List($1,"dl"); }
  | declaration_list declaration                                  { $1->push($2); $$ = $1; }
;

statement_list: 
    statement                                                     { $$ = new List($1,"sl"); }
  | statement_list statement                                      { $1->push($2); $$ = $1; }
;

expression_statement: 
    T_SEMICOLON                                                   { $$ = NULL;}
  | expression T_SEMICOLON                                        { $$ = $1; }
;

selection_statement: 
    T_IF T_LBRACKET expression T_RBRACKET statement                       { $$ = new Selection(*($1), $3, $5, NULL); }
  | T_IF T_LBRACKET expression T_RBRACKET statement T_ELSE statement      { $$ = new Selection(*($1), $3, $5, $7); }
  | T_SWITCH T_LBRACKET expression T_RBRACKET statement                   { $$ = new Selection(*($1), $3, $5, NULL); }
;

iteration_statement: 
    T_WHILE T_LBRACKET expression T_RBRACKET statement                                            { $$ = new While_Loop($5,$3); }       
  | T_DO statement T_WHILE T_LBRACKET expression T_RBRACKET T_SEMICOLON                           { $$ = new While_Loop($2,$5); }
  | T_FOR T_LBRACKET expression_statement expression_statement T_RBRACKET statement               { $$ = new For_Loop($3,$4,NULL,$6); }
  | T_FOR T_LBRACKET expression_statement expression_statement expression T_RBRACKET statement    { $$ = new For_Loop($3,$4,$5,$7); }
;

jump_statement: 
    T_GOTO IDENTIFIER T_SEMICOLON               { $$ = new Jump(*($1),*($2)); }
  | T_CONTINUE T_SEMICOLON                      { $$ = new Jump(*($1)); }
  | T_BREAK T_SEMICOLON                         { $$ = new Jump(*($1)); }
  | T_RETURN T_SEMICOLON                        { $$ = new Jump(*($1)); }
  | T_RETURN expression T_SEMICOLON             { $$ = new Jump(*($1),$2); }
;

translation_unit: 
    external_declaration                        { g_root->push($1); }
	| translation_unit external_declaration       { g_root->push($2); }
;

external_declaration: 
    function_definition                         {  $$ = $1; }
	| declaration                                 {  $$ = $1; }
;

function_definition: 
    declaration_specifiers declarator declaration_list compound_statement                   { $$ = new Function_definition($1, $2, $3, $4); }
	| declaration_specifiers declarator compound_statement                                    { $$ = new Function_definition($1, $2, NULL, $3); }      
	| declarator declaration_list compound_statement                                          { $$ = new Function_definition(NULL, $1, $2, $3); }
	| declarator compound_statement                                                           { $$ = new Function_definition(NULL, $1, NULL, $2); }
;

%%

BaseNode* g_root; // Definition of variable (to match declaration earlier)

BaseNode* parseAST(std::string file){
  yyin  = fopen(file.c_str(), "r");       //Opens the input files
  if(yyin){
    g_root = new TopNode();
    yyparse();
  }
  return g_root;

  // yywrap() to read otther files as well
}
