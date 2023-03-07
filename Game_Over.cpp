
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
	int temp = 11;
	mtx.lock();
	for(int i = 10; i >= 0; i--){
		for(int j = i; j < temp; j++){
			cout << gm[j] << endl;
		}
	Sleep(100);
	if(i > 0){
	system("cls");
	}
	}
	mtx.unlock();	
}


int main(){
	thread thr(Game_over);
	thr.join();
}
