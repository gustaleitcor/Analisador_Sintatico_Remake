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

  bool program(TOKEN_ITERATOR &token);

  bool variable_declaration(TOKEN_ITERATOR &token);
  bool variable_declaration_list(TOKEN_ITERATOR &token);
  bool variable_declaration_list2(TOKEN_ITERATOR &token);
  bool identifiers_list(TOKEN_ITERATOR &token);
  bool identifiers_list2(TOKEN_ITERATOR &token);
  bool type(TOKEN_ITERATOR &token);

  bool subprogram_declaration(TOKEN_ITERATOR &token);
  bool subprograms_declarations(TOKEN_ITERATOR &token);
  bool subprograms_declarations2(TOKEN_ITERATOR &token);
  bool arguments(TOKEN_ITERATOR &token);
  bool parameters_list(TOKEN_ITERATOR &token);
  bool parameters_list2(TOKEN_ITERATOR &token);
  bool compost_command(TOKEN_ITERATOR &token);
  bool optinals_command(TOKEN_ITERATOR &token);
  bool command_list(TOKEN_ITERATOR &token);
  bool command_list2(TOKEN_ITERATOR &token);
  bool command(TOKEN_ITERATOR &token);
  bool expression(TOKEN_ITERATOR &token);
  bool simple_expression(TOKEN_ITERATOR &token);
  bool simple_expression2(TOKEN_ITERATOR &token);
  bool term(TOKEN_ITERATOR &token);
  bool term2(TOKEN_ITERATOR &token);
  bool factor(TOKEN_ITERATOR &token);
  bool expression_list(TOKEN_ITERATOR &token);
  bool expression_list2(TOKEN_ITERATOR &token);
  bool else_part(TOKEN_ITERATOR &token);
  bool procedure_activation(TOKEN_ITERATOR &token);
};

Type stringToType(std::string s);
