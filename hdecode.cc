#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
using namespace std;

int f_size = 0;
string binCont = "";
int letter;
int counter = 0;

struct hnode {
   size_t count;
   unsigned char data;
   hnode * left;
   hnode * right;
   hnode(int letter, size_t count) : 
   	data(letter), count(count), left(NULL), right(NULL){}
};
// 8-bit string rep. of decimal input
string dec_bin(int n, string bits) {
	if(n == 0) {
		return bits;
	}
	bits = to_string(n % 2) + bits;
	return dec_bin(n / 2, bits);
}
hnode * rebuildTree(ifstream & input) {
	if(letter == 'L') {
		letter = input.get();
		return new hnode(letter, 0);
	}
	// left subtree
	letter = input.get();
	hnode * left = rebuildTree(input);
	letter = input.get();
	// right subtree
	hnode * right = rebuildTree(input);
	hnode * n = new hnode(5, 0);
	n -> left = left;
	n -> right = right;
	return n;
}
bool isLeaf(hnode * tree) {
	return tree -> left == NULL && tree -> right == NULL;
}
// traverse tree until code found
// abuse fact that no code is prefix of other code
char findCode(string bits, hnode * tree) {
	if(isLeaf(tree)) {
		return tree -> data;
	}
	// 48 is ascii for '0'
	// counter keeps track of length of code
	if(bits[0] == 48) {
		counter++;
		return findCode(bits.substr(1, bits.length() - 1), tree -> left);
	}
	else {
		counter++;
		return findCode(bits.substr(1, bits.length() - 1), tree -> right);
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
int main(int argc, char **argv) {

  string filename = argv[1];
  ifstream in(filename);
  if(!in) {
    cerr << "ERROR: Source file does not exist";
    return 0;
  }

  string fname = filename.substr(0, filename.length() - 4);
  ofstream output(fname);
  // getting file size
  while ((letter = in.get()) != 'I' && letter != 'L') {
  	f_size = f_size*10 + (letter - '0');
  }
  // rebuilding tree
  hnode * root;
  root = rebuildTree(in);
// make binary of chars
  string temp;
  while((letter = in.get()) != EOF) {
  	if(letter == 0) {
		temp = "00000000";
	}
	else {
		temp = dec_bin(letter, "");
		// pad binary to 8 bits
		while(temp.length() % 8 != 0) {
			temp = "0" + temp;
		}
	}
  	binCont += temp;
  }
  string bin_str = binCont;
  // traversing tree to get chars from codes
  for(int i = 0; i < f_size; i++) {
  	output << findCode(bin_str, root);
  	// removing used binary bits
  	bin_str = binCont.substr(counter, binCont.length() - counter);
  }
  lrpoot_delete(root);
  
  remove(filename.c_str());
}



