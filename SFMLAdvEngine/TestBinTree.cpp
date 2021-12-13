#include "TestBinTree.h"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <string>
#include <sstream>

#include "BoidAgentData.h"
#include "BinTree.h"

#include "add.h"
#include "mul.h"
#include "div.h"


using namespace std;

int random(int a){
	int i = rand() % a; /* A random integer between 1 and 10 */;
	return i;
}

int randomp(int a){
	int i = rand() % a + 1; /* A random integer between 1 and 10 */;
	return i;//(float)i;
}

void set_xvals(int values){
	int* xvals = new int[values];
	for (int i = 0; i < values; i++) {
		xvals[i] = i;
	}
}

void set_yvals(int values){
	int* yvals = new int[values];
	for (int i = 0; i < values; i++) {
		yvals[i] = i;
	}
}

void TestBinTree::testBinTree(){
	/*int* xvals = new int[0];
	int* yvals = new int[0];
	set_xvals(100);
	set_yvals(100);*/

	srand(time(NULL)); //initialize random seed

	BoidAgentData test;

	//array of operators available for the tree
	int operators[3], op;
	operators[0] = -1; //=+
	operators[1] = -2; //=*
	operators[2] = -3; //=/

	//strcpy(op, operators[random(3)]);
	op = operators[random(3)];
	//cout << op<<endl;

	BinTree<char> btree(op);

	op = operators[random(3)];
	btree.insert(op);

	int x = randomp(100);
	test.position.x = x;
	static_cast<char>(x);
	btree.insert(x);

	int y = randomp(100);
	test.position.y = y;
	//static_cast<char>(y);
	btree.insert(y);

	int ac = randomp(100);
	//test.acceleration = {100,100};
	//static_cast<char>(ac);
	btree.insert(ac);

	//test.acceleration = { randomp(100), randomp(100) };


	/*string st;
	cout << "PreOrder: ";
	st<<btree.preOrder();
	cout << endl;
	cout << "InOrder: ";
	btree.inOrder();
	cout << endl;
	cout << "PostOrder: ";
	btree.postOrder();*/

	
	cout << "PreOrder: ";
	btree.preOrder();
	cout << endl;
	cout << "InOrder: ";
	btree.inOrder();
	cout << endl;
	cout << "PostOrder: ";
	btree.postOrder();
	

	/*delete xvals[];
	delete yvals[];*/
};

