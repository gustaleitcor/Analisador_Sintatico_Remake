#include "./Sintatico.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {

  std::string token, type, line_str;
  unsigned long line_ul;
  std::ifstream input_file("analysed/Test1.txt");
  Sintatico sintatico;

  while (input_file >> type) {
    input_file >> token;
    input_file >> line_str;
    line_ul = std::stoul(line_str);

    sintatico.tokens.push_back(
        (Token){.type = stringToType(type), .name = token, .line = line_ul});
  }

  if (sintatico.analyse()) {
    std::cout << "Programa correto" << std::endl;
  } else {
    std::cout << "Programa incorreto" << std::endl;
  }

  // sintatico.saveToFile("out.txt");

  // for (auto token : sintatico.tokens) {
  //   std::cout << token.type << ' ' << token.name << std::endl;
  // }

  input_file.close();

  return 0;
}
