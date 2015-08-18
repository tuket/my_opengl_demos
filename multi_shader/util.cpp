#include "util.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

string loadTextFile(const string& fileName)
{
	
	stringstream ss;
	
	fstream fs;
	fs.open(fileName.c_str(), fstream::in);
	
	if(fs.fail())
	{
		cerr << "Error: could not open " << fileName << endl;
		return string();
	}
	
	ss << fs.rdbuf();
	
	return ss.str();
	
}
