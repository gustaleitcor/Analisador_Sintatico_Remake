#include "Sintatico.h"
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <exception>
#include <iostream>
#include <iterator>
#include <ostream>
#include <vector>

std::string typeToString(Type t);
Type stringToType(std::string s);

void Sintatico::saveToFile(std::string){};

void Sintatico::next() {
  if ((++this->token - tokens.begin()) >= tokens.size()) {
    throw std::exception();
  }

  // std::cout << token->name << std::endl;
}

Token Sintatico::peek() {
  if ((this->token - tokens.begin() + 1) >= tokens.size()) {
    throw std::exception();
  }

  return *(token + 1);
}

// Coisas do semântico
// while (b = 1 + 2 + 3)

// OI MARCELO!!!!

std::vector<Token> Sintatico::saveLine(std::string s) {
  std::vector<Token> line;
  for (auto it = token; it->name != s; it++) {
    line.push_back(*it);
  }
  return line;
}

bool Sintatico::evaluate(std::vector<Token> line,
                         int option) { // i = 1, assign i = 2, while/if
  for (int j = line.size() - 1; j >= 0; j--) {
    if ((line[j].type == Type::DELIMITER) || (line[j].type == Type::KEYWORD)) {
      if ((line[j].type == Type::KEYWORD) &&
          (line[j + 1].type != Type::BOOLEAN)) {
        return false;
      }
      line.erase(line.begin() + j);
      continue;
    }
    if (line[j].type == Type::IDENTIFIER) {
      for (int i = stack.size() - 1; i >= 0; i--) {
        if (stack[i].token.name == line[j].name) {
          line[j].type = stack[i].semantic_type;
        }
      }
    }
  }

  // a == b != c
  // a := b + c
  // a := INTEGER
  // ASSIGN

  // a + b != c + d

  // ab != cd
  // return false

  // bool
  // a + b
  // INTEGER

  // var a,b : integer;
  // while(a + b); => while(INTEGER);
  // while(INTEGER); != while(BOOLEAN);
  // while(a+b == c+d)

  Type V1, V2, V3;
  Type result;
  if (line[0].type == PROCEDURE) {
    for (auto val : line) {
      if (val.type == Type::ASSIGN) {
        return false;
      }
    }
    return true;
  }

  if (option == 2) { // while e if

    for (int j = line.size() - 3; j >= 0; j--) {

      if (line[j + 1].type == OPERATOR) {
        if (line[j].type == REAL || line[j + 2].type == REAL) {
          result = REAL;
        } else if (line[j].type == INTEGER && line[j + 2].type == INTEGER) {
          result = INTEGER;
        } else {
          return false;
        }
        std::cout << typeToString(line[j + 1].type) << ' ' << j << std::endl;

        line.erase(line.begin() + j);
        line.erase(line.begin() + j);
        line.erase(line.begin() + j);
        line.insert(line.begin() + j, {result});
      }
    }

    for (int j = line.size() - 3; j >= 0; j--) {
      if (line[j + 1].type == RELACIONAL_OPERATOR) {

        if (line[j].type == REAL || line[j + 2].type == REAL) {
          result = BOOLEAN;
        } else if (line[j].type == INTEGER && line[j + 2].type == INTEGER) {
          result = BOOLEAN;
        } else {
          return false;
        }

        line.erase(line.begin() + j);
        line.erase(line.begin() + j);
        line.erase(line.begin() + j);
        line.insert(line.begin() + j, {result});
      }
    }

    for (int j = line.size() - 3; j >= 0; j--) {
      if (line[j + 1].type == LOGICAL_OPERATOR) {

        if (line[j].type == BOOLEAN && line[j + 2].type == BOOLEAN) {
          result = BOOLEAN;
        } else {
          return false;
        }

        line.erase(line.begin() + j);
        line.erase(line.begin() + j);
        line.erase(line.begin() + j);
        line.insert(line.begin() + j, {result});
      }
    }

    if (line.size() == 1 && line[0].type == BOOLEAN) {
      return true;
    } else {
      return false;
    }
  }

  // Verifica :=
  if (line[1].type == Type::ASSIGN) {
    for (int i = line.size() - 1; i >= 2; i -= 2) {
      if (line[i - 1].type != Type::OPERATOR) {
        if (line[i - 1].type == Type::ASSIGN) {
          break;
        }
        return false;
      }
      V1 = line[i].type;
      V2 = line[i - 2].type;
      line.pop_back();
      line.pop_back();
      line.pop_back();

      if (V1 == REAL || V2 == REAL) {
        result = REAL;
      } else if (V1 == INTEGER && V2 == INTEGER) {
        result = INTEGER;
      } else {
        return false;
      }

      line.push_back({result});
    }

    if (line[0].type == REAL && line[2].type == INTEGER) {
      return true;
    } else if (line[0].type == INTEGER && line[2].type == INTEGER) {
      return true;
    } else if (line[0].type == REAL && line[2].type == REAL) {
      return true;
    } else if (line[0].type == BOOLEAN && line[2].type == BOOLEAN) {
      return true;
    } else {
      return false;
    }
  }

  return false;
}

