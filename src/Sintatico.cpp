#include "./Sintatico.h"
#include <exception>
#include <iostream>
#include <list>
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

void Sintatico::saveToFile(std::string){};

bool Sintatico::analyse() {
  TOKEN_ITERATOR token = this->tokens.begin();
  return this->program(token);
};

bool Sintatico::program(TOKEN_ITERATOR &token) {
  if (token->name == "program") {
    token++;
    if (token->type == Type::IDENTIFIER) {
      token++;
      if (token->name == ";") {
        token++;
        if (variable_declaration(token)) {
          token++;
          if (subprograms_declarations(token)) {
            token++;
            if (compost_command(token)) {
              token++;
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

bool Sintatico::variable_declaration(TOKEN_ITERATOR &token) {
  if (token->name == "var") {
    token++;
    if (variable_declaration_list(token)) {
      return true;
    }
  }

  return true;
}

bool Sintatico::variable_declaration_list(TOKEN_ITERATOR &token) {
  if (identifiers_list(token)) {
    token++;
    if (token->name == ":") {
      token++;
      if (type(token)) {
        token++;
        if (token->name == ";") {
          token++;
          if (variable_declaration_list2(token)) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool Sintatico::variable_declaration_list2(TOKEN_ITERATOR &token) {
  if (identifiers_list(token)) {
    token++;
    if (token->name == ":") {
      token++;
      if (type(token)) {
        token++;
        if (token->name == ";") {
          token++;
          if (variable_declaration_list2(token)) {
            return true;
          }
        }
      }
    }
  }
  return true;
}

bool Sintatico::identifiers_list(TOKEN_ITERATOR &token) {
  if (token->type == Type::IDENTIFIER) {
    token++;
    if (identifiers_list2(token)) {
      return true;
    }
  }
  return false;
}

bool Sintatico::identifiers_list2(TOKEN_ITERATOR &token) {
  if (token->name == ";") {
    token++;
    if (token->type == Type::IDENTIFIER) {
      token++;
      if (identifiers_list2(token)) {
        return true;
      }
    }
  }
  return true;
}

bool Sintatico::type(TOKEN_ITERATOR &token) {
  if (token->type == token->type == Type::INTEGER ||
      token->type == Type::REAL || token->type == Type::BOOLEAN) {
    return true;
  }
  return false;
}

bool Sintatico::subprograms_declarations(TOKEN_ITERATOR &token) {
  if (subprograms_declarations2(token)) {
    return true;
  }
  return false;
}

bool Sintatico::subprograms_declarations2(TOKEN_ITERATOR &token) {
  if (subprogram_declaration(token)) {
    token++;
    if (token->name == ";") {
      token++;
      if (subprograms_declarations2(token)) {
        return true;
      }
    }
  }
  return true;
}

bool Sintatico::subprogram_declaration(TOKEN_ITERATOR &token) {
  if (token->name == "procedure") {
    token++;
    if (token->type == Type::IDENTIFIER) {
      token++;
      if (arguments(token)) {
        token++;
        if (variable_declaration(token)) {
          token++;
          if (subprograms_declarations(token)) {
            token++;
            if (compost_command(token)) {
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

bool Sintatico::arguments(TOKEN_ITERATOR &token) {
  if (token->name == "(") {
    token++;
    if (parameters_list(token)) {
      return true;
    }
  }
  return true;
}

bool Sintatico::parameters_list(TOKEN_ITERATOR &token) {
  if (identifiers_list(token)) {
    token++;
    if (token->name == ":") {
      token++;
      if (type(token)) {
        token++;
        if (parameters_list2(token)) {
          return true;
        }
      }
    }
  }
  return false;
}

bool Sintatico::parameters_list2(TOKEN_ITERATOR &token) {
  if (token->name == ";") {
    token++;
    if (identifiers_list(token)) {
      token++;
      if (token->name == ":") {
        token++;
        if (type(token)) {
          token++;
          if (parameters_list2(token)) {
            return true;
          }
        }
      }
    }
  }
  return true;
}

bool Sintatico::compost_command(TOKEN_ITERATOR &token) {
  if (token->name == "begin") {
    token++;
    if (optinals_command(token)) {
      token++;
      if (token->name == "end") {
        return true;
      }
    }
  }
  return false;
}

bool Sintatico::optinals_command(TOKEN_ITERATOR &token) {
  if (command_list(token)) {
    return true;
  }
  return true;
}

bool Sintatico::command_list(TOKEN_ITERATOR &token) {
  if (command(token)) {
    token++;
    if (command_list2(token)) {
      return true;
    }
  }
  return false;
}

bool Sintatico::command_list2(TOKEN_ITERATOR &token) {
  if (token->name == ";") {
    token++;
    if (command(token)) {
      token++;
      if (command_list2(token)) {
        return true;
      }
    }
  }
  return true;
}

bool Sintatico::command(TOKEN_ITERATOR &token) {
  if (token->type == Type::IDENTIFIER) {
    token++;
    if (token->name == ":=") {
      token++;
      if (expression(token)) {
        token++;
        if (procedure_activation(token)) {
          return true;
        } else if (compost_command(token)) {
          return true;
        } else if (token->name == "if") {
          token++;
          if (expression(token)) {
            token++;
            if (token->name == "then") {
              token++;
              if (command(token)) {
                token++;
                if (else_part(token)) {
                  return true;
                }
              }
            }
          }
        } else if (token->name == "while") {
          token++;
          if (expression(token)) {
            token++;
            if (token->name == "do") {
              token++;
              if (command(token)) {
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

bool Sintatico::else_part(TOKEN_ITERATOR &token) {
  if (token->name == "else") {
    token++;
    if (command(token)) {
      return true;
    }
  }
  return true;
}

bool Sintatico::procedure_activation(
    TOKEN_ITERATOR &token) { // OLHAR DPS ACHO Q TA ERRADO
  if (token->type == Type::IDENTIFIER) {
    token++;
    if (token->name == "(") {
      token++;
      if (expression_list(token)) {
        token++;
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

bool Sintatico::expression_list(TOKEN_ITERATOR &token) {
  if (expression(token)) {
    token++;
    if (expression_list2(token)) {
      return true;
    }
  }
  return false;
}

bool Sintatico::expression_list2(TOKEN_ITERATOR &token) {
  if (token->name == ";") {
    token++;
    if (expression(token)) {
      token++;
      if (expression_list2(token)) {
        return true;
      }
    }
  }
  return true;
}

bool Sintatico::expression(TOKEN_ITERATOR &token) {
  if (expression_list(token)) {
    token++;
    if (token->type == Type::RELACIONAL_OPERATOR) {
      token++;
      if (simple_expression(token)) {
        return true;
      }
    }
  }
  return false;
}

bool Sintatico::simple_expression(TOKEN_ITERATOR &token) {
  if (term(token)) {
    token++;
    if (simple_expression2(token)) {
      return true;
    }
  } else if (token->name == "-" || token->name == "+") {
    token++;
    if (term(token)) {
      token++;
      if (simple_expression2(token)) {
        return true;
      }
    }
  }
  return false;
}

bool Sintatico::simple_expression2(TOKEN_ITERATOR &token) {
  if (token->name == "-" || token->name == "+" || token->name == "or") {
    token++;
    if (term(token)) {
      token++;
      if (simple_expression2(token)) {
        return true;
      }
    }
  }
  return true;
}

bool Sintatico::term(TOKEN_ITERATOR &token) {
  if (factor(token)) {
    token++;
    if (term2(token)) {
      return true;
    }
  }
  return false;
}

bool Sintatico::term2(TOKEN_ITERATOR &token) {
  if (token->name == "*" || token->name == "/" || token->name == "and") {
    token++;
    if (factor(token)) {
      token++;
      if (term2(token)) {
        return true;
      }
    }
  }
  return true;
}

bool Sintatico::factor(TOKEN_ITERATOR &token) {
  if (token->type == Type::IDENTIFIER) {
    token++;
    if (token->name == "(") {
      token++;
      if (expression_list(token)) {
        token++;
        if (token->name == ")") {
          return true;
        }
      }
    } else {
      return true;
    }
  } else if (token->type == Type::IDENTIFIER) {
    return true;
  } else if (token->type == Type::REAL) {
    return true;
  } else if (token->type == Type::BOOLEAN) {
    return true;
  } else if (expression(token)) {
    return true;
  } else if (token->name == "not") {
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