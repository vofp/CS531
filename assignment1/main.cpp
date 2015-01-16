#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Room {
public:
	int length;
	int width;
	int prob;
	int **dirt;
	// int dirt_count;

	int set_values (int x, int y, int p) {
		width = x;
		length = y;
		prob = p;
		dirt = new int*[width];
		// dirt_count = 0;
		// srand (s);
		for (int i = 0; i < width; ++i) {
			dirt[i] = new int[length];
			for (int j = 0; j < length; ++j) {
				int r = rand() % 100;
				if(r >= p){
					dirt[i][j] = 0;
				}else{
					dirt[i][j] = 1;
					// dirt_count++;
				}
			}
		}
		return 0;
	}
	int display() {
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < length; ++j) {
				cout << dirt[i][j];
			}
			cout << endl;
		}
		return 0;
	}
	int dirt_count(){
		int count = 0;
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < length; ++j) {
				if (dirt[i][j])
				{
					count++;
				}
			}
		}
		return count;
	}
	int clean(int x, int y){
		if(dirt[x][y] == 1){
			// dirt_count -= 1;
		}
		dirt[x][y] = 0;
		return 0;
	}
};

class Agent {
public:
	enum Sensor {
		Wall = 1,
		Dirt = 2,
		Home = 4, 
	};
	int loc_x;
	int loc_y;
	int home_x;
	int home_y;
	int direction;
	Room r;
	//  0
	// 3 1
	//  2

	int set_room(Room e){
		r = e;
		loc_x = r.width -1;
		loc_y = 0;
		home_x = r.width -1;
		home_y = 0;
		direction = 0;
		return 0;
	}

	int forward(){
		int old_loc_x = loc_x;
		int old_loc_y = loc_y;
		switch(direction) {
			case 0: loc_x--; break;
			case 1: loc_y++; break;
			case 2: loc_x++; break;
			case 3: loc_y--; break;
		}
		if(loc_x < 0 || loc_x >= r.width || loc_y < 0 || loc_y >= r.length ) {
			loc_x = old_loc_x;
			loc_y = old_loc_y;
			return 1;
		}
		return 0;
	}

	int right(){
		direction++;
		if(direction > 3) {
			direction = 0;
		}
		return 0;
	}
	int left(){
		direction--;
		if(direction < 0) {
			direction = 3;
		}
		return 0;
	}
	int suck(){
		r.clean(loc_x, loc_y);
		// r.dirt_count -= 1;
		return 0;
	}
	int off(){
		return 0;
	}
	int display() {
		for (int i = 0; i < r.width; ++i) {
			for (int j = 0; j < r.length; ++j) {
				if(i == loc_x && j == loc_y){
					if(r.dirt[i][j] == 0) {
						cout << "C";
					} else {
						cout << "D";
					}
				} else {
					cout << r.dirt[i][j];
				}
			}
			cout << endl;
		}
		return 0;
	}
	
	int wall_sensor(){
		if(direction == 0 && loc_x == 0) {
			return 1;
		} 
		if(direction == 1 && loc_y == r.length-1) {
			return 1;
		}
		if(direction == 2 && loc_x == r.width-1) {
			return 1;
		}
		if(direction == 3 && loc_y == 0) {
			return 1;
		}
		return 0;
	}

	int dirt_sensor(){
		return r.dirt[loc_x][loc_y];
	}

	int home_sensor() {
		if(loc_x == home_x && loc_y == home_y) {
			return 1;
		}
		return 0;
	}

	int sensor(){
		int w = wall_sensor();
		int d = dirt_sensor();
		int h = home_sensor();

		int s = 0;
		if(w == 1) {
			s |= Wall;
		}
		if(d == 1) {
			s |= Dirt;
		}
		if(h == 1) {
			s |= Home;
		}
		return s;
	}

	int run1(){
		int count = 1;

		while(true) {
			count++;
			int s = sensor();
			if((s & Dirt) != 0) {
				suck();
			} else if(((s & Home) != 0) && ((s & Wall) != 0)) {
				return count;
			} else if((s & Wall) != 0) {
				right();
			} else {
				forward();
			}
		}
	}

