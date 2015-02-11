#include <iostream>
#include <vector>

using namespace std;

struct A
{
	vector<int> a[2][2];
};

int test(struct A a){
	a.a[0][0][0] += 1;
	cout << a.a[0][0][0] << a.a[0][0][1] << endl;
}


int main(int argc, char const *argv[]) {
	A a;
	a.a[0][0].push_back(1);
	a.a[0][0].push_back(2);
	test(a);
	cout << a.a[0][0][0] << a.a[0][0][1] << endl;
	return 0;
}