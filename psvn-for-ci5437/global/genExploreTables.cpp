#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

int main(){
	double b;
	long long nds, d, prev_nds, prev_d;
	string nodes, at, depth;

	cin>>nds>>nodes>>at>>depth>>d;
	prev_nds = nds;
	prev_d = d;

	while(cin>>nds>>nodes>>at>>depth>>d){
		b = (double)nds/(double)prev_nds;
		cout<<prev_nds<<" nodes "<<at<<" depth "<<prev_d<<" "<<b<<endl;
		prev_nds = nds;
		prev_d = d;
	}
	cout<<prev_nds<<" nodes "<<at<<" depth "<<prev_d<<" "<<"--"<<endl;
	return 0;
}