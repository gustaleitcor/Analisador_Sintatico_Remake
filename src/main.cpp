#include "./Sintatico.h"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {

  std::string token, type, line_str;
  unsigned long line_ul;
  Sintatico sintatico;

  if (argc < 2) {
    std::cout << "format: ./analyse <arquivo de teste>" << std::endl;
    return -1;
  }

  int result = system(
      (std::string("./Analisador_Lexico_Regex/analyse") + " ./" + argv[1])
          .c_str());

  if (result == -1) {
    std::cerr << "ERROR: lexic FAILED" << std::endl;
    std::cout << "Lexico falhou" << std::endl;
  }

  std::ifstream input_file("./a.txt");

  if (!input_file) {
    std::cerr << "ERROR: Could not open file" << std::endl;
    std::cout << "Não foi possivel abrir o arquivo gerado pelo lexico "
              << argv[1] << "\nTente novamente" << std::endl;
    return -1;
  }

  while (input_file >> type) {
    input_file >> token;
    input_file >> line_str;
    line_ul = std::stoul(line_str);

    sintatico.tokens.push_back(
        (Token){.type = stringToType(type), .name = token, .line = line_ul});
  }

  input_file.close();

  try {
    if (sintatico.analyse()) {
      std::cout << "Programa correto" << std::endl;
    } else {
      if (sintatico.token == sintatico.tokens.end()) {
        std::cout << "Programa correto" << std::endl;
      }
      std::cout << "Programa incorreto" << std::endl;
    }
  } catch (...) {
    std::cout << "Programa incorreto by crash" << std::endl;
  }

  // sintatico.saveToFile("out.txt");

  // for (auto token : sintatico.tokens) {
  //   std::cout << token.type << ' ' << token.name << std::endl;
  // }

  return 0;
}
