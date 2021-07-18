#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
using namespace std;

// heap node  
struct hnode {
   size_t count;
   unsigned char data;
   hnode * left;
   hnode * right;
   hnode(int letter, size_t count) : 
    data(letter), count(count), left(NULL), right(NULL){}
};
// global variables
size_t heap_size;
size_t file_size;

size_t COUNTS[256] = {0};
string pre_order_traversal = "";
string CODES[256];
hnode * heap[256];

// if left add 0, else add 1
// prevents any codes from being prefixes of other codes
void makeCodes(hnode * tree, string pc = "") {
  if(!(tree -> left) && !(tree -> right)) {
    CODES[tree -> data] = pc;
    return;
  }
  makeCodes(tree -> left, pc + "0");
  makeCodes(tree -> right, pc + "1");
}
// pre-order traversal
void pre_tree(hnode * tree) {
  if(!(tree -> left) && !(tree -> right)) {
    pre_order_traversal += "L";
    pre_order_traversal += (char)(tree -> data);
  }
  else {
    pre_order_traversal += "I";
    pre_tree(tree -> left);
    pre_tree(tree -> right);
  }
}
int strbin(string s) {
  int num = 0;
  for(int i = 0; i < s.length(); i++) {
    num = num * 2 + (s[i] - '0');
  }
  return num;
}
// padding & writing
void writeChars(string bits, ofstream & output) {
  while(bits.length() % 8 != 0) {
    bits += '0';
  }
  int j = 0;
  while(j < bits.length()) {
    output.put(strbin(bits.substr(j, 8)));
    j += 8;
  }
}
// left to right post-order traversal deletion
void lrpoot_delete(hnode * tree) {
  if(!tree) {
    return;
  }
  lrpoot_delete(tree -> left);
  lrpoot_delete(tree -> right);
  delete tree;
}
// turns "heap" array into minHeap
void heapify(int index) {
  int parent = (index - 1) / 2;
  // swap here
  if(index > 0 && heap[index] -> count < heap[parent] -> count) {
    hnode * temp = heap[index];
    heap[index] = heap[parent];
    heap[parent] = temp;
    heapify(parent);
  }
}
void heap_insert(int index, hnode * node) {
  heap[index] = node;
  heapify(index);
  heap_size += 1;
}
void heap_shift(int index) {
  // break when value leq to at least 1 child
  int lindex = 2 * index + 1;
  int rindex = lindex + 1;
  int pval = heap[index] -> count;
  // break when no children
  int lval = lindex >= heap_size ? pval + 1 : heap[lindex] -> count;
  int rval = rindex >= heap_size ? pval + 1 : heap[rindex] -> count;

  if(pval > lval || pval > rval) {
    int dex = lval > rval ? rindex : lindex;
    hnode * temp = heap[index];
    heap[index] = heap[dex];
    heap[dex] = temp;
    heap_shift(dex);
  }
}
// popping off root
hnode * heap_pop() {
  hnode * temp = heap[0];
  heap[0] = heap[heap_size - 1];
  heap_size -= 1;
  heap_shift(0);
  return temp;
}
// make tree from min heap
void build_tree() {
  if(heap_size > 1) {
    hnode * l = heap_pop();
    hnode * r = heap_pop();
    hnode * root = new hnode(50, l -> count + r -> count);
    root -> left = l;
    root -> right = r;
    heap_insert(heap_size, root);
    build_tree();
  }
}
int main(int argc, char **argv)
{
  string filename = argv[1];
  ifstream in(filename);
  if(!in) {
    cerr << "ERROR: Source file does not exist";
    return 0;
  }

  int letter;
  while ((letter = in.get()) != EOF) {
  	COUNTS[letter]++;
    file_size++;
  }

  for(int i = 0; i < 256; i++) {
    if(COUNTS[i] > 0) {
      hnode * h = new hnode(i, COUNTS[i]);
      heap_insert(heap_size, h);
    }
  }
  // heap[0] is root
  build_tree();
  makeCodes(heap[0]);

  string fname = filename;
  fname += ".huf";
  ofstream output(fname);
  pre_tree(heap[0]);
  
  output << file_size;
  output << pre_order_traversal;

  string codestr;
  ifstream in2(filename);
  // re-reading file to write out bits
  while ((letter = in2.get()) != EOF) {
    codestr += CODES[letter];
  }
  writeChars(codestr, output);

  lrpoot_delete(heap[0]);
  
  remove(filename.c_str());
}


