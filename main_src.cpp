// main file for code 

#include "main_src.h"
#include "parser/file_reader.h"

using namespace std;


int main(int argv,char *argc[])
{
	
	/* get input */
	
	if(argv<3)
	{
		cout<<"Please enter file input , eg : ./exec control_file data_file \n";
		return 0;
	}

	bool is_target_front=false;
	Tries *root;
	
	vector<pair<vector<string>,string> > input_data;
	unordered_map<string,Attribute_feature*> attributes;
	vector<string> target_values;
	vector<string> a_list;
	vector<pair<string,bool> > is_cont;

	is_target_front=read_control_file(argc[1],a_list,is_cont);
	read_data_file(argc[2],input_data,is_target_front,attributes,a_list,target_values);	

	/* target value in front = true else false */


	/* add details of if attribute is continous or not is_cont */

	fill_cont_values(is_cont,attributes);

	
	//display_att_listing(attributes);

	root=form_decision_tree(input_data,attributes,target_values);


	/* Display Tree */

	cout<<"\n";
	cout<<"\n";
	cout<<"=========================================\n";
	cout<<"-------------OUTPUT---------------------------\n";
	cout<<"=========================================\n";
	cout<<"\n";
	cout<<"\n";

	vector<string> stack;
	display_tree(root,stack,0,0);

				
	return 0;
}

/* function to dipplay refined data contents */

void display_refine_data(vector<pair<vector<string>,string> > input_data)
{
	for(int i=0;i<input_data.size();i++)
	{
		vector<string> temp=input_data[i].first;
		for(string s : temp)
		{
			cout<<s<<"\t";
		}
		cout<<" ---> \t"<<input_data[i].second<<"\n";


	}
}
/* funnction to display attributes listing  */

void display_att_listing(unordered_map<string,Attribute_feature*> attributes)
{
	for(pair<string,Attribute_feature*> p : attributes)
	{
		cout<<p.first<<" ===> \t";
		for(string s : p.second->attribute_values)
		{
			cout<<s<<"\t";
		}
		cout<<"\n";
	}
}
/* function to display tree structure */

void display_tree(Tries *root,vector<string> stack,int i,int j)
{
	// display complete tree from root to leaf node

/*
	BASIC REPRESENTATION

	if(root!=NULL)
		cout<<"Parent : "<<i<<"\t"<<j<<"\t"<<root->attribute_name<<"\n";

	if(root->value.size()!=0)
	{
		for(int i=0;i<root->value.size();i++)
		{
			cout<<"Child :\t"<<root->attribute_name<<"value : \t"<<root->value[i].second<<"\n";
			display_tree(root->value[i].first,stack,i,j+1);
		}
	}
*/
	
	if(root==NULL || root->value.size()==0)
	{
		// target node reached 
		if(root!=NULL)
			stack.push_back(root->attribute_name);
		display_stack_contents(stack);
		return;
	}

	string att;

	stack.push_back(root->attribute_name);

	for(int i=0;i<root->value.size();i++)
	{
		att.assign(" = "+root->value[i].second);
		stack.push_back(att);
		display_tree(root->value[i].first,stack,i,j);
		stack.pop_back();
	}

	

}
/* function for tree display */

void display_stack_contents(vector<string> stack)
{
	for(string s : stack)
	{
		cout<<s<<"\t";
	}
	cout<<"\n";
}

/* function to get middle value from the range of continous values */

void get_mid_value(vector<pair<vector<string>,string> > input_data,string split_attribute,float& start,float& end)
{

	vector<string> lc_att=input_data[0].first;

	
	int get_attribute_colm=get_attribute_co(lc_att,split_attribute);

	for(int i=1;i<input_data.size();i++)
	{
		vector<string> _att=input_data[i].first;
		start=min(stof(_att[get_attribute_colm]),start);
		end=max(stof(_att[get_attribute_colm]),end);
	}	

}
/* function to return majority target value */

string get_majority_target(vector<pair<vector<string>,string> >& input_data)
{

	unordered_map<string,int> check_target;
	int max_value=INT_MIN;

	string target_value;

	for(int i=1;i<input_data.size();i++)
	{
		if(check_target.find(input_data[i].second)==check_target.end())
		{
			check_target.insert(make_pair(input_data[i].second,1));
		}
		else
		{
			check_target[input_data[i].second]++;
		}

		if(max_value<check_target[input_data[i].second])
		{
			max_value=check_target[input_data[i].second];
			target_value.assign(input_data[i].second);
		}
	}

	return target_value;

}
/* function to form decision tree */

