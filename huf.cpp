#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <queue>

// Disciplina: Projeto e Análise de Algoritmo
// Feito por: Jackson Lima - 21351221

using namespace std;

typedef struct TreeNode {
  int frequency;
  char symbol;
  struct TreeNode* zero;
  struct TreeNode* one;
} TreeNode;

typedef struct Table {
  string code;
  char symbol;
} Table;

TreeNode* NewNode(int frequency, char symbol) {
  TreeNode* node = new(TreeNode);    // "new" is like "malloc"
  node->frequency = frequency;
  node->symbol = symbol;
  node->one = NULL;
  node->zero = NULL;

  return(node);
}

TreeNode* NewNode(int frequency, TreeNode* zero, TreeNode* one) {
  TreeNode* node = new(TreeNode);    // "new" is like "malloc"
  node->frequency = frequency;
  node->one = one;
  node->zero = zero;

  return(node);
}

void printVector(vector<char> vertex) {
  for(int index=0; index<vertex.size(); index++) {
    cout << vertex[index] << " ";
  }
  cout << endl;
}

void printGraph(vector<TreeNode> graph) {
  for(int index=0; index<graph.size(); index++) {
    cout << graph[index].symbol << ": " << graph[index].frequency << endl;
  }
  cout << endl;
}

void printTree(TreeNode* node) {
  if (node == NULL) return;

  cout << node->frequency << endl;
  printTree(node->zero);
  printTree(node->one);
}

void printTable(vector<Table> table) {
  for(int index=0; index<table.size(); index++) {
    cout << table[index].symbol << ": " << table[index].code << endl;
  }
  cout << endl;
}

int getIndexBySymbol(char symbol, vector<TreeNode> graph) {
  for(int index=0; index<graph.size(); index++) {
    if (graph[index].symbol == symbol) {
      return index;
    }
  }

  return -1;
}

vector<TreeNode> getGraph(vector<char> data) {
  std::vector<TreeNode> graph;
  int indexFound;

  for(int index=0; index<data.size(); index++) {
    // Found
    indexFound = getIndexBySymbol(data[index], graph);
    if(indexFound != -1) {
      graph[indexFound].frequency += 1;
    } else {
      TreeNode* node = NewNode(1, data[index]);

      graph.push_back(*node);
    }
  }

  return graph;
}

int getLesserFrequency(vector<TreeNode> graph) {
  int lesserFrequency = graph[0].frequency;
  int symbolIndex = 0;

  for(int index = 1; index < graph.size(); index++) {
    if(graph[index].frequency < lesserFrequency) {
      graph[index].frequency  = lesserFrequency;
      symbolIndex = index;
    }
  }

  return symbolIndex;
}

TreeNode* getTree(vector<TreeNode> graph) {
  int index;

  while(graph.size() > 1) {
    index = getLesserFrequency(graph);
    TreeNode first = graph[index];
    graph.erase(graph.begin()+index);

    index = getLesserFrequency(graph);
    TreeNode second = graph[index];
    graph.erase(graph.begin()+index);

    TreeNode* newNode = NewNode(second.frequency + first.frequency, &second, &first);

    graph.push_back(*newNode);
  }

  return &graph[0];
}

vector<Table> getCodes(TreeNode* root, string code) {
  vector<Table> table, tableOne, tableZero;


  /*
  if(root->symbol != '\0') {
    Table column;
    column.code = code;
    column.symbol = root->symbol;
    //cout << root->symbol << endl;
    table.push_back(column);
  }*/

  //cout << root->frequency << endl;
  tableOne = getCodes(root->one, "1" + code);
  tableZero = getCodes(root->zero, "0" + code);
  /*
  //table.reserve( tableOne.size() + tableZero.size() ); // preallocate memory
  if(tableOne.size()) {
    table.insert( table.end(), tableOne.begin(), tableOne.end() );
  }
  if(tableZero.size()) {
    table.insert( table.end(), tableZero.begin(), tableZero.end() );
  }*/
  return table;
}

int main(int argc, char** argv) {
  std::ifstream infile(argv[1], std::ios::binary);

  std::vector<char> data = std::vector<char>(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());

  //printVector(data);
  //printGraph(getGraph(data));
  TreeNode* root = getTree(getGraph(data));
  printTree(root);
  //printTable(getCodes(&root, ""));
  infile.close();

  return 0;
}
