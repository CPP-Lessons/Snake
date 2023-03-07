#include<iostream>
#include<vector>
#include<algorithm>
#include<thread>
#include<string>
#include<mutex>
#include<windows.h>
#include<chrono>

using namespace std;

mutex mtx;


void Game_over(){
	vector<string> gm;
	gm.push_back("  ####   ####  ##   ## ##### ");
	gm.push_back(" ##     ##  ## ### ### ## ");
	gm.push_back(" ## ### ###### ## # ## #### ");
	gm.push_back(" ##  ## ##  ## ##   ## ## ");
	gm.push_back("  ####  ##  ## ##   ## ##### ");
	gm.push_back(" ");
	gm.push_back("  ####  ##  ## ##### ##### ");
	gm.push_back(" ##  ## ##  ## ##    ## ## ");
	gm.push_back(" ##  ## ##  ## ####  ##### ");
	gm.push_back(" ##  ##  ####  ##    ##  ## ");
	gm.push_back("  ####    ##   ##### ##  ## ");
	system("cls");
	int temp = 0;
	mtx.lock();
	for(int i = 0; i < 12; i++){
		for(int j = 0; j < temp; j++){
			cout << gm[j] << endl;
		}
	Sleep(100);
	temp += 1;
	if(i < 11){
	system("cls");
	}
	}
	mtx.unlock();	
}


int main(){
	thread thr(Game_over);
	thr.join();
}