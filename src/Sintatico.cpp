#include "Sintatico.h"
#include <iostream>

std::string typeToString(Type t);
Type stringToType(std::string s);

void Sintatico::saveToFile(std::string){};

void Sintatico::next() {
  if ((++this->token - tokens.begin()) >= tokens.size()) {
    throw std::exception();
  }
  std::cout << token->name << std::endl;
}

Token Sintatico::peek() {
  if ((this->token - tokens.begin() + 1) >= tokens.size()) {
    throw std::exception();
  }

  return *(token + 1);
}

// Coisas do semântico

bool Sintatico::checkInScope(Variable comp, size_t scope) {
  // std::cout << stack.size() << std::endl;

  for (auto var : stack) {
    if (var.scope == scope && comp.token.name == var.token.name) {
      std::cout << "variable " << comp.token.name
                << " alredy declared. line: " << comp.token.line << std::endl;
      return true;
    }
  }
  std::cout << comp.scope << ' ' << comp.token.name << std::endl;

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

  std::cout << "end fo analisis" << std::endl;

  for (auto var : stack) {
    std::cout << var.scope << ' ' << var.token.name << std::endl;
  }

  return result;
};

bool Sintatico::program() {
  if (token->name == "program") {
    next();
    if (token->type == Type::IDENTIFIER) {

      if (!checkInScope({*token, scope}, scope)) {
        stack.push_back({*token, scope});
      } else {
        return false; // Variable alredy declared
      }

      next();
      if (token->name == ";") {

        if (!variable_declaration()) {
          return false; // variable_declaration FAILED
        }

        if (!subprograms_declarations()) {
          return false; // subprograms_declarations FAILED
        }

        next();

        scope++;

        if (compost_command()) {
          next();
          if (token->name == ".") {
            std::cout << "fim" << std::endl;
            scope--;
            removeScope(scope);
            return true;
          } else {
            return false; // Expected '.'
          }
        } else {
          return false; // compost_command FAILED
        }
      } else {
        return false; // Expected ';'
      }
    } else {
      return false; // Expected IDENTIFIER
    }
  } else {
    return false; // Expected program
  }
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
          return false; // Expected ';'
        }
      } else {
        return false; // type FAILED
      }
    } else {
      return false; // Expected ';'
    }
  } else {
    return false; // identifiers_list FAILED
  }

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
            return false; // Expected ';'
          }
        } else {
          return false; // type FAILED
        }
      } else {
        return false; // Expected ':'
      }
    } else {
      return false; // identifiers_list FAILED
    }
  }

  return true; // ε
}

bool Sintatico::identifiers_list() {
  if (token->type == Type::IDENTIFIER) {

    if (!checkInScope({*token, scope}, scope)) {
      stack.push_back({*token, scope});
    } else {
      return false; // Variable alredy declared
    }

    return identifiers_list2();
  }

  return false;
}

bool Sintatico::identifiers_list2() {
  if (peek().name == ",") {
    next();
    if (token->name == ",") {
      next();
      if (token->type == Type::IDENTIFIER) {

        if (!checkInScope({*token, scope}, scope)) {
          stack.push_back({*token, scope});
        } else {
          return false; // Variable alredy declared
        }

        return identifiers_list2();
      } else {
        return false; // Expected IDENTIFIER
      }
    } else {
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
        return false; // Expected ';'
      }
    } else {
      return false; // subprogram_declaration FAILED
    }
  }

  return true; // ε
}

bool Sintatico::subprogram_declaration() {
  if (token->name == "procedure") {
    next();
    if (token->type == Type::IDENTIFIER) {

      if (!checkInScope({*token, scope}, scope)) {
        stack.push_back({*token, scope});
      } else {
        return false; // Variable alredy declared
      }

      scope++;

      arguments();

      next();

      if (token->name == ";") {

        if (!variable_declaration()) {
          return false; // variable_declaration FAILED
        }

        if (!subprograms_declarations()) {
          return false; // subprograms_declarations FAILED
        }

        next();

        if (compost_command()) {
          removeScope(scope);
          scope--;
          return true;
        } else {
          return false; // compost_command FAILED
        }
      } else {
        return false; // Esxpected ';'
      }
    }
  }

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
        return false; // type FAILED
      }
    } else {
      return false; // Expected ':'
    }
  } else {
    return false; // identifiers_list FAILED
  }

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
            return false; // type FAILED
          }
        }
      } else {
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
          return false; // Expected '}'
        }
      } else {
        return false; // parameters_list FAILED
      }
    }
  }

  return true; // ε
}

