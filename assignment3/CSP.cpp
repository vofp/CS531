#include <iostream>
#include <vector>
#include <string> 
#include <algorithm>
#include <fstream>
using namespace std;

class Domain
{
public:
	vector<int> board[9][9];
	int set[9][9];
	int searchX;
	int searchY;
	int values[9];
	int counter;
	int spotsSearched;
	bool solved;
	int num_backtrack;
	bool rule2;
	bool rule3; 
	bool mostConstrained;
	int numsGiven;
	Domain(string s);
	int propagation(int x, int y);
	int removeV(int x, int y, int n);
	int applyR1();
	int applyR2();
	int applyR3();
	int applyR3R();
	int applyR3C();
	int applyR3B();
	int printBoard();
	int setValue(int x, int y, int n);
	int findMostConstrained();
	int nextCell();
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

// 120000700304700000056000000000000000000000000000000000000000000000000000000000000 

Domain::Domain(string s){
	numsGiven = 0;
	for (int x = 0; x < 9; ++x){
		for (int y = 0; y < 9; ++y){
			char c = s[x*9+y];
			if(c == '0'){
				for(int i = 1; i <= 9; ++i){
					board[x][y].push_back(i);
				}
				numsGiven++;
				set[x][y] = 0;
			} else {
				int n = c-'0';
				board[x][y].push_back(n);
				set[x][y] = 0;
			}
		}
		values[x] = 0;
	}
	counter = -1;
	spotsSearched = 0;
	num_backtrack = 0;
	solved = false;
	rule2 = true;
	rule3 = true;
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
			// cout << "1 " <<  x << y << " " << x << i << endl;
			return -1;
		}
		if(removeV(i,y,n) == 0 && i != x){
			// cout << "2 " << x << y << " " << i << y << endl;
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
				// cout << "3 " << x << y << " " << a+i << b+j << endl;
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

int Domain::setValue(int x, int y, int n){
	// cout << x << ", " << y  << "   "<< n << endl;
	set[x][y] = n;
	if (propagation(x,y) == -1 ){
		// cout << "backtrack" << endl;
		return -1;
	}
	board[x][y].clear();
	board[x][y].push_back(set[x][y]);
	// cout << "set " << set[x][y]<< endl;
	spotsSearched++;
}

int Domain::applyR1(){
	int count = 0;
	for (int x = 0; x < 9; ++x){
		for (int y = 0; y < 9; ++y){
			if(set[x][y] == 0 && board[x][y].size() == 1){
				// cout << x << ", " << y << endl;
				if(setValue(x,y,board[x][y][0]) == -1){
					return -1;
				}
				count++;
			}
		}
	}
	return count;
}
// vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
int Domain::applyR2(){
	int counter = 0;
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
					if (setValue(i,check,k) == -1 ){
						return -1;
					}
					counter++;
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
					if (setValue(check,i,k) == -1 ){
						return -1;
					}
					counter++;
				}
			}
		}
	}
	// 00 01 02
	// 10 11 12
	// 20 21 22
	for (int a = 0; a < 3; ++a){
		for (int b = 0; b < 3; ++b){
			vector<int> box;
			int x = a*3;
			int y = b*3;
			for (int i = 0; i < 3; ++i){
				for (int j = 0; j < 3; ++j){
					box.insert(box.end(),board[x+i][y+j].begin(),board[x+i][y+j].end());
				}
			}
			int count[10] = {0, 0,0,0,0,0, 0,0,0,0};
			for (vector<int>::iterator n = box.begin(); n != box.end(); ++n){
				count[*n]++;
			}
			for (int k = 1; k <= 9; ++k){
				if(count[k] == 1) {
					int checki = -1;
					int checkj = -1;
					for (int i = 0; i < 3; ++i){
						for (int j = 0; j < 3; ++j){
							if(set[x+i][y+j] == 0){
								for (vector<int>::iterator n = board[x+i][y+j].begin(); n != board[x+i][y+j].end() && checki == -1; ++n){
									if(*n == k){
										checki = i;
										checkj = j;
									}
								}
							}
						}
					}
					if(checki != -1){
						if (setValue(x+checki,y+checkj,k) == -1 ){
							return -1;
						}
						counter++;
					}
				}
			}
		}
	}
	return counter;
}
int Domain::applyR3(){
	applyR3R();
	applyR3C();
	applyR3B();
}