Tries* form_decision_tree(vector<pair<vector<string>,string> >& input_data,unordered_map<string,Attribute_feature*> attributes,vector<string>& target_values)
{
	string target_val;

	//cout<<"DATA ===============================\n";
	//	display_refine_data(input_data);

//		cout<<"DATA END===============================\n";

	if(input_data.size()==1) return NULL;
	if(is_single_target(input_data,target_val) || is_empty_attribute(input_data,target_val,attributes))
	{
		Tries *node = new Tries(vector<string>(),target_val);
		//cout<<"Termination\n";
		return node;
	}
	else
	{
		
		string split_attribute=get_split_attribute_ig(attributes,input_data,target_values);
		cout<<split_attribute<<"\n";
		
		// check for discrete or continous attribute
		if(attributes[split_attribute]->is_cont==true)
		{
			// binary split
			float start=INT_MAX,end=INT_MIN;

			get_mid_value(input_data,split_attribute,start,end);
			float mid=(start+end)/2;
			
			// one 0 -> <=mid , mid > <=end

			//cout<<start<<"\t"<<end<<"\n";
			vector<string> temp;
			string tt;
			tt.assign("<="+to_string(mid));
			temp.push_back(tt);
			tt.assign(">"+to_string(mid));
			temp.push_back(tt);

			Tries *node = new Tries(temp,split_attribute);

			//cout<<"===========LEFT========================  "<<mid<<"\t"<<split_attribute<<"\n";
			vector<pair<vector<string>,string> > process_data_left=refine_data_cont(input_data,split_attribute,start,mid);
			display_refine_data(process_data_left);
			if(process_data_left.empty()==false)	
			node->value[0].first=form_decision_tree(process_data_left,attributes,target_values);
			else
			{
				// get majority value
			//	cout<<"Empty value \n";
				
				Tries *end_node = new Tries(vector<string>(),get_majority_target(input_data));
				node->value[0].first=end_node;
			}

			//cout<<"===========RIGHT======================== "<<mid<<"\t"<<split_attribute<<"\n";
			vector<pair<vector<string>,string> > process_data_right=refine_data_cont(input_data,split_attribute,mid,end);	
			display_refine_data(process_data_right);
			if(process_data_right.empty()==false)
			node->value[1].first=form_decision_tree(process_data_right,attributes,target_values);
			else
			{
				// get majority value
			//	cout<<"Empty value \n";

				Tries *end_node = new Tries(vector<string>(),get_majority_target(input_data));
				node->value[1].first=end_node;
			}
			
			//cout<<min_value<<"\t"<<max_value<<"\n";
			return node;
		}
		else
		{
			
			Tries *node = new Tries(attributes[split_attribute]->attribute_values,split_attribute);

			attributes.erase(split_attribute);
			for(int i=0;i<node->value.size();i++)
			{
				vector<pair<vector<string>,string> > process_data=refine_data(input_data,split_attribute,node->value[i].second);
				node->value[i].first=form_decision_tree(process_data,attributes,target_values);
			}
		
			return node;
		}
		
	}
}

/* function to determine all positive/negative targets */
bool is_single_target(vector<pair<vector<string>,string> >& input_data,string& target_value)
{

	unordered_set<string> check_target;

	for(int i=1;i<input_data.size();i++)
	{
		if(check_target.count(input_data[i].second)==0)
			check_target.insert(input_data[i].second);
	}

	if(check_target.size()==1)
	{
		for(string s : check_target)
		{
			target_value.assign(s);
		}
		//cout<<"Return false\n";
		return true;
	}
	
	return false;
}

/* function to check empty attribute and return majority value of target */

bool is_empty_attribute(vector<pair<vector<string>,string> >& input_data,string& target_value,unordered_map<string,Attribute_feature*>& attributes)
{
	if(attributes.empty()==true)
	{
		// get the majority target value from the remaining data

		// doesn't make much sense in case of continous data
		//cout<<"Attributes empty\n";
		unordered_map<string,int> check_target;
		int max_value=INT_MIN;

		for(int i=1;i<input_data.size();i++)
		{
			if(check_target.find(input_data[i].second)==check_target.end())
			{
				check_target.insert(make_pair(input_data[i].second,1));
			}
			else
			{
				check_target[input_data[i].second]++;
			}

			if(max_value<check_target[input_data[i].second])
			{
				max_value=check_target[input_data[i].second];
				target_value.assign(input_data[i].second);
			}
		}
		//		cout<<"Return Else false\n";
		return true;
	}
	else
	{

		return false;
	}
}
/* print processed data */

void print_process_data(vector<pair<vector<string>,string> > processed_data)
{

	for(int i=0;i<processed_data.size();i++)
	{
		for(string s : processed_data[i].first)
			cout<<s<<"\t";
		cout<<" --- >\t"<<processed_data[i].second<<"\n";

	}

}
/* get the split attribute */