bool Sintatico::compost_command() {
  if (token->name == "begin") {

    if (!optinals_command()) {
      return false; // optinals_command FAILED
    }

    next();

    if (token->name == "end") {
      return true;
    } else {
      return false; // Expected 'END'
    }
  } else {
    return false; // Expected 'BEGIN'
  }

  return false; // arguments FAILED
}

bool Sintatico::optinals_command() {
  if (peek().type == Type::IDENTIFIER || peek().name == "begin" ||
      peek().name == "if" || peek().name == "while") {
    next();
    if (command_list()) {
      return true;
    } else {
      return false; // command_list FAILED
    }
  }

  return true; // ε
}

bool Sintatico::command_list() {
  if (command()) {
    return command_list2();
  } else {
    return false; // command FAILED
  }

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
    if (!checkIfExists({*token, scope})) {
      return false; // Variable does not exists
    }

    if (peek().name == ":=") {
      next();
      if (token->name == ":=") {
        next();
        if (expression()) {
          return true;
        } else {
          return false; // expression FAIlED
        }
      }
    }

    if (procedure_activation()) {
      return true;
    } else {
      return false; // procedure_activation FAILED
    }
  }

  if (token->name == "begin") {
    scope++;
    if (compost_command()) {
      scope--;
      return true;
    } else {
      return false; // compost_command FAILED
    }
  }

  if (token->name == "if") {
    next();
    if (expression()) {
      next();
      if (token->name == "then") {
        next();
        if (command()) {
          return else_part();
        } else {
          return false; // command FAILED
        }
      } else {
        return false; // Expected 'then'
      }
    } else {
      return false; // expression FAILED
    }
  }

  if (token->name == "while") {
    next();
    if (expression()) {
      next();
      if (token->name == "do") {
        next();
        if (command()) {
          return true;
        } else {
          return false; // command FAILED
        }
      } else {
        return false; // Expected 'do'
      }
    } else {
      return false; // expression FAILED
    }
  }

  return false; // command FAILED
}

bool Sintatico::else_part() {
  if (peek().name == ";") // gambiarra
    next();
  if (peek().name == "else") {
    next();
    if (token->name == "else") {
      next();
      if (command()) {
        return true;
      } else {
        return false; // command FAILED
      }
    }
  }

  return true; // ε
}

bool Sintatico::procedure_activation() {
  if (token->type == Type::IDENTIFIER) {
    if (!checkIfExists({*token, scope})) {
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
            return false; // Expected ')'
          }
        } else {
          return false; // expression_list FAILED
        }
      }
    }
    return true;
  }
  return false; // procedure_activation FAILED
}

bool Sintatico::expression_list() {
  if (expression()) {
    return expression_list2();
  } else {
    return false; // expression FAILED
  }
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
          return false; // expression_list FAILED
        }
      }
    }

    return true;
  } else {
    return false; // simple_expression FAILED
  }

  return false; // expression FAILED
}

bool Sintatico::simple_expression() {
  if (term()) {
    return simple_expression2();
  } else {
    return false; // term FAILED
  }

  if (token->name == "+" || token->name == "-" || token->name == "and") {
    next();
    if (term()) {
      return simple_expression2();
    } else {
      return false; // term FAILED
    }
  }

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
    return false; // factor FAILED
  }
  return false; // term FAILED
}

bool Sintatico::term2() {

  if (peek().name == "*" || peek().name == "/" || peek().name == "or") {
    next();
    if (token->name == "*" || token->name == "/" || token->name == "or") {
      next();
      if (factor()) {
        return term2();
      } else {
        return false; // factor FAILED
      }
    }
  }

  return true; // ε
}

bool Sintatico::factor() {
  if (token->type == Type::IDENTIFIER) {
    if (!checkIfExists({*token, scope})) {
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

  return false;
}

Type stringToType(std::string s) {
  if (s == "REAL") {
    return Type::REAL;
  } else if (s == "INTEGER") {
    return Type::INTEGER;
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
  }
  throw std::exception();
}

std::string typeToString(Type t) {

  if (t == Type::REAL) {
    return "REAL";
  } else if (t == Type::INTEGER) {
    return "INTEGER";
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
  }
  throw std::exception();
}