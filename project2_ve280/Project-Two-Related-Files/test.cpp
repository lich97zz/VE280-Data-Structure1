/*************************************************************************
    > File Name: test.cpp
    > Author: Charles Chang
    > Mail: charleschang213@sjtu.edu.cn 
    > Created Time: 2018-06-05 14:07:06
 ************************************************************************/

#include<iostream>
#include "p2.cpp"
#include "recursive.cpp"
#include "test.h"
#include <cstdlib>
using namespace std;

static bool list_equal(list_t l1, list_t l2)
    // EFFECTS: returns true iff l1 == l2.
{
    if(list_isEmpty(l1) && list_isEmpty(l2))
    {
        return true;
    }
    else if(list_isEmpty(l1) || list_isEmpty(l2))
    {
        return false;
    }
    else if(list_first(l1) != list_first(l2))
    {
        return false;
    }
    else
    {
        return list_equal(list_rest(l1), list_rest(l2));
    }    
}


static bool tree_equal(tree_t t1, tree_t t2)
    // EFFECTS: returns true iff t1 == t2
{
    if(tree_isEmpty(t1) && tree_isEmpty(t2))
    {
        return true;
    }
    else if(tree_isEmpty(t1) || tree_isEmpty(t2))
    {
        return false;
    }
    else
    {
        return ((tree_elt(t1) == tree_elt(t2)) 
            && tree_equal(tree_left(t1), tree_left(t2))
            && tree_equal(tree_right(t1), tree_right(t2)));
    }
}

static int max_int(int a,int b) {
	return a>b?a:b;
}

static bool threetimes(int a) {
	return a%3==0;
}