// B - REAL , C - INT, D - INT, A - INT

// A := B + (C + D);

Type Sintatico::getType() {
  TOKEN_ITERATOR it;
  for (it = token; it->name != ":"; it++)
    ;

  if ((it + 1)->name == "integer") {
    return Type::INTEGER;
  }

  if ((it + 1)->name == "real") {
    return Type::REAL;
  }

  if ((it + 1)->name == "boolean") {
    return Type::BOOLEAN;
  }

  throw std::exception();
}

bool Sintatico::checkInScope(Variable comp, size_t scope) {
  // std::cout << stack.size() << std::endl;

  for (auto var : stack) {
    if (var.scope == scope && comp.token.name == var.token.name) {
      std::cout << "variable " << comp.token.name
                << " alredy declared. line: " << comp.token.line << std::endl;
      return true;
    }
  }
  // std::cout << comp.scope << ' ' << comp.token.name << std::endl;

  return false;
}

bool Sintatico::checkIfExists(Variable var) {
  for (auto stack_var : stack) {
    if (stack_var.token.name == var.token.name &&
        stack_var.scope <= var.scope) {
      return true;
    }
  }
  return false;
}

void Sintatico::removeScope(size_t scope) {
  for (int i = stack.size() - 1; i >= 0; i--) {
    if (stack[i].scope >= scope) {
      stack.pop_back();
    }
  }
}

// Sintatico

bool Sintatico::analyse() {
  token = this->tokens.begin();
  scope = 0;
  bool result = this->program();

  // std::cout << "end fo analisis" << std::endl;

  // for (auto var : stack) {
  //   std::cout << var.scope << ' ' << var.token.name << std::endl;
  // }

  return result;
};

bool Sintatico::program() {
  if (token->name == "program") {
    next();
    if (token->type == Type::IDENTIFIER) {

      if (!checkInScope({*token, Type::PROGRAM, scope}, scope)) {
        stack.push_back({*token, Type::PROGRAM, scope});
      } else {
        std::cout << "Variable alredy declared. Line: " << token->line
                  << std::endl;
        return false; // Variable alredy declared
      }

      next();
      if (token->name == ";") {

        if (!variable_declaration()) {
          std::cout << "variable_declaration() FAILED. Line: " << token->line
                    << std::endl;
          return false; // variable_declaration FAILED
        }

        if (!subprograms_declarations()) {
          std::cout << "subprograms_declarations() FAILED. Line: "
                    << token->line << std::endl;
          return false; // subprograms_declarations FAILED
        }

        next();

        scope++;

        if (compost_command()) {
          next();
          if (token->name == ".") {
            // std::cout << "fim" << std::endl;
            scope--;
            removeScope(scope);
            return true;
          } else {
            std::cout << "Expected: . Line: " << token->line << std::endl;
            return false; // Expected '.'
          }
        } else {
          std::cout << "compost_command() FAILED. Line: " << token->line
                    << std::endl;
          return false; // compost_command FAILED
        }
      } else {
        std::cout << "Expected: ;. Line: " << token->line << std::endl;
        return false; // Expected ';'
      }
    } else {
      std::cout << "Expected: IDENTIFIER. Line: " << token->line << std::endl;
      return false; // Expected IDENTIFIER
    }
  } else {
    std::cout << "Expected: program. Line: " << token->line << std::endl;
    return false; // Expected program
  }
  std::cout << "program() FAILED. Line: " << token->line << std::endl;
  return false;
}

