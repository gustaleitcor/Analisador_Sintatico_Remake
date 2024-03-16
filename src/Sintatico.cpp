#include "./Sintatico.h"
#include <exception>
#include <iostream>
#include <list>
#include <unistd.h>
#include <vector>

/*
int control = 0; // onde estamos

if(this->tokens[control] == "program"){
    control++;
    if(this->tokens[control] == "id"){
        control++;
        if(this->tokens[control] == ";"){

        }else{
            printf("ERRO");
        }
    }
}

*/

std::string typeToString(Type t);
Type stringToType(std::string s);

void Sintatico::saveToFile(std::string){};

void Sintatico::next() {
  if ((++this->token - tokens.begin()) >= tokens.size()) {
    throw std::exception();
  }
  // std::cout << typeToString(this->token->type) << ' ' << this->token->name
  //           //<< std::endl;
}

Token Sintatico::peek() {
  if ((this->token - tokens.begin() + 1) >= tokens.size()) {
    throw std::exception();
  }

  return *(token + 1);
}

bool Sintatico::analyse() {
  token = this->tokens.begin();
  return this->program();
};

bool Sintatico::program() {
  // std::cout << token->name << ' ' << "### program" //<< std::endl;
  if (token->name == "program") {
    next();
    if (token->type == Type::IDENTIFIER) {
      next();
      if (token->name == ";") {
        next();
        if (variable_declaration()) {
          if (subprograms_declarations()) {
            /// std::cout << token->name
            // << "--------" //<< std::endl;
            if (compost_command()) {
              next();
              if (token->name == ".") {
                return true;
              } else {
                // std::cout << "Expected '.' in line: " << token->line
                //<< std::endl;
              }
            }
          }
        }
      } else {
        // std::cout << "Expected ';' in line: " << token->line //<< std::endl;
      }
    } else {
      // std::cout << "Expected 'IDENTIFIER' in line: " << token->line
      //<< std::endl;
    }
  } else {
    // std::cout << "Expected 'program' in line: " << token->line //<<
    // std::endl;
  }

  return false;
}

bool Sintatico::variable_declaration() {
  /// //std::cout << token->name << ' ' << "### variable_declaration" <<
  /// std::endl;
  if (token->name == "var") {
    next();
    if (variable_declaration_list()) {
      return true;
    } else {
      return false;
    }
  } else {
    // std::cout << "Expected 'var' in line: " << token->line //<< std::endl;
  }

  return true;
}

bool Sintatico::variable_declaration_list() {
  ////std::cout << token->name << ' ' << "### variable_declaration_list"
  //        //<< std::endl;
  if (identifiers_list()) {
    if (token->name == ":") {
      next();
      if (type()) {
        next();
        if (token->name == ";") {
          next();
          if (variable_declaration_list2()) {
            return true;
          }
        } else {
          std::cout << "Expected ';' in line: " << (token - 1)->line
                    << std::endl;
        }
      }
    } else {
      std::cout << "Expected ':' in line: " << token->line << std::endl;
    }
  }
  return false;
}

bool Sintatico::variable_declaration_list2() {
  ////std::cout << token->name << ' ' << "### variable_declaration_list2"
  //        //<< std::endl;
  if (identifiers_list()) {
    if (token->name == ":") {
      next();
      if (type()) {
        next();
        if (token->name == ";") {
          next();
          if (variable_declaration_list2()) {
            return true;
          }
        } else {
          // std::cout << "Expected ';' in line: " << token->line //<<
          // std::endl;
        }
      }
    } else {
      // std::cout << "Expected ':' in line: " << token->line //<< std::endl;
    }
  }
  return true;
}

bool Sintatico::identifiers_list() {
  ////std::cout << token->name << ' ' << "### identifiers_list" //<< std::endl;
  if (token->type == Type::IDENTIFIER) {
    next();
    if (identifiers_list2()) {
      return true;
    }
  } else {
    // std::cout << "Expected 'IDENTIFIER' in line: " << token->line <<
    // std::endl;
  }
  return false;
}

bool Sintatico::identifiers_list2() {
  // std::cout << token->name << ' ' << "### identifiers_list2" //<< std::endl;
  if (token->name == ",") {
    next();
    if (token->type == Type::IDENTIFIER) {
      next();
      if (identifiers_list2()) {
        return true;
      }
    } else {
      // std::cout << "Expected 'IDENTIFIER' in line: " << token->line
      //<< std::endl;
    }
  }
  return true;
}

bool Sintatico::type() {
  // std::cout << token->name << ' ' << "### type" //<< std::endl;
  if (token->name == "integer" || token->name == "real" ||
      token->name == "true" || token->name == "false") {
    return true;
  } else {
    // std::cout << "Expected 'IDENTIFIER' or 'REAL' or 'BOOLEAN' in line: "
    //<< token->line //<< std::endl;
  }
  return false;
}

bool Sintatico::subprograms_declarations() {
  // //std::cout << token->name << ' ' << "### subprograms_declarations"
  ////<< std::endl;
  if (subprograms_declarations2()) {
    return true;
  }
  return false;
}

