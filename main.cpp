#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

class _inst{
public:
	_inst(){ };
	_inst(unsigned long int _addr, char _result) : addr(_addr), result(_result){ };
	~_inst(){ };
	unsigned long int addr;
	char result;
};

void l2gbp(vector<_inst>insts, int m, int p){
	unsigned int gbh = 0;
	vector<vector<int>> pht;
	int mispredicted = 0;

	// initilization
	for(int i = 0; i < pow(2,p); i++){
		vector<int>temp;
		for(int j = 0; j < pow(2,m); j++){
			temp.push_back(3);
		}
		pht.push_back(temp);
	}

	for(int i = 0; i < insts.size(); i++){
		if(insts[i].result == 'T')
			gbh = gbh << 1 | 1;
		else
			gbh = gbh << 1 | 0;
		
		int row_num = insts[i].addr % int(pow(2,p));
		int col_num = gbh % int(pow(2,m));
		col_num  = 0;
		switch(pht[row_num][col_num]){
			case 0: // strongly non-taken
				if(insts[i].result == 'T'){
					pht[row_num][col_num]++;
					mispredicted++;
				}
				break;
			case 1: // weakly non-taken
				if(insts[i].result == 'T'){
					pht[row_num][col_num]++;
					mispredicted++;
				}
				else{
					pht[row_num][col_num]--;
				}
				break;
			case 2: // weakly taken
				if(insts[i].result == 'T'){
					pht[row_num][col_num]++;
				}
				else{
					pht[row_num][col_num]--;
					mispredicted++;
				}
				break;
			case 3: // strongly taken
				if(insts[i].result == 'N'){
					pht[row_num][col_num]--;
					mispredicted++;
				}
				break;
			default:
				cout << "bad 2-bit predictor value" << endl;
		}
	//	cout << "insts[" << i << "]:" << insts[i].result << endl;
	//	cout << "pht[" << row_num << "][" << col_num << "]: " << pht[row_num][col_num] << endl;
	//	cout << "gbh: " << gbh << endl;
	}
	cout << "mispredicted: " << mispredicted << " all insts: " << insts.size() << endl;
	cout << "mispredicted rate: " << double(mispredicted) / double(insts.size()) * 100  << "%" << endl;

}


int main(int argc, char *argv[]){
	fstream fin;
	fin.open(argv[1],ios::in);
	if(!fin){
		cout << "Can't open file: " << argv[1] << endl;
		exit(1);
	}
	vector<_inst>insts;
	_inst temp_i;
	while(fin >> temp_i.addr >> temp_i.result){
		insts.push_back(temp_i);
	}

	l2gbp(insts,2,8);
/*	for(int i = 0; i < insts.size(); i++){
		cout << insts[i].addr << " " << insts[i].result << endl;
	}*/

}