	int run2(){
		int count = 1;
		// srand (time(NULL));
		while(true) {
			count++;
			int s = sensor();
			if((s & Dirt) != 0) {
				suck();
			} else if(((s & Home) != 0) && ((s & Wall) != 0)) {
				return count;
			} else if((s & Wall) != 0) {
				int r = rand() % 100;
				if(r < 50){
					right();
				}else{
					left();
				}
			} else {
				int r = rand() % 100;
				if(r < 50){
					forward();
				}else if(r < 75){
					left();
				}else{
					right();
				}
			}
		}
	}

	int run3(){
		// n+1 * m + (m%2 ? 0 : n ) + dirt sq + 

		int memory = 0;
		int count = 0;

		while(true) {
			count++;
			int s = sensor();
			if((s & Dirt) != 0) {
				suck();
			} else if(memory == 0 && (s & Wall) != 0) {
				right();
				memory = 1;
			} else if(memory == 1) {
				if((s & Wall) != 0) {
					right();
					memory = 6;
				} else {
					forward();
					memory = 2;
				}
			} else if(memory == 2) {
				right();
				memory = 3;
			} else if(memory == 3 && (s & Wall) != 0) {
				left();
				memory = 4;
			} else if(memory == 4) {
				if((s & Wall) != 0) {
					right();
					memory = 6;
				} else {
					forward();
					memory = 5;
				}
			} else if(memory == 5) {
				left();
				memory = 0;
			} else if(memory == 6 && (s & Wall) != 0) {
				right();
				memory = 7;
			} else if(memory == 7 && (s & Home) != 0) {
				return count;
			} else {
				forward();
			}
			display();
			cout << endl;
		}
		return 0;
	}
};


int main(int argc, char const *argv[]) {
	// cout << "test" << endl;
	srand (time(NULL));
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	int p = atoi(argv[3]);

	// int s = rand() % 10000;
	Room r;

	int clean_count1 = 0;
	float clean_count2 = 0;
	int clean_count3 = 0;

	int action_count1 = 0;
	float action_count2 = 0;
	int action_count3 = 0;

	for (int i = 0; i < 1; ++i)
	{
		r.set_values(n,m,p);
		int dirt_count = r.dirt_count();
		Agent a;
		a.set_room(r);
		action_count1 += a.run1();
		clean_count1 += dirt_count - r.dirt_count();
		// cout << n << ", " << m << ", " << p << ", 1, " << action_count << ", " << dirt_count - r.dirt_count() << endl;

		r.set_values(n,m,p);
		dirt_count = r.dirt_count();
		a.set_room(r);
		action_count3 += a.run3();
		clean_count3 += dirt_count - r.dirt_count();
		// cout << n << ", " << m << ", " << p << ", 3, " << action_count << ", " << dirt_count - r.dirt_count() << endl;

		int clean_count_temp = 0;
		int action_count_temp = 0;
		for (int i = 0; i < 100; ++i){
			r.set_values(n,m,p);
			dirt_count = r.dirt_count();
			a.set_room(r);
			action_count_temp += a.run2();
			clean_count_temp += dirt_count - r.dirt_count();
		}
		clean_count2 += clean_count_temp / 100.0;
		action_count2 += action_count_temp / 100.0;
		// cout << n << ", " << m << ", " << p << ", 2, " << action_count/100.0 << ", " << clean_count/100.0 << endl;
	}
	cout << n << ", " << m << ", " << p << ", 1, " << action_count1/100.0 << ", " << clean_count1/100.0 << endl;
	cout << n << ", " << m << ", " << p << ", 2, " << action_count2/100.0 << ", " << clean_count2/100.0 << endl;
	cout << n << ", " << m << ", " << p << ", 3, " << action_count3/100.0 << ", " << clean_count3/100.0 << endl;

	return 0;
}