bool Sintatico::subprograms_declarations2() {
  // //std::cout << token->name << ' ' << "### subprograms_declarations2"
  ////<< std::endl;
  if (subprogram_declaration()) {
    // std::cout << token->name << "--------" //<< std::endl;
    next();
    if (token->name == ";") {
      next();
      // std::cout << token->name << "--------" //<< std::endl;
      if (subprograms_declarations2()) {
        return true;
      }
    } else {
      // std::cout << "Expected ';' in line: " << token->line //<< std::endl;
      return false;
    }
  }
  return true;
}

bool Sintatico::subprogram_declaration() {
  // //std::cout << token->name << ' ' << "### subprogram_declaration" <<
  // std::endl;
  if (token->name == "procedure") {
    next();
    if (token->type == Type::IDENTIFIER) {
      next();
      if (arguments()) {
        next();
        if (variable_declaration()) {
          if (subprograms_declarations()) {
            if (compost_command()) {
              return true;
            }
          }
        }
      }
    }
  } else {
    // std::cout << "Expected 'procedure' in line: " << token->line <<
    // std::endl;
  }
  return false;
}

bool Sintatico::arguments() {
  // //std::cout << token->name << ' ' << "### arguments" //<< std::endl;
  if (token->name == "(") {
    next();
    if (parameters_list()) {
      if (token->name == ")") {
        next();
        if (token->name == ";") {
          return true;
        } else {
          // std::cout << "Expected ';' in line: " << token->line //<<
          // std::endl;
        }
      } else {
        // std::cout << "Expected ')' in line: " << token->line //<< std::endl;
      }
    }
  } else {
    // std::cout << "Expected '(' in line: " << token->line //<< std::endl;
  }
  return true;
}

bool Sintatico::parameters_list() {
  // //std::cout << token->name << ' ' << "### parameters_list" //<< std::endl;
  if (identifiers_list()) {
    if (token->name == ":") {
      next();
      if (type()) {
        next();
        if (parameters_list2()) {
          return true;
        }
      }
    } else {
      // std::cout << "Expected ':" << token->line //<< std::endl;
    }
  }
  return false;
}

bool Sintatico::parameters_list2() {
  // //std::cout << token->name << ' ' << "### parameters_list2" //<< std::endl;
  if (token->name == ",") {
    next();
    if (identifiers_list()) {
      if (token->name == ":") {
        next();
        if (type()) {
          next();
          if (parameters_list2()) {
            return true;
          }
        }
      } else {
        // std::cout << "Expected ': " << token->line //<< std::endl;
      }
    }
  } else {
    // std::cout << "Expected ',' in line: " << token->line //<< std::endl;
  }
  return true;
}

bool Sintatico::compost_command() {
  // //std::cout << token->name << ' ' << "### compost_command" //<< std::endl;
  if (token->name == "begin") {
    next();
    if (optinals_command()) {
      if (token->name == "end") {
        return true;
      } else {
        // std::cout << "Expected 'end' in line: " << token->line //<<
        // std::endl;
      }
    }
  } else {
    // std::cout << "Expected 'begin' in line: " << token->line //<< std::endl;
  }
  return false;
}

bool Sintatico::optinals_command() {
  // //std::cout << token->name << ' ' << "### optinals_command" //<< std::endl;
  if (command_list()) {
    return true;
  }
  return true;
}

bool Sintatico::command_list() {
  // //std::cout << token->name << ' ' << "### command_list" //<< std::endl;
  if (command()) {
    next();
    if (command_list2()) {
      return true;
    }
  }
  return false;
}

bool Sintatico::command_list2() {
  // //std::cout << token->name << ' ' << "### command_list2" //<< std::endl;
  if (command()) {
    next();
    if (command_list2()) {
      return true;
    }
  }
  return false;
}

bool Sintatico::command() {
  // //std::cout << token->name << ' ' << "### command" //<< std::endl;

  if (token->type == Type::IDENTIFIER) {
    next();
    if (token->name == ":=") {
      next();
      if (expression()) {
        if (token->name == ";") { // não tinha
          return true;
        } else {
          std::cout << "Expected ';' in line: " << (token - 1)->line
                    << std::endl;
          return false;
        }
      }
    } else {
      // std::cout << "Expected ':=' in line: " << token->line //<< std::endl;
    }
  } else if (procedure_activation()) {
    return true;
  } else if (compost_command()) {
    return true;
  } else if (token->name == "if") {
    next();
    if (expression()) {
      next();
      if (token->name == "then") {
        next();
        if (command()) {
          if (else_part()) {
            return true;
          } else {
            // std::cout << "Expected 'else' in line: " << token->line
            //<< std::endl;
          }
        } else {
          // std::cout << "Expected 'command' in line: " << token->line
          //<< std::endl;
        }
      } else {
        // std::cout << "Expected 'then' in line: " << token->line //<<
        // std::endl;
      }
    } else {
      // std::cout << "Expected 'expression' in line: " << token->line
      //<< std::endl;
    }
  } else if (token->name == "while") {
    next();
    // std::cout << token->name << "--------" //<< std::endl;
    if (expression()) {
      // std::cout << token->name << "--------" //<< std::endl;
      next();
      if (token->name == "do") {
        next();
        if (command()) {
          return true;
        } else {
          // std::cout << "Expected 'command' in line: " << token->line
          ////<< std::endl;
        }
      } else {
        // std::cout << "Expected 'do' in line: " << token->line //<< std::endl;
      }
    } else {
      // std::cout << "Expected 'exprssion' in line: " << token->line <<
      // std::endl;
    }
  }

  return false;
}

