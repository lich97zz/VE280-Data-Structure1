#include <iostream>
#include "recursive.h"
#include "p2.h"
using namespace std;

int size(list_t list){
	if(list_isEmpty(list))
		return 0;
	return 1 + size(list_rest(list));
}

int sum(list_t list){
	if(list_isEmpty(list))
		return 0;
	return list_first(list) + sum(list_rest(list));
}

int product(list_t list){
	if(list_isEmpty(list))
		return 1;
	return list_first(list) * product(list_rest(list));
}

// static int add(int x, int y){
// 	return x + y;
// }

int accumulate(list_t list, int (*fn)(int, int), int base){
	if(list_isEmpty(list))
		return base;
	return fn(list_first(list), accumulate(list_rest(list), fn, base));
}

static int list_last(list_t list){
//EFFECTS: return the last element of the list
//REQUIRES: list is not empty
	if(size(list) == 1)
		return list_first(list);
	return list_last(list_rest(list));
}

list_t chop(list_t list, unsigned int n);

list_t reverse(list_t list){
	if(size(list) >= 1)
		return list_make(list_last(list), reverse(chop(list, 1)));
	return list_make();
}

list_t append(list_t first, list_t second){
	if(size(first) >= 1 && size(second) >= 1)
		return list_make(list_first(first), append(list_rest(first), second));
	else if(size(first) >= 1 && size(second) < 1)
		return first;
	else if(size(first) < 1 && size(second) >= 1)
		return second;
	return list_make();
}

list_t filter_odd(list_t list){
	if(size(list) >= 1){
		if(list_first(list) % 2 == 1)
			return list_make(list_first(list), filter_odd(list_rest(list)));
		else
			return filter_odd(list_rest(list));
	}
	return list_make();
}

list_t filter_even(list_t list){
	if(size(list) >= 1){
		if(list_first(list) % 2 == 1)
			return filter_even(list_rest(list));
		else
			return list_make(list_first(list), filter_even(list_rest(list)));
	}
	return list_make();
}

// bool odd(int a){
// 	return (a%2 == 1)? true : false;
// }

list_t filter(list_t list, bool (*fn)(int)){
	if(size(list) >= 1){
		if(fn(list_first(list)))
			return list_make(list_first(list), filter(list_rest(list), fn));
		else
			return filter(list_rest(list), fn);
	}
	return list_make();	
}

list_t insert_list(list_t first, list_t second, unsigned int n){
	if(n)
		return list_make(list_first(first), insert_list(list_rest(first), second, n-1));
	if(size(second))
		return list_make(list_first(second), insert_list(first, list_rest(second), n));
	else
		return first;
}

list_t chop(list_t list, unsigned int n){
	if((unsigned int)size(list) > n)
		return list_make(list_first(list), chop(list_rest(list), n));
	return list_make();
}

int tree_sum(tree_t tree){
	if(tree_isEmpty(tree))
		return 0;
	return tree_sum(tree_left(tree)) + tree_sum(tree_right(tree)) + tree_elt(tree);
}


bool tree_search(tree_t tree, int key){
	if(tree_isEmpty(tree))
		return false;
	bool b1 = (key == tree_elt(tree));
	bool b2 = tree_search(tree_left(tree), key);
	bool b3 = tree_search(tree_right(tree), key);
	return b1 || b2 || b3;
}

static int max(int a, int b){
	//EFFECTS: return the larger one between a and b
	return (a>b) ? a : b;
}

static int min_3(int a, int b, int c){
	//EFFECTS: return the smallest one between a, b, c
	int temp = (a<b) ? a : b;
	return (temp<c) ? temp : c;
}

static int min(int a, int b){
	//EFFECTS: return the smallest one between a, b
	return (a<b) ? a : b;
}

int depth(tree_t tree){
	if(tree_isEmpty(tree))
		return 0;
	return max(depth(tree_left(tree)), depth(tree_right(tree))) + 1;
}

int tree_min(tree_t tree){
	bool b1 = !tree_isEmpty(tree);
	bool b2 = !tree_isEmpty(tree_left(tree));
	bool b3 = !tree_isEmpty(tree_right(tree));
	if(b1 && b2 && b3)
		return min_3(tree_elt(tree), tree_min(tree_left(tree)), tree_min(tree_right(tree)));
	if(b1 && !b2 && b3)
		return min(tree_elt(tree), tree_min(tree_right(tree)));
	if(b1 && b2 && !b3)
		return min(tree_elt(tree), tree_min(tree_left(tree)));
	//if(b1 && !b2 && !b3)
	return tree_elt(tree);
}

list_t traversal(tree_t tree){
	if(tree_isEmpty(tree))
		return list_make();
	list_t t1 = traversal(tree_left(tree));
	list_t t2 = list_make(tree_elt(tree), traversal(tree_right(tree)));
	return insert_list(t1, t2, size(t1));
}

bool tree_hasPathSum(tree_t tree, int sum){
	if (tree_isEmpty(tree))
		return !bool(sum);
	return tree_hasPathSum(tree_left(tree), sum-tree_elt(tree)) || tree_hasPathSum(tree_right(tree), sum-tree_elt(tree));
}

bool covered_by(tree_t A, tree_t B){
	if(tree_isEmpty(A))
		return true;
	if(tree_isEmpty(B))
		return false;
	bool b1 = (tree_elt(A) == tree_elt(B));
	bool b2 = covered_by(tree_left(A), tree_left(B));
	bool b3 = covered_by(tree_right(A), tree_right(B));
	return (b1 && b2 && b3);
}

bool contained_by(tree_t A, tree_t B){
	if(!tree_isEmpty(A) && tree_isEmpty(B))
		return false;
	if(tree_isEmpty(A) && tree_isEmpty(B))
		return true;
	return covered_by(A, B) || contained_by(A, tree_left(B)) || contained_by(A, tree_right(B));
}

tree_t insert_tree(int elt, tree_t tree){
	if(tree_isEmpty(tree))
		return tree_make(elt, tree_make(), tree_make());
	if(elt < tree_elt(tree)){
		tree_t right = (tree_isEmpty(tree_right(tree))) ? tree_make() : tree_right(tree);
		tree_t left = (tree_isEmpty(tree_left(tree))) ? tree_make() : tree_left(tree);
		return tree_make(tree_elt(tree), insert_tree(elt, left), right);
	}
	//if(elt >= tree_elt(tree))
	tree_t right = (tree_isEmpty(tree_right(tree))) ? tree_make() : tree_right(tree);
	tree_t left = (tree_isEmpty(tree_left(tree))) ? tree_make() : tree_left(tree);
	return tree_make(tree_elt(tree), left, insert_tree(elt, right));
}
