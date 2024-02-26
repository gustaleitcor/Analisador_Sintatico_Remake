#pragma once

#include <string>
#include <vector>

#define TOKEN_ITERATOR std::vector<Token>::iterator

enum Type {
  REAL,
  INTEGER,
  BOOLEAN,
  RELACIONAL_OPERATOR,
  LOGICAL_OPERATOR,
  OPERATOR,
  KEYWORD,
  IDENTIFIER,
  DELIMITER,
  ASSIGN,
  EXPRESSION,
  PROGRAM,
  PROCEDURE,
};

struct Token {
  Type type;
  std::string name;
  unsigned long line;
};

class Sintatico {
public:
  std::vector<Token> tokens;
  bool analyse();
  void saveToFile(std::string);
  TOKEN_ITERATOR token;

  void next();

  bool program();

  bool variable_declaration();
  bool variable_declaration_list();
  bool variable_declaration_list2();
  bool identifiers_list();
  bool identifiers_list2();
  bool type();

  bool subprogram_declaration();
  bool subprograms_declarations();
  bool subprograms_declarations2();
  bool arguments();
  bool parameters_list();
  bool parameters_list2();
  bool compost_command();
  bool optinals_command();
  bool command_list();
  bool command_list2();
  bool command();
  bool expression();
  bool simple_expression();
  bool simple_expression2();
  bool term();
  bool term2();
  bool factor();
  bool expression_list();
  bool expression_list2();
  bool else_part();
  bool procedure_activation();
};

Type stringToType(std::string s);