int Domain::applyR3R(){
	int counter = 0;
	for (int x = 0; x < 9; ++x){
		int c[9];
		for (int y = 0; y < 9; ++y){
			if(set[x][y] == 0){	
				c[y] = 0;
				for (vector<int>::iterator i = board[x][y].begin(); i != board[x][y].end(); ++i){
					c[y] *= 10;
					c[y] += *i;
				}
			} else {
				c[y] = -1;
			}
		}
		for (int y = 0; y < 9; ++y)
		{
			if(c[y] == -1){	
				int count = 0;
				for (int i = 0; i < 9; ++i)
				{
					if(c[y] == c[i]){
						count++;
					}
				}
				if(board[x][y].size() == count){
					int cy = c[y];
					for (int i = 0; i < 9; ++i)
					{
						if(cy == c[i]){
							c[i] = -1;
						} else {
							for (vector<int>::iterator n = board[x][y].begin(); n != board[x][y].end(); ++n)
							{
								if(removeV(x,i,*n) == 0){
									return -1;
								}
								counter++;
							}
						}
					}
				}
			}
		}
	}
	return counter;
}

int Domain::applyR3C(){
	int counter = 0;
	for (int y = 0; y < 9; ++y){
		int c[9];
		for (int x = 0; x < 9; ++x){
			if(set[x][y] == 0){	
				c[x] = 0;
				for (vector<int>::iterator i = board[x][y].begin(); i != board[x][y].end(); ++i){
					c[x] *= 10;
					c[x] += *i;
				}
			} else {
				c[x] = -1;
			}
		}
		for (int x = 0; x < 9; ++x)
		{
			if(c[x] == -1){	
				int count = 0;
				for (int i = 0; i < 9; ++i)
				{
					if(c[x] == c[i]){
						count++;
					}
				}
				if(board[x][y].size() == count){
					int cx = c[x];
					for (int i = 0; i < 9; ++i)
					{
						if(cx == c[i]){
							c[i] = -1;
						} else {
							for (vector<int>::iterator n = board[x][y].begin(); n != board[x][y].end(); ++n)
							{
								if(removeV(i,y,*n) == 0){
									return -1;
								}
								counter++;
							}
						}
					}
				}
			}
		}
	}
	return counter;
}

int Domain::applyR3B(){
	int counter = 0;
	for (int a = 0; a < 3; ++a){
		for (int b = 0; b < 3; ++b){
			int c[9];
			for (int i = 0; i < 9; ++i){
				int x = a*3 + i/3;
				int y = b*3 + i%3;
				if(set[x][y] == 0){	
					c[i] = 0;
					for (vector<int>::iterator h = board[x][y].begin(); h != board[x][y].end(); ++h){
						c[i] *= 10;
						c[i] += *h;
					}
				} else {
					c[i] = -1;
				}
			}
			for (int i = 0; i < 9; ++i)
			{
				int x = a*3 + i/3;
				int y = b*3 + i%3;
				if(c[i] == -1){	
					int count = 0;
					for (int j = 0; j < 9; ++j)
					{
						if(c[i] == c[j]){
							count++;
						}
					}
					if(board[x][y].size() == count){
						int ci = c[i];
						for (int j = 0; j < 9; ++j)
						{
							if(ci == c[j]){
								c[j] = -1;
							} else {
								for (vector<int>::iterator n = board[x][y].begin(); n != board[x][y].end(); ++n)
								{
									int x2 = a*3 + j/3;
									int y2 = b*3 + j%3;
									if(removeV(x2,y2,*n) == 0){
										return -1;
									}
									return counter++;
								}
							}
						}
					}
				}
			}
		}
	}
	return counter;
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
	cout << endl;
}

