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
	bool is_target_value;

	
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
		is_target_value=false;
	}

	~Tries()
	{
		/* destructor function */
	}
};

class Attribute_feature
{
	public:
	string min_value;
	string max_value;
	float mean;
	float sd;
	bool is_cont;
	bool has_empty;
	vector<string> attribute_values;

	Attribute_feature(vector<string> att_val)
	{
		for(string s : att_val)
		{
			attribute_values.push_back(s);
		}
	}
	~Attribute_feature()
	{
	}
};