bool Sintatico::variable_declaration() {
  if (peek().name == "var") {
    next();
    if (token->name == "var") {
      next();
      if (variable_declaration_list()) {
        return true;
      } else {
        std::cout << "variable_declaration_list() FAILED. Line: " << token->line
                  << std::endl;
        return false; // variable_declaration_list FAILED
      }
    }
  }

  return true; // ε
}

bool Sintatico::variable_declaration_list() {
  if (identifiers_list()) {
    next();
    if (token->name == ":") {
      next();
      if (type()) {
        next();
        if (token->name == ";") {
          return variable_declaration_list2();
        } else {
          std::cout << "Expected: ;. Line: " << token->line << std::endl;
          return false; // Expected ';'
        }
      } else {
        std::cout << "type() FAILED. Line: " << token->line << std::endl;
        return false; // type FAILED
      }
    } else {
      std::cout << "Expected: ;. Line: " << token->line << std::endl;
      return false; // Expected ';'
    }
  } else {
    std::cout << "identifiers_list() FAILED. Line: " << token->line
              << std::endl;
    return false; // identifiers_list FAILED
  }
  std::cout << "variable_declaration_list() FAILED. Line: " << token->line
            << std::endl;
  return false; // variable_declaration_list FAILED
}

bool Sintatico::variable_declaration_list2() {
  if (peek().type == Type::IDENTIFIER) {
    next();
    if (identifiers_list()) {
      next();
      if (token->name == ":") {
        next();
        if (type()) {
          next();
          if (token->name == ";") {
            return variable_declaration_list2();
          } else {
            std::cout << "Expected: ;. Line: " << token->line << std::endl;
            return false; // Expected ';'
          }
        } else {
          std::cout << "type() FAILED. Line: " << token->line << std::endl;
          return false; // type FAILED
        }
      } else {
        std::cout << "Expected: :. Line: " << token->line << std::endl;
        return false; // Expected ':'
      }
    } else {
      // std::cout << "indentifiers_list() FAILED. Line: " << token->line
      //           << std::endl;
      return false; // identifiers_list FAILED
    }
  }

  return true; // ε
}

bool Sintatico::identifiers_list() {
  if (token->type == Type::IDENTIFIER) {

    if (!checkInScope({*token, getType(), scope}, scope)) {
      stack.push_back({*token, getType(), scope});
    } else {
      std::cout << "Variable alredy declared. Line: " << token->line
                << std::endl;
      return false; // Variable alredy declared
    }

    return identifiers_list2();
  }

  // std::cout << "indentifiers_list() FAILED. Line: " << token->line <<
  // std::endl;
  return false;
}

bool Sintatico::identifiers_list2() {
  if (peek().name == ",") {
    next();
    if (token->name == ",") {
      next();
      if (token->type == Type::IDENTIFIER) {

        if (!checkInScope({*token, getType(), scope}, scope)) {
          stack.push_back({*token, getType(), scope});
        } else {
          std::cout << "Variable alredy declared. Line: " << token->line
                    << std::endl;
          return false; // Variable alredy declared
        }

        return identifiers_list2();
      } else {
        std::cout << "Expected: IDENTIFIER. Line: " << token->line << std::endl;
        return false; // Expected IDENTIFIER
      }
    } else {
      std::cout << "Expected: ,. Line: " << token->line << std::endl;
      return false; // Expected ','
    }
  }
  return true; // ε
}

