

#include<sstream>
#include "file_reader.h"

using namespace std;

void spplit(string s,char del,vector<string>& res)
{
	stringstream ss(s);
	string item;
	while(getline(ss,item,del))
	{
		res.push_back(item);
	}

	for(string s : res)
	{
		cout<<s<<"\t";
	}
	cout<<"\n";
}

/* read data from control file */
bool read_control_file(string file_name,unordered_map<string,Attribute_feature*>& attributes)
{
	return true;
}

/* read actual data file */
void read_data_file(string file_name,vector<pair<vector<string>,string> >& input_data,bool is_target_front)
{
}
