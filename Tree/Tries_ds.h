/* file for representing DS for a tree node */

#include<vector>
#include<unordered_map>
#include<utility>

using namespace std;

class Tries
{
	public:

	string attribute_name;

	vector<pair<Tries*,string> > value;

	
	Tries(vector<string> values_str,string atr)
	{
		for(string s : values_str)
		{
			pair<Tries*,string> p;
			p.first=NULL;		
			p.second.assign(s);
			value.push_back(p);
		}
		attribute_name.assign(atr);
	}

	~Tries()
	{
		/* destructor function */
	}
};

class Attribute_feature
{
	public:
	int min_value;
	int max_value;
	int mean;
	int sd;
	vector<string> attribute_values;
	vector<string> target_values;

	Attribute_feature(int min_val,int max_val,int m,int s,vector<string> att_val)
	{
		min_value=min_val;
		max_value=max_val;
		mean=m;
		sd=s;

		for(string s : att_val)
		{
			attribute_values.push_back(s);
		}
	}
	~Attribute_feature()
	{
	}
};