bool Sintatico::type() {
  if (token->name == "integer") {
    return true;
  }

  if (token->name == "real") {
    return true;
  }

  if (token->name == "boolean") {
    return true;
  }

  std::cout << "type() FAILED. Line: " << token->line << std::endl;

  return false; // type FAILED
}

bool Sintatico::subprograms_declarations() {
  if (peek().name == "procedure") {
    next();
    if (subprogram_declaration()) {
      next();
      if (token->name == ";") {
        return subprograms_declarations();
      } else {
        std::cout << "Expected: ;. Line: " << token->line << std::endl;
        return false; // Expected ';'
      }
    } else {
      std::cout << "subprogram_declaration FAILED Line: " << token->line
                << std::endl;
      return false; // subprogram_declaration FAILED
    }
  }

  return true; // ε
}

bool Sintatico::subprogram_declaration() {
  if (token->name == "procedure") {
    next();
    if (token->type == Type::IDENTIFIER) {

      if (!checkInScope({*token, Type::PROCEDURE, scope}, scope)) {
        stack.push_back({*token, Type::PROCEDURE, scope});
      } else {
        std::cout << "Variable alredy declared. Line: " << token->line
                  << std::endl;
        return false; // Variable alredy declared
      }

      scope++;

      arguments();

      next();

      if (token->name == ";") {

        if (!variable_declaration()) {
          std::cout << "variable_declaration FAILED Line: " << token->line
                    << std::endl;
          return false; // variable_declaration FAILED
        }

        if (!subprograms_declarations()) {
          std::cout << "subprograms_declarations FAILED Line: " << token->line
                    << std::endl;
          return false; // subprograms_declarations FAILED
        }

        next();

        if (compost_command()) {
          removeScope(scope);
          scope--;
          return true;
        } else {
          std::cout << "compost_command() FAILED. Line: " << token->line
                    << std::endl;
          return false; // compost_command FAILED
        }
      } else {
        std::cout << "Expected: ;. Line: " << token->line << std::endl;
        return false; // Expected ';'
      }
    }
  }

  std::cout << "subprogram_declaration() FAILED. Line: " << token->line
            << std::endl;
  return false; // subprogram_declaration FAILED
}

bool Sintatico::parameters_list() {
  if (identifiers_list()) {
    next();
    if (token->name == ":") {
      next();
      if (type()) {
        return parameters_list2();
      } else {
        std::cout << "type() FAILED. Line: " << token->line << std::endl;
        return false; // type FAILED
      }
    } else {
      std::cout << "Expected: :. Line: " << token->line << std::endl;
      return false; // Expected ':'
    }
  } else {
    std::cout << "identifiers_list() FAILED. Line: " << token->line
              << std::endl;
    return false; // identifiers_list FAILED
  }

  std::cout << "parameters_list() FAILED. Line: " << token->line << std::endl;
  return false; // parameters_list FAILED
}

bool Sintatico::parameters_list2() {

  if (peek().name == ";") {
    next();
    if (token->name == ";") {
      next();
      if (identifiers_list()) {
        next();
        if (token->name == ":") {
          next();
          if (type()) {
            return parameters_list2();
          } else {
            std::cout << "type() FAILED Line: " << token->line << std::endl;
            return false; // type FAILED
          }
        }
      } else {
        std::cout << "identifiers_list() FAILED Line: " << token->line
                  << std::endl;
        return false; // identifiers_list FAILED
      }
    }
  }

  return true; // ε
}

bool Sintatico::arguments() {
  if (peek().name == "(") {
    next();
    if (token->name == "(") {
      next();
      if (parameters_list()) {
        next();
        if (token->name == ")") {
          return true;
        } else {
          std::cout << "Expected: }. Line: " << token->line << std::endl;
          return false; // Expected '}'
        }
      } else {
        std::cout << "parameters_list() FAILED. Line: " << token->line
                  << std::endl;
        return false; // parameters_list FAILED
      }
    }
  }

  return true; // ε
}

