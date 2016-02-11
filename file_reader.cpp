#include<iostream>
#include<vector>
#include<string>
#include<sstream>

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
int main()
{
	vector<string> res;
	string s = "5.1, 3.5,1.4,0.2,Iris-setosa";
	
	spplit(s,',',res);
	return 0;
}