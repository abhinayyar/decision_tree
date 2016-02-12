// main file for code 

#include "main_src.h"
#include "parser/file_reader.h"

using namespace std;


int main()
{
	
	/* get input */

	bool is_target_front=false;
	Tries *root;
	string control_file_name,data_file_name;

	vector<pair<vector<string>,string> > input_data;
	unordered_map<string,Attribute_feature*> attributes;
	
	is_target_front=read_control_file(control_file_name,attributes);
	read_data_file(data_file_name,input_data,is_target_front);	

	/* target value in front = true else false */

	/* get the root node */

	root=create_tree(attributes,input_data);	

	/* insert nodes in decision tree */
	
	insert_node(root,attributes,input_data);
				

	/* TRIAL CODE 
	vector<string> res;
	res.push_back("abc");	
	res.push_back("dbc");	
	Tries *node = new Tries(res,"first");

	Tries *ptr=node;	
	res.push_back("vgd");
	Tries *node1= new Tries(res,"second");
	node->value[0].first=node1;
	ptr=ptr->value[0].first;

	cout<<ptr->attribute<<"\n";
	*/

	return 0;
}

/* initialize the tree with root node */

Tries *create_tree(unordered_map<string,Attribute_feature*>& attributes,vector<pair<vector<string>,string> >& input_data)
{
	string split_option;	
	split_option=get_initial_split_attribute(attributes,input_data);
	
	Tries *root=new Tries(attributes[split_option]->attribute_values,split_option);

	return root;
}

/* Insert item in decision tree */

void insert_node(Tries *root,unordered_map<string,Attribute_feature*>& attributes,vector<pair<vector<string>,string> >& input_data)
{
	for(int i=0;i<root->value.size();i++)
	{
		root->value[i].first	
	}		
}

/* get initial split attribute */
string get_split_attribute(unordered_map<string,Attribute_feature*>& attributes,vector<pair<vector<string>,string> >& input_data,)
{
	string split_option;

	return split_option;	
}