bool Sintatico::compost_command() {
  if (token->name == "begin") {

    if (!optinals_command()) {
      std::cout << "optinals_command() FAILED. Line: " << token->line
                << std::endl;
      return false; // optinals_command FAILED
    }

    next();

    if ((token - 1)->name == "end") {
      token--;
    }

    std::cout << "debuga!" << token->name << std::endl;

    if (token->name == "end") { // gambiarra
      return true;
    } else {
      std::cout << "Expected: end. Line: " << token->line << std::endl;
      return false; // Expected 'END'
    }
  } else {
    std::cout << "Expected: begin. Line: " << token->line << std::endl;
    return false; // Expected 'BEGIN'
  }

  std::cout << "arguments() FAILED. Line: " << token->line << std::endl;
  return false; // arguments FAILED
}

bool Sintatico::optinals_command() {
  if (peek().type == Type::IDENTIFIER || peek().name == "begin" ||
      peek().name == "if" || peek().name == "while") {
    next();
    if (command_list()) {
      return true;
    } else {
      std::cout << "command_list() FAILED. Line: " << token->line << std::endl;
      return false; // command_list FAILED
    }
  }

  return true; // ε
}

bool Sintatico::command_list() {
  if (command()) {
    return command_list2();
  } else {
    std::cout << "command() FAILED. Line: " << token->line << std::endl;
    return false; // command FAILED
  }

  std::cout << "compost_list() FAILED. Line: " << token->line << std::endl;
  return false; // command_list FAILED
}

bool Sintatico::command_list2() {

  if (peek().name == ";") {
    next();
    if (token->name == ";") {
      if (peek().name == "end") { // gambiarra
        return true;
      }
      next();
      if (command()) {
        return command_list2();
      } else {
        std::cout << "command() FAILED. Line: " << token->line << std::endl;
        return false; // command FAILED
      }
    }
  }

  if (token->name == "end") {
    return true;
  }

  return false; // ε
}

bool Sintatico::command() {

  if (token->type == Type::IDENTIFIER) {

    if (!evaluate(saveLine(";"), 1)) {
      std::cout << "Tipo conflitante. Line: " << token->line << std::endl;
      return false; // Expression not correct
    }

    if (!checkIfExists({*token, Type::IDENTIFIER, scope})) {
      std::cout << "variable does not exists. Line: " << token->line
                << std::endl;
      return false; // Variable does not exists
    }

    if (peek().name == ":=") {
      next();
      if (token->name == ":=") {
        next();
        if (expression()) {
          return true;
        } else {
          std::cout << "expression() FAILED. Line: " << token->line
                    << std::endl;
          return false; // expression FAIlED
        }
      }
    }

    if (procedure_activation()) {
      return true;
    } else {
      std::cout << "procedure_activation() FAILED. Line: " << token->line
                << std::endl;
      return false; // procedure_activation FAILED
    }
  }

  if (token->name == "begin") {
    scope++;
    if (compost_command()) {
      scope--;
      return true;
    } else {
      std::cout << "compost_command() FAILED. Line: " << token->line
                << std::endl;
      return false; // compost_command FAILED
    }
  }

  if (token->name == "if") {
    next();

    if (!evaluate(saveLine("then"), 2)) {
      std::cout << "Tipo conflitante. Line: " << token->line << std::endl;
      return false;
    }

    if (expression()) {
      next();
      if (token->name == "then") {
        next();
        if (command()) {
          return else_part();
        } else {
          std::cout << "command() FAILED. line: " << token->line << std::endl;
          return false; // command FAILED
        }
      } else {
        std::cout << "Expected 'then'. line: " << token->line << std::endl;
        return false; // Expected 'then'
      }
    } else {
      std::cout << "expression() FAILED. line: " << token->line << std::endl;
      return false; // expression FAILED
    }
  }

  if (token->name == "while") {
    next();
    if (!evaluate(saveLine("do"), 2)) {
      std::cout << "tipo conflitante line: " << token->line << std::endl;
      return false;
    }
    if (expression()) {
      next();
      if (token->name == "do") {
        next();
        if (command()) {
          return true;
        } else {
          std::cout << "command() FAILED. line: " << token->line << std::endl;
          return false; // command FAILED
        }
      } else {
        std::cout << "Expected 'do' line: " << token->line << std::endl;
        return false; // Expected 'do'
      }
    } else {
      std::cout << "expression() FAILED. line: " << token->line << std::endl;
      return false; // expression FAILED
    }
  }
  std::cout << "command() FAILED. line: " << token->line << std::endl;
  return false; // command FAILED
}