string get_split_attribute_ig(unordered_map<string,Attribute_feature*>& attributes,vector<pair<vector<string>,string> >& input_data,vector<string>& target_values)
{
	string res_attrribute;

	// here split need to be done on attribute type
	// split method is information gain


	float entropy_s=get_entropy_overall(input_data);
	float total_s=input_data.size()-1;

	float max_gain=INT_MIN;

	//cout<<"Entropy : "<<entropy_s<<"\n";

	for(pair<string,Attribute_feature*> p : attributes)
	{
		if(p.second->is_cont==true)
		{
			// entropy deciding criteria different for discrete and continous attributes
			float start=0,end=0;
			get_mid_value(input_data,p.first,start,end);
			float mid=(start+end)/2;
			float ater=0;
			vector<pair<vector<string>,string> > process_data_left=refine_data_cont(input_data,p.first,start,mid);
			float total_sv=process_data_left.size()-1;
			float entropy_sv=get_entropy_overall(process_data_left);
			ater+=((total_sv/total_s)*entropy_sv);	

			vector<pair<vector<string>,string> > process_data_right=refine_data_cont(input_data,p.first,mid,end);
			float total_sv1=process_data_right.size()-1;
			float entropy_sv1=get_entropy_overall(process_data_right);
			ater+=((total_sv1/total_s)*entropy_sv1);	

			float ig=entropy_s-ater;
			//cout<<"=====================\n";
			//cout<<p.first<<"\t"<<ig<<"\n";
			if(max_gain<ig)
			{
				max_gain=ig;
				res_attrribute.assign(p.first);
			}

		}
		else
		{
			float ater=0;
			for(string s : p.second->attribute_values)
			{
				vector<pair<vector<string>,string> > processed_data=refine_data(input_data,p.first,s);
				float total_sv=processed_data.size()-1;
				//print_process_data(processed_data);
				float entropy_sv=get_entropy_overall(processed_data);
				ater+=((total_sv/total_s)*entropy_sv);	
			}
			//cout<<"\n";

			float ig=entropy_s-ater;
			//cout<<p.first<<"\t"<<ig<<"\n";
			if(max_gain<ig)
			{
				max_gain=ig;
				res_attrribute.assign(p.first);
			}
		}
	}

	

	// we need to choose the attribute with maximum gain

	//cout<<"Max -> "<<res_attrribute<<"\n";
	return res_attrribute;
}

/* get generic entropy for calculation of information gain */

float get_entropy_overall(vector<pair<vector<string>,string> >& input_data)
{
	unordered_map<string,float> _etr;

	float entropy_s=0.0;

	for(int i=1;i<input_data.size();i++)
	{
		if(_etr.find(input_data[i].second)==_etr.end())
		{
			_etr.insert(make_pair(input_data[i].second,1.0));
		}
		else
		{
			//cout<<_etr[input_data[i].second]<<"\n";
			_etr[input_data[i].second]++;
		}
	}

	float total_et=input_data.size()-1;

	for(pair<string,int> p : _etr)
	{
		entropy_s+=(-1.0*(p.second/total_et)*log2(p.second/total_et));
		//cout<<entropy_s<<"\t";
	}

	return entropy_s;
}
/* function to refine continous data */
vector<pair<vector<string>,string> > refine_data_cont(vector<pair<vector<string>,string> >& input_data,string split_attribute,float start,float end)
{

	// get data the range > start and <=end

	vector<pair<vector<string>,string> > process_data;
	

	vector<string> lc_att=input_data[0].first;

	
	int get_attribute_colm=get_attribute_co(lc_att,split_attribute);

	//cout<<split_attribute<<"\t"<<att_value<<"\t"<<get_attribute_colm<<"\n";
	// embedd attribute data
	pair<vector<string>,string> p;

	for(int i=0;i<lc_att.size();i++)
	{
		p.first.push_back(lc_att[i]);
	}
	p.second.assign("Attribute_List");

	process_data.push_back(p);

	//cout<<"Atrribute : "<<split_attribute<<"\t"<<get_attribute_colm<<"\n";
	for(int i=1;i<input_data.size();i++)
	{
		vector<string> _att=input_data[i].first;
		if(stof(_att[get_attribute_colm])>start && stof(_att[get_attribute_colm])<=end)
		{
			process_data.push_back(input_data[i]);
		}
	}

	return process_data;

}
/* refine data as per choice  */
vector<pair<vector<string>,string> > refine_data(vector<pair<vector<string>,string> >& input_data,string split_attribute,string att_value)
{
	vector<pair<vector<string>,string> > process_data;


	// in processed data the first row always corresponds to the list of attributes



	vector<string> lc_att=input_data[0].first;

	
	int get_attribute_colm=get_attribute_co(lc_att,split_attribute);

	//cout<<split_attribute<<"\t"<<att_value<<"\t"<<get_attribute_colm<<"\n";
	// embedd attribute data
	pair<vector<string>,string> p;

	for(int i=0;i<lc_att.size();i++)
	{
		p.first.push_back(lc_att[i]);
	}
	p.second.assign("Attribute_List");

	process_data.push_back(p);

	for(int i=1;i<input_data.size();i++)
	{
		vector<string> _att=input_data[i].first;
		if(_att[get_attribute_colm].compare(att_value)==0)
		{
			process_data.push_back(input_data[i]);
		}
	}
	

	return process_data;
}

/* get column in data corresponding to particular attribute */

int get_attribute_co(vector<string> lc_att,string split_attribute)
{

	int get_attribute_colm=0;
	for(int i=0;i<lc_att.size();i++)
	{
		if(lc_att[i].compare(split_attribute)==0)
		{
			get_attribute_colm=i;
			break;
		}
	}
	return get_attribute_colm;
}

