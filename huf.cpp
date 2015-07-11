#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>

// Disciplina: Projeto e Análise de Algoritmo
// Feito por: Jackson Lima - 21351221

using namespace std;

void printVector(vector<char> vertex) {
  for(int index=0; index<vertex.size(); index++) {
    cout << vertex[index] << " ";
  }
  cout << endl;
}

int main(int argc, char** argv) {
  std::ifstream infile(argv[1], std::ios::binary);

  std::vector<char> data = std::vector<char>(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());

  printVector(data);
  infile.close();

  return 0;
}
