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
	if(m + p != 10){
		cout << "wrong m and p value, m + p need equal 10" << endl;
		return;
	}
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
		int row_num = insts[i].addr % int(pow(2,p));
		int col_num = gbh % int(pow(2,m));
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

		if(insts[i].result == 'T')
			gbh = gbh << 1 | 1;
		else
			gbh = gbh << 1 | 0;
	//	cout << "insts[" << i << "]:" << insts[i].result << endl;
	//	cout << "pht[" << row_num << "][" << col_num << "]: " << pht[row_num][col_num] << endl;
	//	cout << "gbh: " << gbh << endl;
	}
	//cout << "Two-Level Global Branch Predictor:" << endl;
	cout << mispredicted << "," << insts.size() << ",";
	cout << double(mispredicted) / double(insts.size()) * 100  << "%" << endl;
}

void l2lbp(vector<_inst>insts, int k, int m, int p){
	if(m + p != 10){
		cout << "wrong m and p value, m + p need equal 10" << endl;
		return;
	}
	else if(int(pow(2,k)) * m != 64){
		cout << "wrong k and m value, (2 ^ k) * m need equal 64" << endl;
		return;
	}
	vector<unsigned int> lbh;
	vector<vector<int>> pht;
	int mispredicted = 0;

	// initilization
	for(int i = 0; i < pow(2,k); i++){
		lbh.push_back(0);
	}
	
	for(int i = 0; i < pow(2,p); i++){
		vector<int>temp;
		for(int j = 0; j < pow(2,m); j++){
			temp.push_back(3);
		}
		pht.push_back(temp);
	}

	for(int i = 0; i < insts.size(); i++){
		int row_num = insts[i].addr % int(pow(2,p));
		int index_lbh = insts[i].addr % int(pow(2,k));
		int col_num =  lbh[index_lbh] % int(pow(2,m));
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

		if(insts[i].result == 'T')
			lbh[index_lbh] = lbh[index_lbh] << 1 | 1;
		else
			lbh[index_lbh] = lbh[index_lbh] << 1 | 0;
	//	cout << "insts[" << i << "]:" << insts[i].result << endl;
	//	cout << "pht[" << row_num << "][" << col_num << "]: " << pht[row_num][col_num] << endl;
	//	cout << "gbh: " << gbh << endl;
	}
	//cout << "Two-Level local Branch Predictor:" << endl;
	cout << mispredicted << "," << insts.size() << ",";
	cout << double(mispredicted) / double(insts.size()) * 100  << "%" << endl;
}


int main(int argc, char *argv[]){
	string mode = argv[1];
	if(mode != "g" && mode != "l"){
		cout << "error mode parameter" << endl;
		exit(1);
	}
	fstream fin;
	fin.open(argv[2],ios::in);
	if(!fin){
		cout << "Can't open file: " << argv[2] << endl;
		exit(1);
	}
	vector<_inst>insts;
	_inst temp_i;
	while(fin >> temp_i.addr >> temp_i.result){
		insts.push_back(temp_i);
	}
	
	cout << argv[2] << ",";
	if(mode == "g"){
		if(argc == 5)
			l2gbp(insts,atoi(argv[3]),atoi(argv[4]));
		else
			cout << "error parameter count" << endl;
	}
	else if(mode == "l"){
		if(argc == 6)
			l2lbp(insts,atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
		else
			cout << "error parameter count" << endl;
	}
}