bool Sintatico::else_part() {
  if ((token + 2)->name == "else") {
    next();
    next();
    if (token->name == "else") {
      next();
      if (command()) {
        return true;
      } else {
        std::cout << "command() FAILED. line: " << token->line << std::endl;
        return false; // command FAILED
      }
    }
  }

  return true; // ε
}

bool Sintatico::procedure_activation() {
  if (token->type == Type::IDENTIFIER) {
    if (!checkIfExists({*token, Type::IDENTIFIER, scope})) {
      std::cout << "procedure does not exists. line: " << token->line
                << std::endl;
      return false; // procedure does not exists
    }
    if (peek().name == "(") {
      next();
      if (token->name == "(") {
        next();

        if (token->name == ")") {
          return true; // gambiarra
        }

        if (expression_list()) {
          next();
          if (token->name == ")") {
            return true;
          } else {
            std::cout << "Expected ')'. line: " << token->line << std::endl;
            return false; // Expected ')'
          }
        } else {
          std::cout << "expression_list() FAILED. line: " << token->line
                    << std::endl;
          return false; // expression_list FAILED
        }
      }
    }
    return true;
  }
  std::cout << "procedure_activation() FAILED. line: " << token->line
            << std::endl;
  return false; // procedure_activation FAILED
}

bool Sintatico::expression_list() {
  if (expression()) {
    return expression_list2();
  } else {
    std::cout << "expression() FAILED. line: " << token->line << std::endl;
    return false; // expression FAILED
  }
  std::cout << "expression_list() FAILED. line: " << token->line << std::endl;
  return false; // expression_list FAILED
}

bool Sintatico::expression_list2() {
  if (peek().name == ",") {
    next();
    if (token->name == ",") {
      next();
      if (expression()) {
        return expression_list2();
      } else {
        std::cout << "expression() FAILED. line: " << token->line << std::endl;
        return false; // expression FAILED
      }
    }
  }

  return true; // ε
}

bool Sintatico::expression() {
  if (simple_expression()) {
    if (peek().type == Type::RELACIONAL_OPERATOR) {
      next();
      if (token->type == Type::RELACIONAL_OPERATOR) {
        next();
        if (expression_list()) {
          return true;
        } else {
          std::cout << "expression_list() FAILED. line: " << token->line
                    << std::endl;
          return false; // expression_list FAILED
        }
      }
    }

    return true;
  } else {
    std::cout << "simple_expression() FAILED. line: " << token->line
              << std::endl;
    return false; // simple_expression FAILED
  }

  std::cout << "expression() FAILED. line: " << token->line << std::endl;
  return false; // expression FAILED
}

bool Sintatico::simple_expression() {
  if (term()) {
    return simple_expression2();
  } else {
    std::cout << "term() FAILED. line: " << token->line << std::endl;
    return false; // term FAILED
  }

  if (token->name == "+" || token->name == "-" || token->name == "or") {
    next();
    if (term()) {
      return simple_expression2();
    } else {
      std::cout << "term() FAILED. line: " << token->line << std::endl;
      return false; // term FAILED
    }
  }

  std::cout << "simple_expression() FAILED. line: " << token->line << std::endl;

  return false; // simple_expression FAILED
}

bool Sintatico::simple_expression2() {
  if (peek().name == "+" || peek().name == "-" || peek().name == "and") {
    next();
    if (token->name == "+" || token->name == "-" || peek().name == "and") {
      next();
      if (term()) {
        return simple_expression2();
      } else {
        std::cout << "term() FAILED. line: " << token->line << std::endl;
        return false; // term FAILED
      }
    }
  }

  return true; // ε
}