int Domain::nextCell(){
	for (int x = 0; x < 9; ++x)
	{
		for (int y = 0; y < 9; ++y)
		{
			if (set[x][y] == 0)
			{
				searchX = x;
				searchY = y;
				int counter = 0;
				for (std::vector<int>::iterator i = board[searchX][searchY].begin(); i != board[searchX][searchY].end(); ++i)
				{
					values[counter] = *i;
					counter++;
				}
				for (int i = counter; i < 9; ++i)
				{
					values[i] = 0;
				}
			}
		}
	}
}

// int checkSubDomain()
int Domain::findMostConstrained(){
	int max_constr = 0;
	int max_cell_x = 0;
	int max_cell_y = 0;
	int max_count[10] = {0, 0,0,0,0,0, 0,0,0,0};

	for (int x = 0; x < 9; ++x){
		for (int y = 0; y < 9; ++y){
			if (set[x][y] == 0){
				int count[10] = {0, 0,0,0,0,0, 0,0,0,0};
				for (int i = 0; i < 9; ++i){
					if (i !=x ){
						for (std::vector<int>::iterator n = board[i][y].begin(); n != board[i][y].end(); ++n){
							count[*n]++;
						}
					}
					if (i !=y ){
						for (std::vector<int>::iterator n = board[x][i].begin(); n != board[x][i].end(); ++n){
							count[*n]++;
						}
					}
					int a = x/3;
					int b = y/3;
					a *= 3;
					b *= 3;
					int k = a + (i % 3);
					int j = b + (i / 3);
					if (k != x && j != y){
						for (std::vector<int>::iterator n = board[k][j].begin(); n != board[k][j].end(); ++n){
							count[*n]++;
						}
					}
				}
				int num_constr = 0;
				for (std::vector<int>::iterator n = board[x][y].begin(); n != board[x][y].end(); ++n){
					num_constr += count[*n];
				}
				if(num_constr > max_constr){
					max_constr = num_constr;
					max_cell_x = x;
					max_cell_y = y;
					copy(count, count + 10, max_count);
				}
			}
		}
	}
	searchX = max_cell_x;
	searchY = max_cell_y;
	int count[10] = {0, 0,0,0,0,0, 0,0,0,0};
	int con[10] = {0, 0,0,0,0,0, 0,0,0,0};
	copy(max_count, max_count + 10, count);
	sort(count, count + 10);
	for (std::vector<int>::iterator i = board[searchX][searchY].begin(); i != board[searchX][searchY].end(); ++i)
	{
		con[*i] = 1;
	}
	int counter = 0;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 1; j <= 9; ++j)
		{
			if(count[i] != 0 && count[i] == max_count[j] && con[j] == 1){

				values[counter] = j;
				counter++;
				// cout << j << ": " << count[i] << endl;
				max_count[j] = 0;
			}
		}
	}
	for (int i = counter; i < 9; ++i)
	{
		values[i] = 0;
	}
	return 0;
}



Domain backtrack(Domain d){
	// d.printBoard();
	// char c;
	// cin >> c;
	// cout << "set value" << endl;
	if(d.counter != -1){
		if(d.setValue(d.searchX, d.searchY, d.values[d.counter]) == -1){
			return d;
		}
	}
	// cout << "rules" << endl;
	int counter = 1;
	while(counter != 0){
		counter = d.applyR1();
		// cout << counter << endl;
		if(counter == -1){
			return d;
		}
		int r = 0;
		if(d.rule2){
			r = d.applyR2();
		}
		if(r == -1){
			return d;
		}
		// cout << r << endl;
		counter += r;

		r = 0;
		if(d.rule3){
			r = d.applyR3();
		}
		if(r == -1){
			return d;
		}

		counter += r;

		// cout << counter << endl;
		// d.printBoard();
		// char c;
		// cin >> c;
	}
	// d.printBoard();
	d.spotsSearched = 0;
	for (int x = 0; x < 9; ++x)
	{
		for (int y = 0; y < 9; ++y)
		{
			if(d.set[x][y] != 0){
				d.spotsSearched++;
			}
		}
	}
	// cout << "searched" << d.spotsSearched << endl;
	if(d.spotsSearched == 81){
		// cout << "solved" << endl;
		d.solved = true;
		return d;
	}
	if(d.mostConstrained){
		d.findMostConstrained();
	} else {
		d.nextCell();
	}
	// cout << "(" << d.searchX << "," << d.searchY << ") ";
	// for (int i = 0; i < 9; ++i)
	// {
	// 	cout << d.values[i] << ", ";
	// }
	// cout << endl;
	for (int i = 0; i < 9; ++i)
	{
		d.counter = i;
		if(d.values[d.counter] == 0){
			return d;
		}
		Domain r = backtrack(d);
		if(r.solved){
			return r;
		}
		// cout << "backtrack" << endl;
		d.num_backtrack++;
	}
	return d;
}

