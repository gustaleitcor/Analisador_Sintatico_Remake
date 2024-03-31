#pragma once

#include <cstddef>
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

struct Variable {
  Token token;
  Type semantic_type;
  size_t scope;
};

class Sintatico {
public:
  std::vector<Token> tokens;
  bool analyse();
  void saveToFile(std::string);
  TOKEN_ITERATOR token;

  std::vector<Variable> stack;
  size_t scope;

  void next();
  Token peek();

  bool program();

  bool variable_declaration();
  bool variable_declaration_list();
  bool variable_declaration_list2();
  bool identifiers_list();
  bool identifiers_list2();
  bool type();

  bool subprogram_declaration();
  bool subprograms_declarations();
  // bool subprograms_declarations2();
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

  bool checkInScope(Variable comp, size_t scope);
  bool checkIfExists(Variable var);
  bool evaluate(std::vector<Token> line);
  void removeScope(size_t scope);
  std::vector<Token> saveLine();
  Type getType();
};

Type stringToType(std::string s);
