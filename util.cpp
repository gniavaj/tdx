#include "stdafx.h"
#include<fstream>
#include<iomanip>
#include <string>
#include <sstream>
using namespace std;

void tdxLog(string mes, int o) {

	if (o==0) {
		return;
	}
	ofstream ofile;        

    ofile.open("c:\\tdxlog.txt", ios::app);     
   
	ofile<<mes<<endl;  

    ofile.close(); 
}

string intToString(int a) {

	ostringstream temp; 
    temp<<a;
	return temp.str();
}

string floatToString(float a) {

	ostringstream temp; 
    temp<<a;
	return temp.str();
}

string shortToString(short a) {

	ostringstream temp; 
    temp<<a;
	return temp.str();
}

string longToString(long a) {

	ostringstream temp; 
    temp<<a;
	return temp.str();
}

string unsignedShortToString(unsigned short a) {

	ostringstream temp; 
    temp<<a;
	return temp.str();
}

string unsignedCharToString(unsigned char a) {
	int s = a;
	return intToString(s);
}

string charPointToString(char* a) {
	string s = a;
	return s;
}