int testFile(string filename,bool r2, bool r3 , bool c){
	string line;


	ifstream file (filename.c_str());
	if (file.is_open()){
		clock_t t;
		t = 0;
		int counter = 0;
		int bt_counter = 0;
		int bt_used_counter = 0;
		int given = 0;
		while ( getline (file,line) ){
			clock_t t1;
    		t1 = clock();
			Domain d (line);
			d.rule2 = r2;
			d.rule3 = r3;
			d.mostConstrained = c;
			Domain r = backtrack(d);
			t1 = clock() - t1;
			t += t1;
			// cout << "time " << t1 << endl;
			// cout << "backtrack " << r.num_backtrack << endl;
			bt_counter += r.num_backtrack;
			if(r.num_backtrack != 0){
				bt_used_counter++;
			}
			// cout << r.numsGiven <<endl;
			given += r.numsGiven;
			// r.printBoard();
			counter++;
		}

		cout << r2 << r3 << c;
		cout << ", " << (given*1.0)/counter;
		cout << ", " << (bt_used_counter*1.0)/counter;
		cout << ", "<< (bt_counter*1.0)/counter;
		cout << ", "<< bt_counter;
		cout << ", " << (t*1.0)/counter << endl;
		// cout << "settings " << r2 << r3 << c << endl;
		// cout << "numsGiven " << (given*1.0)/counter << endl;
		// cout << "\% of problems used " << (bt_used_counter*1.0)/counter << endl;
		// cout << "avg bt "<< (bt_counter*1.0)/counter << endl;
		// cout << "total bt "<< bt_counter << endl;
		// cout << "avg time " << (t*1.0)/counter << endl;
		// cout << endl;
		file.close();
	}
	else cout << "Unable to open file"; 
}


int main(int argc, char const *argv[]) {
	// Domain d ("120700000340000700560000000000000000000000000000000000000000000000000000000000000");
	// Domain d ("123000000456000000000000000070000000000000000000000000007000000000000000000000000");
	// Domain d ("120000700304700000056000000000000000000000000000000000000000000000000000000000000");
	// Domain d ("240300000000520407000046008610700084009060500730005061100470000302051000000002019");
	// Domain d ("003010008000400030870003020010009605300867002906500040020900074090006000500070100");
	// Domain d ("170000006006090040300070000000900030094020870030005000000060001080010500500000082");
 //    Domain d ("000006009090300108076000402000800005000502000900003000409000830605004090700100000");

	// // d.counter = -1;
	// Domain r = backtrack(d);

	// r.printBoard();

	// testFile("easy.txt");
	string filename (argv[1]);
	// testFile(filename,false,false,false);
	testFile(filename,false,false,true);
	// testFile(filename,true,false,false);
	testFile(filename,true,false,true);
	// testFile(filename,true,true,false);
	// testFile(filename,true,true,false);
	testFile(filename,true,true,true);

	// d.applyR1();

	// cout << d.board[0][0].size() << d.board[0][2][5] << endl;
	// d.printBoard();

	// cout << endl <<endl<<endl;

	// d.applyR2();
	// d.printBoard();

	// cout << endl <<endl<<endl;


	// d.applyR1();

	// // cout << d.board[0][0].size() << d.board[0][2][5] << endl;
	// d.printBoard();

	// cout << endl <<endl<<endl;

	// d.applyR2();
	// d.printBoard();


}
