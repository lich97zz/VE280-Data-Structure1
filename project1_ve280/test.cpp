#include <iostream>
// #include <string>

using namespace std;

int main(){
	int a = 0;
	const int* const x = &a;
	int b = 1;
	*x = 1;
	return 0;
}