bool Sintatico::else_part() {
  // //std::cout << token->name << ' ' << "### else_part" //<< std::endl;
  if (peek().name == "else") {
    next();
  }
  if (token->name == "else") {
    next();
    if (command()) {
      return true;
    }
  } else {
    // std::cout << "Expected 'else' in line: " << token->line //<< std::endl;
  }
  return true;
}

bool Sintatico::procedure_activation() { // OLHAR DPS ACHO Q TA ERRADO
  // //std::cout << token->name << ' ' << "### procedure_activation" <<
  // std::endl;
  if (token->type == Type::IDENTIFIER) {
    next();
    if (token->name == "(") {
      next();
      if (expression_list()) {
        next();
        if (token->name == ")") {
          return true;
        } else {
          // std::cout << "Expected ')' in line: " << token->line //<<
          // std::endl;
        }
      }
    } else {
      // std::cout << "Expected '(' in line: " << token->line //<< std::endl;
      return true;
    }
  }
  return false;
}

bool Sintatico::expression_list() {
  // //std::cout << token->name << ' ' << "### expression_list" //<< std::endl;
  if (expression()) {
    next();
    if (expression_list2()) {
      return true;
    }
  }
  return false;
}

bool Sintatico::expression_list2() {
  // //std::cout << token->name << ' ' << "### expression_list2" //<< std::endl;
  if (token->name == ",") {
    next();
    if (expression()) {
      next();
      if (expression_list2()) {
        return true;
      }
    }
  } else {
    // std::cout << "Expected ',' in line: " << token->line //<< std::endl;
  }
  return true;
}

bool Sintatico::expression() {
  // //std::cout << token->name << ' ' << "### expression" //<< std::endl;
  if (simple_expression()) {
    if (token->type == Type::RELACIONAL_OPERATOR) {
      next();
      // std::cout << token->name
      //<< "--------" //<< std::endl;
      if (simple_expression()) {
        if (token->name == ")") {
          return true;
        } else {
          // std::cout << "Expected ')' in line: " << token->line //<<
          // std::endl;
          return false;
        }
      }
    } else {
      return true;
    }
  }
  return false;
}

bool Sintatico::simple_expression() {
  // //std::cout << token->name << ' ' << "### simple_expression" //<<
  // std::endl;
  if (token->name == "-" || token->name == "+") {
    next();
    if (term()) {
      if (simple_expression2()) {
        return true;
      }
    }
  } else if (term()) {
    if (simple_expression2()) {
      return true;
    }
  }
  return false;
}

bool Sintatico::simple_expression2() {
  // //std::cout << token->name << ' ' << "### simple_expression2" //<<
  // std::endl;
  if (token->name == "-" || token->name == "+" || token->name == "or") {
    next();
    if (term()) {
      if (simple_expression2()) {
        return true;
      }
    }
  } else {
    // std::cout << "Expected 'Aditive operator' in line: " << token->line
    ////<< std::endl;
  }
  return true;
}

bool Sintatico::term() {
  // //std::cout << token->name << ' ' << "### term" //<< std::endl;
  if (factor()) {
    if (token->name == "*" || token->name == "/" || token->name == "and") {
      next();
      if (term()) {
        return true;
      }
    } else {
      // std::cout << "Expected 'Multiplicative operator' in line: " <<
      // token->line
      ////<< std::endl;
      return true;
    }
  }
  return false;
}
/*
bool Sintatico::term2() {
  //std::cout << token->name << ' ' << "### term2" //<< std::endl;
  if (token->name == "*" || token->name == "/" || token->name == "and") {
    next();
    if (factor()) {
      next();
      if (term2()) {
        return true;
      }
    }
  }
  return true;
}
*/
bool Sintatico::factor() {
  // //std::cout << token->name << ' ' << "### factor" //<< std::endl;
  if (token->type == Type::IDENTIFIER) {
    next();
    if (token->name == "(") {
      next();
      if (expression_list()) {
        next();
        if (token->name == ")") {
          return true;
        } else {
          // std::cout << "Expected ')' in line: " << token->line //<<
          // std::endl;
        }
      }
    } else {
      // std::cout << "Expected '(' in line: " << token->line //<< std::endl;
      return true;
    }
  } else if (token->type == Type::INTEGER) {
    next();
    return true;
  } else if (token->type == Type::REAL) {
    next();
    return true;
  } else if (token->type == Type::BOOLEAN) {
    next();
    return true;
  } else if (token->name == "(") {
    std::cout << "opa" << std::endl;
    next();
    if (expression()) {
      if (token->name == ")") {
        return true;
      }
    }
  } else if (token->name == "not") {
    next();
    if (factor()) {
      return true;
    }
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