bool Sintatico::term() {
  if (factor()) {
    return term2();
  } else {
    std::cout << "factor() FAILED. line: " << token->line << std::endl;
    return false; // factor FAILED
  }
  std::cout << "term() FAILED. line: " << token->line << std::endl;
  return false; // term FAILED
}

bool Sintatico::term2() {

  if (peek().name == "*" || peek().name == "/" || peek().name == "and") {
    next();
    if (token->name == "*" || token->name == "/" || token->name == "and") {
      next();
      if (factor()) {
        return term2();
      } else {
        std::cout << "factor() FAILED. line: " << token->line << std::endl;
        return false; // factor FAILED
      }
    }
  }

  return true; // ε
}

bool Sintatico::factor() {
  if (token->type == Type::IDENTIFIER) {
    if (!checkIfExists({*token, Type::IDENTIFIER, scope})) {
      std::cout << "variable does not exists. line: " << token->line
                << std::endl;
      return false; // procedure does not exists
    }
    if (peek().name == "(") {
      next();
      if (token->name == "(") {
        next();
        if (expression_list()) {
          next();
          if (token->name == ")") {
            return true;
          }
        }
      }
    }
    return true;
  }

  if (token->type == Type::INTEGER) {
    return true;
  }

  if (token->type == Type::REAL) {
    return true;
  }

  if (token->type == Type::BOOLEAN) {
    return true;
  }

  if (token->name == "(") {
    next();
    if (expression()) {
      next();
      if (token->name == ")") {
        return true;
      }
    }
  }

  if (token->name == "not") {
    next();
    if (factor()) {
      return true;
    }
  }

  if (token->name == ")") {
    return true;
  }

  std::cout << "factor() FAILED. line: " << token->line << std::endl;
  return false;
}

Type stringToType(std::string s) {
  if (s == "REAL") {
    return Type::REAL;
  } else if (s == "INTEGER") {
    return Type::INTEGER;
  } else if (s == "BOOLEAN") {
    return Type::BOOLEAN;
  } else if (s == "RELACIONAL_OPERATOR") {
    return Type::RELACIONAL_OPERATOR;
  } else if (s == "LOGICAL_OPERATOR") {
    return Type::LOGICAL_OPERATOR;
  } else if (s == "OPERATOR") {
    return Type::OPERATOR;
  } else if (s == "KEYWORD") {
    return Type::KEYWORD;
  } else if (s == "IDENTIFIER") {
    return Type::IDENTIFIER;
  } else if (s == "DELIMITER") {
    return Type::DELIMITER;
  } else if (s == "ASSIGN") {
    return Type::ASSIGN;
  } else if (s == "EXPRESSION") {
    return Type::EXPRESSION;
  } else if (s == "PROGRAM") {
    return Type::PROGRAM;
  } else if (s == "PROCEDURE") {
    return Type::PROCEDURE;
  }
  throw std::exception();
}

std::string typeToString(Type t) {

  if (t == Type::REAL) {
    return "REAL";
  } else if (t == Type::INTEGER) {
    return "INTEGER";
  } else if (t == Type::BOOLEAN) {
    return "BOOLEAN";
  } else if (t == Type::RELACIONAL_OPERATOR) {
    return "RELACIONAL_OPERATOR";
  } else if (t == Type::LOGICAL_OPERATOR) {
    return "LOGICAL_OPERATOR";
  } else if (t == Type::OPERATOR) {
    return "OPERATOR";
  } else if (t == Type::KEYWORD) {
    return "KEYWORD";
  } else if (t == Type::IDENTIFIER) {
    return "IDENTIFIER";
  } else if (t == Type::DELIMITER) {
    return "DELIMITER";
  } else if (t == Type::ASSIGN) {
    return "ASSIGN";
  } else if (t == Type::EXPRESSION) {
    return "EXPRESSION";
  } else if (t == Type::PROGRAM) {
    return "PROGRAM";
  } else if (t == Type::PROCEDURE) {
    return "PROCEDURE";
  }
  throw std::exception();
}
