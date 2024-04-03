#include "Sintatico.h"
#include <iostream>

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
          break;
        }
      }
      if (line[j].type == Type::IDENTIFIER) {
        std::cout << "variable '" << line[j].name
                  << "' does not exists Line: " << line[j].line << std::endl;
        return false;
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
        // std::cout << typeToString(line[j + 1].type) << ' ' << j << std::endl;

        line.erase(line.begin() + j);
        line.erase(line.begin() + j);
        line.erase(line.begin() + j);
        line.insert(line.begin() + j, {result});
      }
    }

    for (int j = line.size() - 3; j >= 0; j--) {
      if (line[j + 1].type == RELACIONAL_OPERATOR) {
        if (line[j].type == BOOLEAN || line[j + 2].type == BOOLEAN) {
          return false;
        } else if (line[j].type == REAL || line[j + 2].type == REAL) {
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
  if (option == 1) {
    for (int j = line.size() - 3; j >= 0; j--) {

      // for (auto var : line) {
      //   std::cout << var.name << ' ';
      // }
      // std::cout << std::endl;

      // for (auto var : line) {
      //   std::cout << typeToString(var.type) << ' ';
      // }
      // std::cout << std::endl;

      if (line[j + 1].type == OPERATOR) {
        if (line[j].type == REAL || line[j + 2].type == REAL) {
          result = REAL;
        } else if (line[j].type == INTEGER && line[j + 2].type == INTEGER) {
          result = INTEGER;
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

    if (line.size() == 3 && line[1].type == ASSIGN) {
      if (line[0].type == BOOLEAN && line[2].type == BOOLEAN) {
        return true;
      } else if (line[0].type == REAL) {
        if (line[2].type == INTEGER || line[2].type == REAL)
          return true;
      } else if (line[0].type == INTEGER && line[2].type == INTEGER) {
        return true;
      }
      return false;
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
