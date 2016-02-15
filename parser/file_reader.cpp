

#include<sstream>
#include<fstream>
#include "file_reader.h"

using namespace std;

vector<string> spplit(string s,char del)
{
	stringstream ss(s);
	string item;
	vector<string> res;
	while(getline(ss,item,del))
	{
		res.push_back(item);
	}

	return res;
}

/* read data from control file */
bool read_control_file(string file_name,vector<string>& a_list,vector<pair<string,bool> >& is_cont)
{
	ifstream ifile;

	ifile.open(file_name);

	vector<string> input(2);
	for(int i=0;i<2;i++)
	{
		ifile >> input[i];
	}

	a_list = spplit(input[0],',');

	vector<string> temp=spplit(input[1],',');
	for(int i=0;i<temp.size();i++)
	{
		pair<string,bool> p;
		p.first=a_list[i];
		if(temp[i].compare("0")==0)
		{
			p.second=false;
		}
		else
		{
			p.second=true;
		}
		is_cont.push_back(p);
	}

	//for(string s : a_list)
	//	cout<<s<<"\t";
	//cout<<"\n";
	// O means at back and 1 means at front
	if(a_list.back().compare("0")==0)
	{
		a_list.pop_back();
		//cout<<"Return false not--------------\n";
		return false;
	} 
	else
	{
		a_list.pop_back();
		//cout<<"Return true not--------------\n";
		return true;
	}
}
/* fill if values are continous or not */
void fill_cont_values(vector<pair<string,bool> >& is_cont,unordered_map<string,Attribute_feature*>& attributes)
{
	for(int i=0;i<is_cont.size();i++)
	{
		attributes[is_cont[i].first]->is_cont=is_cont[i].second;

		//cout<<is_cont[i].first<<" ---> \t";

		//if(is_cont[i].second==true)
		//{
		//	sort(attributes[is_cont[i].first]->attribute_values.begin(),attributes[is_cont[i].first]->attribute_values.end());
		//	attributes[is_cont[i].first]->min_value.assign(attributes[is_cont[i].first]->attribute_values[0]);
		//	attributes[is_cont[i].first]->max_value.assign(attributes[is_cont[i].first]->attribute_values[attributes[is_cont[i].first]->attribute_values.size()-1]);
			//for(string s : attributes[is_cont[i].first]->attribute_values)
			//	cout<<s<<"\t";
			//cout<<"\n";
		//}
	}
}

/* check contents */
void check_contents(vector<string> line_att)
{
	for(string s : line_att)
		cout<<s<<"\t";
	cout<<"\n";
}

void display_tracker_content(vector<pair<string,vector<string> > > tracker)
{
	for(int i=0;i<tracker.size();i++)
	{
		cout<<tracker[i].first<<" ----> \t";
		for(string s : tracker[i].second)
		{
			cout<<s<<"\t";
		}
		cout<<"\n";
	}
}
/* read actual data file */
void read_data_file(string file_name,vector<pair<vector<string>,string> >& input_data,bool is_target_front,unordered_map<string,Attribute_feature*>& attributes,vector<string>& a_list,vector<string>& target_values)
{
		// zero row is always the attribute list with target value as Attribute_List


	ifstream ifile;

	ifile.open(file_name);

	// write list of attrubute to zero row;


	vector<string> att_list;

	vector<pair<string,vector<string> > > tracker;
	unordered_set<string> target_tracker;

	for(string s : a_list)
	{
		vector<string> temp;
		tracker.push_back(make_pair(s,temp));
	}

	string input;

	while(ifile >> input)
	{
		vector<string> line_att= spplit(input,',');
		//check_contents(line_att);
		//cout<<line_att[9]<<"**********\n";
		string target_value;

		
		if(is_target_front)
		{
			target_value.assign(line_att[0]);
			line_att.erase(line_att.begin());
		}
		else
		{
			target_value.assign(line_att[line_att.size()-1]);
			line_att.pop_back();
		}

		//cout<<target_value<<"\n";
		

		// add value to tracker

		for(int i=0;i<line_att.size();i++)
		{
			if(find(tracker[i].second.begin(),tracker[i].second.end(),line_att[i])==tracker[i].second.end())
				tracker[i].second.push_back(line_att[i]);
		}

		if(target_tracker.count(target_value)==0)
			target_tracker.insert(target_value);

		
		input_data.push_back(make_pair(line_att,target_value));


	}

	
	for(string s : target_tracker)
	{
		//cout<<s<<"\t";
		target_values.push_back(s);
	}
	//display_tracker_content(tracker);

	for(int i=0;i<tracker.size();i++)
	{
		// min , max , mean ,sd values can be configured from here 
		Attribute_feature *att_node = new Attribute_feature(tracker[i].second);
		attributes.insert(make_pair(tracker[i].first,att_node));
	}

	for(string s : a_list)
	{
		att_list.push_back(s);
	}

	input_data.insert(input_data.begin(),make_pair(att_list,"Attribute_List"));
	
}
