#ifndef STRUCTURE_CPP
#define STRUCTURE_CPP

#include <string>
#include <vector>
using namespace std;

template <typename T>
struct triplet {
    T first;
    T second;
    T third;
    vector<string> fourth;
    T fifth;
};

typedef struct treeNode {
  treeNode* left;
  treeNode* right;
  pair<string, vector<string>> currentRule;
  string currentValue;
} treeNode;

#endif