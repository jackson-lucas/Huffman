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


class Node {
  public:
    int frequency;
    char symbol;
    vector<Node> adjacents;
    Node(int frequency, char symbol);
    Node(int frequency, vector<Node> adjacents);
    void print();
    void printOne();
    void plusOneFrequency();
};

Node::Node(int frequency, char symbol) {
  this->frequency = frequency;
  this->symbol = symbol;
}

Node::Node(int frequency, vector<Node> adjacents) {
  this->frequency = frequency;
  this->adjacents = adjacents;
}

void Node::print() {
  cout << " ( ";
  if(adjacents.size() > 0) {
    adjacents[0].print();
  }

  cout << symbol << ": " << frequency;

  if(adjacents.size() > 1) {
    adjacents[1].print();
  }
  cout << " ) ";
}

void Node::printOne() {
  cout << symbol << ": " << frequency << endl;
}

void Node::plusOneFrequency() {
  frequency++;
}

class Graph {
  public:
    vector<Node> nodes;
    Graph(vector<Node> nodes);
    void printNodes();
    int getLesserFrequency();
    Node getTree();
};

Graph::Graph(vector<Node> nodes) {
  this->nodes = nodes;
}

void Graph::printNodes() {
  for(int index=0; index<nodes.size(); index++) {
    cout << nodes[index].symbol << ": " << nodes[index].frequency << endl;
  }
  cout << endl;
}

int Graph::getLesserFrequency() {
  int lesserFrequency = nodes[0].frequency;
  int symbolIndex = 0;

  for(int index = 1; index < nodes.size(); index++) {
    if(nodes[index].frequency < lesserFrequency) {
      lesserFrequency = nodes[index].frequency;
      symbolIndex = index;
    }
  }

  return symbolIndex;
}

Node Graph::getTree() {
  int index;

  while(nodes.size() > 1) {
    vector<Node> adjacents;

    index = getLesserFrequency();
    adjacents.push_back(nodes[index]);
    nodes.erase(nodes.begin()+index);

    index = getLesserFrequency();
    adjacents.push_back(nodes[index]);
    nodes.erase(nodes.begin()+index);


    Node newNode(adjacents[0].frequency + adjacents[1].frequency, adjacents);
    //newNode.printOne();
    nodes.push_back(newNode);
  }

  return nodes[0];
}

class Table {
  public:
    string code;
    char symbol;
};

int getIndexBySymbol(char symbol, vector<Node> graph) {
  for(int index=0; index<graph.size(); index++) {
    if (graph[index].symbol == symbol) {
      return index;
    }
  }

  return -1;
}

Graph getGraph(vector<char> data) {
  std::vector<Node> nodes;
  int indexFound;

  for(int index=0; index<data.size(); index++) {
    // Found
    indexFound = getIndexBySymbol(data[index], nodes);
    if(indexFound != -1) {
      //nodes[indexFound].printOne();
      nodes[indexFound].plusOneFrequency();
    } else {
      Node node(1, data[index]);
      //node.printOne();
      nodes.push_back(node);
    }
  }

  Graph graph(nodes);
  return graph;
}

void printVector(vector<char> vertex) {
  for(int index=0; index<vertex.size(); index++) {
    cout << vertex[index] << " ";
  }
  cout << endl;
}

int main(int argc, char** argv) {
  std::ifstream infile(argv[1], std::ios::binary);

  std::vector<char> data = std::vector<char>
    (std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());

  printVector(data);

  Graph graph = getGraph(data);
  //graph.printNodes();
  Node root = graph.getTree();
  root.print();

  infile.close();

  return 0;
}