int main(){
	list_t answer = list_make();
	#ifdef LIST_TEST
	list_t A = list_make();
	list_t B = list_make();
	for(int i=1;i<=10;i++) A = list_make(i,A);
	for(int i=100;i>=91;i--) B = list_make(i,B);
	cout << "A=";
	list_print(A);
	cout << endl << "B=";
	list_print(B);
	cout << endl;
	cout << "Press Enter to start testing" << endl;
	////getchar();
	/* test size() */
	#ifdef SIZE
	cout  << "size() test section" << endl;
	cout << "size(A)=" << size(A) << endl;
	if (size(A)==10) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Filed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	/* test sum() */
	#ifdef SUM
	cout << "sum() test section" << endl;
	cout << "sum(A)=" << sum(A) << endl;
	if (sum(A)==55) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	/* test sum() */
	#ifdef PRODUCT
	cout << "product() test section" << endl;
	cout << "product(A)=" << product(A) << endl;
	if (product(A)==3628800) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	/* test accumulate() */
	#ifdef ACCUMULATE
	cout << "accumulate() test section" << endl;
	cout << "The function used is max function" << endl << "case base=-1" << endl;
	cout << "Your answer=" << accumulate(A,max_int,-1) << endl;
	if (accumulate(A,max_int,-1)==10) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	/* test reverse() */
	#ifdef REVERSE
	cout << "reverse() test section" << endl;
	cout << "reverse(A)=";
	list_print(reverse(A));
	cout << endl;
	answer = list_make();
	for (int i=10;i>=1;i--) answer = list_make(i,answer);
	if (list_equal(reverse(A),answer)) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	/* test reverse() */
	#ifdef APPEND	
	cout << "append() test section" << endl;
	cout << "append(A,B)=";
	list_print(append(A,B));
	cout << endl;
	answer = list_make();
	for (int i=100;i>=91;i--) answer = list_make(i,answer);
	for (int i=1;i<=10;i++) answer = list_make(i,answer);
	if (list_equal(append(A,B),answer)) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef FILTER_ODD
	cout << "filter_odd() test section" << endl;
	cout << "filter_odd(A)=";
	list_print(filter_odd(A));
	cout << endl;
	answer = list_make();
	for (int i=1;i<=10;i=i+2) answer = list_make(i,answer);
	if (list_equal(filter_odd(A),answer)) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef FILTER_EVEN
	cout << "filter_even() test section" << endl;
	cout << "filter_even(A)=";
	list_print(filter_even(A));
	cout << endl;
	answer = list_make();
	for (int i=2;i<=10;i=i+2) answer = list_make(i,answer);
	if (list_equal(filter_even(A),answer)) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef FILTER
	cout << "filter() test section" << endl << "The filter will let 3 multiples remain" << endl;
	cout << "Your answer=";
	list_print(filter(A,threetimes));
	cout << endl;
	answer = list_make();
	for (int i=3;i<=10;i=i+3) answer = list_make(i,answer);
	if (list_equal(filter(A,threetimes),answer)) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef INSERT_LIST
	cout << "insert_list() test section" << endl << "n=5" << endl;
	cout << "Your answer=";
	list_print(insert_list(A,B,5));
	cout << endl;
	answer = list_make();
	for (int i=1;i<=5;i++) answer = list_make(i,answer);
	for (int i=100;i>=91;i--) answer = list_make(i,answer);
	for (int i=6;i<=10;i++) answer = list_make(i,answer);
	if (list_equal(insert_list(A,B,5),answer)) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef CHOP
	cout << "chop() test section" << endl << "n=5" << endl;
	cout << "Your answer=";
	list_print(chop(A,5));
	cout << endl;
	answer = list_make();
	for (int i=6;i<=10;i++) answer = list_make(i,answer);
	if (list_equal(chop(A,5),answer)) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#endif
	#ifdef TREE_TEST
	cout << "Tree Section" << endl;
	cout << "X = " << endl;
	tree_t X = tree_make(3,tree_make(1,tree_make(),tree_make(2,tree_make(),tree_make())),tree_make(6,tree_make(4,tree_make(),tree_make()),tree_make(7,tree_make(),tree_make(8,tree_make(),tree_make()))));
	tree_print(X);
	cout << "Y = " << endl;
	tree_t Y = tree_make(6,tree_make(4,tree_make(),tree_make()),tree_make(7,tree_make(),tree_make()));
	tree_print(Y);
	cout << "Z = " << endl;
	tree_t Z = tree_make(3,tree_make(1,tree_make(),tree_make()),tree_make(6,tree_make(),tree_make()));
	tree_print(Z);
	cout << "Press Enter to start testing" << endl;
	//getchar();
	/* test tree_sum() */
	#ifdef TREE_SUM
	cout << "tree_sum() test section" << endl;
	cout << "tree_sum(X)=" << tree_sum(X) << endl;
	if (tree_sum(X)==31) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef TREE_SEARCH
	cout << "tree_search() test section" << endl;
	cout << "tree_search(X,7)=" << tree_search(X,7) << endl;
	if (tree_search(X,7)) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef DEPTH
	cout << "depth() test section" << endl;
	cout << "depth(X)=" << depth(X) << endl;
	if (depth(X)==4) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef TREE_MIN
	cout << "tree_min() test section" << endl;
	cout << "tree_min(X)=" << tree_min(X) << endl;
	if (tree_min(X)==1) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef TRAVERSAL
	cout << "traversal() test section" << endl;
	cout << "traversal(X)=";
	list_print(traversal(X));
	cout << endl;
	answer = list_make();
	for (int i=8;i>=1;i--) 
		if (i!=5) answer = list_make(i,answer);
	if (list_equal(answer,traversal(X))) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef COVERED_BY
	cout << "covered_by() test section" << endl;
	cout << "covered_by(Y,X)=" << covered_by(Y,X) << endl;
	cout << "covered_by(Z,X)=" << covered_by(Z,X) << endl;
	if ((covered_by(Z,X))&&(!covered_by(Y,X))) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef CONTAINED_BY
	cout << "contained_by() test section" << endl;
	cout << "contained_by(Y,X)=" << contained_by(Y,X) << endl;
	if (contained_by(Y,X)) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef TREE_HASPATHSUM
	cout << "tree_hasPathSum() test section" << endl;
	cout << "tree_hasPathSum(X,13)=" << tree_hasPathSum(X,13) << endl;
	cout << "tree_hasPathSum(X,100)=" << tree_hasPathSum(X,100) << endl;
	if ((tree_hasPathSum(X,13))&&(!tree_hasPathSum(X,100))) cout << "Success! Press Enter to the next section" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#ifdef INSERT_TREE
	cout << "insert_tree() test section" << endl;
	cout << "Insert 5 into the tree" << endl;
	tree_t answer_t = insert_tree(5,X);
	tree_t true_answer =  tree_make(3,tree_make(1,tree_make(),tree_make(2,tree_make(),tree_make())),tree_make(6,tree_make(4,tree_make(),tree_make(5,tree_make(),tree_make())),tree_make(7,tree_make(),tree_make(8,tree_make(),tree_make()))));
	cout << "Your answer=" << endl;
	tree_print(answer_t);
	if (tree_equal(answer_t,true_answer)) cout << "Success! Press Enter to continue" << endl;
	else {
		cout << "Failed!" << endl;
		return -1;
	}
	//getchar();
	#endif
	#endif
	cout << "Your program has passed all the test sections!" << endl;
	return 0;
}

