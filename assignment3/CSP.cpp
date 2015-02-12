#include <iostream>
#include <vector>
#include <string> 

using namespace std;

class Domain
{
public:
	vector<int> board[9][9];
	int set[9][9];
	Domain(string s);
	int propagation(int x, int y);
	int removeV(int x, int y, int n);
	int applyR1();
	int applyR2();
	int printBoard();
};

// 240 300 000 
// 000 520 407
// 000 046 008
// 610 700 084
// 009 060 500 
// 730 005 061 
// 100 470 000 
// 302 051 000 
// 000 002 019

// 240300000000520407000046008610700084009060500730005061100470000302051000000002019

// 120700000340000700567000000000000000000000000000000000000000000000000000000000000

// 123000000456000000700000000070000000000000000000000000007000000000000000000000000

Domain::Domain(string s){
	for (int x = 0; x < 9; ++x){
		for (int y = 0; y < 9; ++y){
			char c = s[x*9+y];
			if(c == '0'){
				for(int i = 1; i <= 9; ++i){
					board[x][y].push_back(i);
				}
				set[x][y] = 0;
			} else {
				int n = c-'0';
				board[x][y].push_back(n);
				set[x][y] = 0;
			}
		}
	}
	// for (int x = 0; x < 9; ++x){
	// 	for (int y = 0; y < 9; ++y){
	// 		if(set[x][y] != 0){
	// 			// cout << x << ", " << y << endl;
	// 			if (propagation(x,y) == -1 ){
	// 				cout << "fail" << endl;
	// 				return -1;
	// 			}
	// 			board[x][y].push_back(set[x][y]);
	// 		}
	// 	}
	// }
}

int Domain::propagation(int x, int y){
	int n = set[x][y];
	for (int i = 0; i < 9; ++i){
		if(removeV(x,i,n) == 0 && i != y){
			cout << "1 " <<  x << y << " " << x << i << endl;
			return -1;
		}
		if(removeV(i,y,n) == 0 && i != x){
			cout << "2 " << x << y << " " << i << y << endl;
			return -1;
		}
	}
	int a = x/3;
	int b = y/3;
	a *= 3;
	b *= 3;
	for (int i = 0; i < 3; ++i){
		for (int j = 0; j < 3; ++j){
			if(removeV(a+i,b+j,n) == 0 && a+i != x && b+j != y ){
				cout << "3 " << x << y << " " << a+i << b+j << endl;
				return -1;
			}
		}
	}
	return 0;
}

int Domain::removeV(int x, int y, int n){
	// if(board[x][y].size() == 0){
	// 	return 0;
	// }
	// cout << "testing " << x << ", " << y << endl;
	for (vector<int>::iterator i = board[x][y].begin(); i != board[x][y].end(); ++i){
		if (*i == n ){
			// cout << "removing "<< *i << " at " << x << ", " << y << endl;
			i = board[x][y].erase(i);
			// cout << "remove end" << endl;
			if(i == board[x][y].end()) {
				return board[x][y].size();
			}
		}
	}
	return board[x][y].size();
}

int Domain::applyR1(){
	for (int x = 0; x < 9; ++x){
		for (int y = 0; y < 9; ++y){
			if(set[x][y] == 0 && board[x][y].size() == 1){
				set[x][y] = board[x][y][0];
				if (propagation(x,y) == -1 ){
					cout << "backtrack" << endl;
					return -1;
				}
				board[x][y].push_back(set[x][y]);
			}
		}
	}
}
// vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
int Domain::applyR2(){
	for (int i = 0; i < 9; ++i){
		vector<int> row;
		vector<int> col;
		for (int j = 0; j < 9; ++j){
			if(set[i][j] == 0){
				row.insert(row.end(),board[i][j].begin(),board[i][j].end());
			}
			if(set[j][i] == 0){
				col.insert(col.end(),board[j][i].begin(),board[j][i].end());
			}
		}
		int count[10] = {0, 0,0,0,0,0, 0,0,0,0};
		for (vector<int>::iterator n = row.begin(); n != row.end(); ++n){
			count[*n]++;
		}
		for (int k = 1; k <= 9; ++k){
			if(count[k] == 1) {
				int check = -1;
				for (int j = 0; j < 9 && check == -1 ; ++j){
					if(set[i][j] == 0){
						for (vector<int>::iterator n = board[i][j].begin(); n != board[i][j].end() && check == -1; ++n){
							if(*n == k){
								check = j;
							}
						}
					}
				}
				if(check != -1){
					board[i][check].clear();
					board[i][check].push_back(k);
					set[i][check] = k;
					if (propagation(i,check) == -1 ){
						cout << "backtrack" << endl;
						return -1;
					}
				}
			}
		}

		int count2[10] = {0, 0,0,0,0,0, 0,0,0,0};
		for (vector<int>::iterator n = col.begin(); n != col.end(); ++n){
			count2[*n]++;
		}
		for (int k = 1; k <= 9; ++k){
			if(count2[k] == 1) {
				int check = -1;
				for (int j = 0; j < 9 && check == -1 ; ++j){
					if(set[j][i] == 0){
						for (vector<int>::iterator n = board[j][i].begin(); n != board[j][i].end() && check == -1; ++n){
							if(*n == k){
								check = j;
							}
						}
					}
				}
				if(check != -1){
					board[check][i].clear();
					board[check][i].push_back(k);
					set[check][i] = k;
					if (propagation(check,i) == -1 ){
						cout << "backtrack" << endl;
						return -1;
					}
				}
			}
		}
	}

}

int Domain::printBoard(){
	for (int i = 0; i < 9; ++i)
	{
		if(i%3 == 0){
			cout << endl;
		}
		for (int j = 0; j < 9; ++j)
		{
			if(j%3 == 0){
				cout << " ";
			}
			cout << set[i][j];
		}
		cout << endl;
	}
}

// int checkSubDomain()

int main(int argc, char const *argv[]) {
	// Domain d ("240300000000520407000046008610700084009060500730005061100470000302051000000002019");
	// Domain d ("120700000340000700560000000000000000000000000000000000000000000000000000000000000");
	Domain d ("123000000456000000000000000070000000000000000000000000007000000000000000000000000");


	d.applyR1();

	// cout << d.board[0][0].size() << d.board[0][2][5] << endl;
	d.printBoard();

	cout << endl <<endl<<endl;

	d.applyR2();

	d.printBoard();


}

int backtrack(Domain d){
	if(d.applyR1() == -1){
		return -1;
	}
	if(d.applyR2() == -1){
		return -1;
	}
	
}