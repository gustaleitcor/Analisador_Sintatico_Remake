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
  this->token++;
  std::cout << typeToString(this->token->type) << ' ' << this->token->name
            << std::endl;
}

bool Sintatico::analyse() {
  token = this->tokens.begin();
  return this->program();
};

bool Sintatico::program() {
  std::cout << "### program" << std::endl;
  if (token->name == "program") {
    next();
    if (token->type == Type::IDENTIFIER) {
      next();
      if (token->name == ";") {
        next();
        if (variable_declaration()) {
          next();
          if (subprograms_declarations()) {
            next();
            if (compost_command()) {
              next();
              if (token->name == ".") {
                return true;
              }
            }
          }
        }
      }
    }
  }

  return false;
}

bool Sintatico::variable_declaration() {
  std::cout << "### variable_declaration" << std::endl;
  if (token->name == "var") {
    next();
    if (variable_declaration_list()) {
      return true;
    }
  }

  return true;
}

bool Sintatico::variable_declaration_list() {
  std::cout << "### variable_declaration_list" << std::endl;
  if (identifiers_list()) {
    next();
    if (token->name == ":") {
      next();
      if (type()) {
        next();
        if (token->name == ";") {
          next();
          if (variable_declaration_list2()) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool Sintatico::variable_declaration_list2() {
  std::cout << "### variable_declaration_list2" << std::endl;
  if (identifiers_list()) {
    next();
    if (token->name == ":") {
      next();
      if (type()) {
        next();
        if (token->name == ";") {
          next();
          if (variable_declaration_list2()) {
            return true;
          }
        }
      }
    }
  }
  return true;
}

bool Sintatico::identifiers_list() {
  std::cout << "### identifiers_list" << std::endl;
  if (token->type == Type::IDENTIFIER) {
    next();
    if (identifiers_list2()) {
      return true;
    }
  }
  return false;
}

bool Sintatico::identifiers_list2() {
  std::cout << "### identifiers_list2" << std::endl;
  if (token->name == ",") {
    next();
    if (token->type == Type::IDENTIFIER) {
      next();
      if (identifiers_list2()) {
        return true;
      }
    }
  }
  return true;
}

bool Sintatico::type() {
  std::cout << "### type" << std::endl;
  if (token->type == token->type == Type::INTEGER ||
      token->type == Type::REAL || token->type == Type::BOOLEAN) {
    return true;
  }
  return false;
}

bool Sintatico::subprograms_declarations() {
  std::cout << "### subprograms_declarations" << std::endl;
  if (subprograms_declarations2()) {
    return true;
  }
  return false;
}

bool Sintatico::subprograms_declarations2() {
  std::cout << "### subprograms_declarations2" << std::endl;
  if (subprogram_declaration()) {
    next();
    if (token->name == ";") {
      next();
      if (subprograms_declarations2()) {
        return true;
      }
    }
  }
  return true;
}

bool Sintatico::subprogram_declaration() {
  std::cout << "### subprogram_declaration" << std::endl;
  if (token->name == "procedure") {
    next();
    if (token->type == Type::IDENTIFIER) {
      next();
      if (arguments()) {
        next();
        if (variable_declaration()) {
          next();
          if (subprograms_declarations()) {
            next();
            if (compost_command()) {
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

bool Sintatico::arguments() {
  std::cout << "### arguments" << std::endl;
  if (token->name == "(") {
    next();
    if (parameters_list()) {
      return true;
    }
  }
  return true;
}

bool Sintatico::parameters_list() {
  std::cout << "### parameters_list" << std::endl;
  if (identifiers_list()) {
    next();
    if (token->name == ":") {
      next();
      if (type()) {
        next();
        if (parameters_list2()) {
          return true;
        }
      }
    }
  }
  return false;
}

bool Sintatico::parameters_list2() {
  std::cout << "### parameters_list2" << std::endl;
  if (token->name == ";") {
    next();
    if (identifiers_list()) {
      next();
      if (token->name == ":") {
        next();
        if (type()) {
          next();
          if (parameters_list2()) {
            return true;
          }
        }
      }
    }
  }
  return true;
}

bool Sintatico::compost_command() {
  std::cout << "### compost_command" << std::endl;
  if (token->name == "begin") {
    next();
    if (optinals_command()) {
      next();
      if (token->name == "end") {
        return true;
      }
    }
  }
  return false;
}

bool Sintatico::optinals_command() {
  std::cout << "### optinals_command" << std::endl;
  if (command_list()) {
    return true;
  }
  return true;
}

bool Sintatico::command_list() {
  std::cout << "### command_list" << std::endl;
  if (command()) {
    next();
    if (command_list2()) {
      return true;
    }
  }
  return false;
}

bool Sintatico::command_list2() {
  std::cout << "### command_list2" << std::endl;
  if (token->name == ";") {
    next();
    if (command()) {
      next();
      if (command_list2()) {
        return true;
      }
    }
  }
  return true;
}

bool Sintatico::command() {
  std::cout << "### command" << std::endl;

  if (token->type == Type::IDENTIFIER) {
    next();
    if (token->name == ":=") {
      next();
      if (expression()) {
        return true;
      }
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
          next();
          if (else_part()) {
            return true;
          }
        }
      }
    }
  } else if (token->name == "while") {
    next();
    if (expression()) {
      next();
      if (token->name == "do") {
        next();
        if (command()) {
          return true;
        }
      }
    }
  }

  return false;
}

bool Sintatico::else_part() {
  std::cout << "### else_part" << std::endl;
  if (token->name == "else") {
    next();
    if (command()) {
      return true;
    }
  }
  return true;
}

bool Sintatico::procedure_activation() { // OLHAR DPS ACHO Q TA ERRADO
  std::cout << "### procedure_activation" << std::endl;
  if (token->type == Type::IDENTIFIER) {
    next();
    if (token->name == "(") {
      next();
      if (expression_list()) {
        next();
        if (token->name == ")") {
          return true;
        }
      }
    } else {
      return true;
    }
  }
  return false;
}

bool Sintatico::expression_list() {
  std::cout << "### expression_list" << std::endl;
  if (expression()) {
    next();
    if (expression_list2()) {
      return true;
    }
  }
  return false;
}

bool Sintatico::expression_list2() {
  std::cout << "### expression_list2" << std::endl;
  if (token->name == ";") {
    next();
    if (expression()) {
      next();
      if (expression_list2()) {
        return true;
      }
    }
  }
  return true;
}

bool Sintatico::expression() {
  std::cout << "### expression" << std::endl;
  if (simple_expression()) {
    next();
    if (token->type == Type::RELACIONAL_OPERATOR) {
      next();
      if (simple_expression()) {
        return true;
      }
    } else {
      return true;
    }
  }
  return false;
}

bool Sintatico::simple_expression() {
  std::cout << "### simple_expression" << std::endl;
  if (term()) {
    next();
    if (simple_expression2()) {
      return true;
    }
  } else if (token->name == "-" || token->name == "+") {
    next();
    if (term()) {
      next();
      if (simple_expression2()) {
        return true;
      }
    }
  }
  return false;
}

bool Sintatico::simple_expression2() {
  std::cout << "### simple_expression2" << std::endl;
  if (token->name == "-" || token->name == "+" || token->name == "or") {
    next();
    if (term()) {
      next();
      if (simple_expression2()) {
        return true;
      }
    }
  }
  return true;
}

bool Sintatico::term() {
  std::cout << "### term" << std::endl;
  if (factor()) {
    next();
    if (term2()) {
      return true;
    }
  }
  return false;
}

bool Sintatico::term2() {
  std::cout << "### term2" << std::endl;
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

bool Sintatico::factor() {
  std::cout << "### factor" << std::endl;
  if (token->type == Type::IDENTIFIER) {
    next();
    if (token->name == "(") {
      next();
      if (expression_list()) {
        next();
        if (token->name == ")") {
          return true;
        }
      }
    } else {
      return true;
    }
  } else if (token->type == Type::INTEGER) {
    return true;
  } else if (token->type == Type::REAL) {
    return true;
  } else if (token->type == Type::BOOLEAN) {
    return true;
  } else if (token->name == "(") {
    if (expression()) {
      if (token->name == ")") {
        return true;
      }
    }
  } else if (token->name == "not") {
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