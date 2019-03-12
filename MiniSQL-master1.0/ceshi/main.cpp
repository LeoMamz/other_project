#include "Declaration.h"
#include "API.h"
#include "BufferManager.h"
#include <time.h> 
int main() {
	API api;
	string statement;
	//test file
	double start = clock(); 
	api.Exec_File("3000.txt");
	//cout << "Successful!!!!" << endl;
	double end = clock();
	cout << "cost time is " << (end - start)/CLOCKS_PER_SEC << "seconds" << endl;
	while (!api.Check_Finish()) {
		getline(cin, api.inputline);
		start = clock(); 
		api.dealString();
		end = clock();
		cout << "cost time is " << (end - start)/CLOCKS_PER_SEC << "seconds" << endl;
	}


}
