// main file for code 

#include "main_src.h"
#include "parser/file_reader.h"

using namespace std;

#define LIMIT 5
//#define CLASS_LABEL 0

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
	int data_size=0;
	int pass_count=0,leaf=0;
	vector<pair<vector<string>,string> > input_data;
	vector<pair<vector<string>,string> > test_data;
	unordered_map<string,Attribute_feature*> attributes;
	vector<string> target_values;
	vector<string> a_list;
	vector<pair<string,bool> > is_cont;

	is_target_front=read_control_file(argc[1],a_list,is_cont,data_size);
	read_data_file(argc[2],input_data,is_target_front,attributes,a_list,target_values,test_data,data_size);	

	/* target value in front = true else false */


	/* add details of if attribute is continous or not is_cont */

	fill_cont_values(is_cont,attributes);

	
	// flll missing value data 

	//display_refine_data(input_data);
	//cout<<"\n====\n";
	pre_process_data(input_data);

	//display_refine_data(input_data);
	//display_att_listing(attributes);

	root=form_decision_tree(input_data,attributes,target_values,leaf);


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

	/* test the accuracy of decision tree */	
	pass_count=test_decision_tree(root,test_data,a_list,attributes);
	//cout<<"======== PASS COUNT ========\n";
	//cout<<pass_count<<"\n";	
	float per=((float)pass_count/(float)test_data.size())*100;
	cout<<"\n";
	cout<<"========PERCENTAGE PASS========\n";
	cout<<per<<"\n";	


	// prune tree minimum error prunning , all class attributes are equally likely

	me_prune_tree(root);

	// diplay prune tree

	cout<<"\n";
	cout<<"\n";
	cout<<"=========================================\n";
	cout<<"------------- PRUNNED OUTPUT (Minimun Error Prunning )---------------------------\n";
	cout<<"=========================================\n";
	cout<<"\n";
	cout<<"\n";

	vector<string> stack_prune;
	display_tree(root,stack_prune,0,0);

	/* test the accuracy of prunned decision tree */	
	pass_count=test_decision_tree(root,test_data,a_list,attributes);
	//cout<<"======== PASS COUNT ========\n";
	//cout<<pass_count<<"\n";	
	float per_pur=((float)pass_count/(float)test_data.size())*100;
	cout<<"\n";
	cout<<"========PERCENTAGE PASS========\n";
	cout<<per_pur<<"\n";


	// error complexity pruning , all class attributes are not equally likely

	ec_prune_tree(root,test_data.size());


	cout<<"\n";
	cout<<"\n";
	cout<<"=========================================\n";
	cout<<"------------- PRUNNED OUTPUT (Error complexity Prunning )---------------------------\n";
	cout<<"=========================================\n";
	cout<<"\n";
	cout<<"\n";

	vector<string> stack_prune_ec;
	display_tree(root,stack_prune_ec,0,0);

	/* test the accuracy of prunned decision tree */	
	pass_count=test_decision_tree(root,test_data,a_list,attributes);
	//cout<<"======== PASS COUNT ========\n";
	//cout<<pass_count<<"\n";	
	float per_pur_ec=((float)pass_count/(float)test_data.size())*100;
	cout<<"\n";
	cout<<"========PERCENTAGE PASS========\n";
	cout<<per_pur_ec<<"\n";

	return 0;
}

/* function to cal for each node */
float node_check(Tries *root,float test_size,vector<pair<vector<Tries*>,float> >& p_tr,vector<Tries*>& stack)
{

	if(!root || root->value.size()==0) return 0;


	float error_rate=0,error=0,max_e=0,total_v=0;
	float alpha=0;

	for(pair<string,int> p : root->class_pair)
	{
		if(max_e<p.second)
		{
			max_e=p.second;
		}
		total_v+=p.second;
	}

	error_rate=(total_v-max_e)/test_size;

	float not_prunned=0;

	for(int i=0;i<root->value.size();i++)
	{
		not_prunned+=node_check(root->value[i].first,test_size,p_tr,stack);
	}


	alpha=(error_rate-not_prunned)/(root->num_leaves-1);

	stack.push_back(root);
	p_tr.push_back(make_pair(stack,alpha));

	return error_rate;
}

/* final prune tree for particular value of alpha */
void final_prune(Tries *root,vector<Tries*> res)
{
	if(!root || root->value.size()==0 || res.size()==0) return;

	int i=0;
	bool is_there=false;
	int majority=INT_MIN;
	string maj_name;
	for(Tries *ptr : res)
	{
		if(ptr==root)
		{
			// need to prune corresponding to this 
			is_there=true;

			for(pair<string,int> p : root->class_pair)
			{
		

				if(majority<p.second)
				{
					majority=p.second;
					maj_name.assign(p.first);
				}
			}

			while(root->value.size()>1)
			{
				root->value.pop_back();
			}
			Tries *end_node= new Tries(vector<string>(),maj_name);
			end_node->is_end=true;
			root->value[0].first=end_node;

		}
		i++;
	}

	for(int i=0;i<root->value.size();i++)
	{
		final_prune(root->value[i].first,res);
	}
}
/* function to do error complexity pruning */

void ec_prune_tree(Tries *root,float test_size)
{

	// error complexity pruning method

	vector<pair<vector<Tries*>,float> > p_tr;
	vector<Tries*> stack;

	float target=0;

	for(int i=0;i<root->value.size();i++)
	{
		target=node_check(root->value[i].first,test_size,p_tr,stack);
	}

	// prune the tree on the basis of smallest value of alpha

	vector<Tries*> res;

	float min_e=test_size;
	for(int i=0;i<p_tr.size();i++)
	{
		if(p_tr[i].second<min_e)
		{
			min_e=p_tr[i].second;
			res=p_tr[i].first;
			//cout<<min_e<<"\n";
		}
	}

	//for(Tries *s : res)
	//{
	//	cout<<s->attribute_name<<"\t";
	//}
	//cout<<"\n";
	// root need not to be proned

	// prune tree for attributes corresponding to smallest value of alpha

	final_prune(root,res);
}
/* function to check prunning per node */

float check_each_node(Tries *root,float prev_total)
{


	if(!root || root->value.size()==0) return 0;

	// value corresponding to not pruning sub tree
	float target=0;


	float total_v=0,majority=INT_MIN,class_nu=root->class_pair.size();
	string maj_name;

	for(pair<string,int> p : root->class_pair)
	{
		total_v+=p.second;

		if(majority<p.second)
		{
			majority=p.second;
			maj_name.assign(p.first);
		}
	}

	for(int i=0;i<root->value.size();i++)
	{
		target+=check_each_node(root->value[i].first,total_v);
	}


	// value corresponding to prunning 

	float prunning_value=0;


	prunning_value=(total_v - majority + class_nu -1)/(total_v+class_nu);

	if(prunning_value<target)
	{
		// we must prune the tree;

		while(root->value.size()>1)
		{
			//delete root->value.back().first;
			root->value.pop_back();
		}

		Tries *end_node= new Tries(vector<string>(),maj_name);
		end_node->is_end=true;
		root->value[0].first=end_node;
	}

	return (total_v/prev_total)*prunning_value;

}
/* function to prune decision tree */

void me_prune_tree(Tries *root)
{

	// we can cannot prune root

	// pruning method : minimum error pruning

	float target=0;

	float prev_total=0;

	for(pair<string,int> p : root->class_pair)
	{
		prev_total+=p.second;
	}

	for(int i=0;i<root->value.size();i++)
	{
		target+=check_each_node(root->value[i].first,prev_total);
	}

	// we need not to prune root so no need to compare target
}
/* function to test accuracy of decision tree */
int test_decision_tree(Tries *root,vector<pair<vector<string>,string> > test_data,vector<string> a_list,unordered_map<string,Attribute_feature*> attributes)
{
	int pass_count=0;
	for(int i=0;i<test_data.size();i++)
	{
		pass_count+=check_each_data(root,test_data[i],a_list,attributes);
	}
	return pass_count;
}

/* test for each data */
int check_each_data(Tries *root,pair<vector<string>,string> t_data,vector<string> a_list,unordered_map<string,Attribute_feature*> attributes)
{
	int get_attribute_colm=get_attribute_co(a_list,root->attribute_name);
	//cout<<root->attribute_name<<"\n";
	if(get_attribute_colm!=-1)
	{	
		if(attributes[root->attribute_name]->is_cont==false)
		{
			int value_index=get_value_index(root,t_data.first[get_attribute_colm]);
			if(value_index!=-1)
			return check_each_data(root->value[value_index].first,t_data,a_list,attributes);
			else
			{
				//cout<<"=========== WRONG DATA RETRIEVED ========\n";
				return 0;
			}
		}
		else
		{
			//cout<<"cont attribute\n";
			int value_index=get_cont_value_index(root,t_data.first[get_attribute_colm]);
			return check_each_data(root->value[value_index].first,t_data,a_list,attributes);
		}
	}
	else if(get_attribute_colm==-1 && root->is_end==true)
	{
		if(root->attribute_name.compare(t_data.second)==0)
			return 1;
		else
			return 0;
	}
	return 0;	
}

/* function to get value index for continous attribute */
int get_cont_value_index(Tries *root,string value)
{
	string left=root->value[0].second;
	string right=root->value[1].second;

	left.erase(left.begin());
	left.erase(left.begin());
	right.erase(right.begin());

	float left_value=stof(value);
	float act_value=stof(left);

	if(left_value<=act_value)
		return 0;
	else
		return 1;
	//cout<<left<<"\t"<<right<<"\n";
}
/* function to get index for a value */

int get_value_index(Tries *root,string value)
{
	for(int i=0;i<root->value.size();i++)
	{
		if(root->value[i].second.compare(value)==0)
			return i;
	}
	return -1;
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

	// get value corresponding to each node
	// define class label if you want to test class labels
	
#ifdef CLASS_LABEL
	string sub="( ";
	for(pair<string,int> p : root->class_pair)
	{
		sub+=to_string(p.second)+"  ";
	}
	sub+=" )";
	stack.push_back(sub);
#endif

#ifdef DISLEAF

	string ll=" [ " + to_string(root->num_leaves) + " ] ";
	stack.push_back(ll);
#endif


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
		if(_att[get_attribute_colm].empty()==false)
		{
			start=min(stof(_att[get_attribute_colm]),start);
			end=max(stof(_att[get_attribute_colm]),end);
		}
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
/* function get predicted values */

string get_predicted_value(vector<string>& target_values)
{
	string target_value;

	unordered_map<string,int> dict;
	int max_value=0;

	for(string s : target_values)
	{
		if(dict.find(s)==dict.end())
		dict.insert(make_pair(s,1));
		else
		dict[s]++;

		if(max_value<dict[s])
		{
			max_value=dict[s];
			target_value.assign(s);
		}
	}


	return target_value;


}
/* function to get class pair for continous data input */

unordered_map<string,int>  get_class_pair_cont(vector<pair<vector<string>,string> >& input_data,float mid)
{
	unordered_map<string,int> lc_pair;

	string left = "<="+to_string(mid);
	string right = ">"+to_string(mid);
	lc_pair.insert(make_pair(left,0));
	lc_pair.insert(make_pair(right,0));


	// implementation of code assuning continous target value case : need to track
	/*

	for(int i=0;i<input_data.size();i++)
	{

		
		if(stof(p.second)<=mid)
		{
			lc_pair[left]++;
		}
		else
		{
			lc_pair[right]++;
		}
		
	}
	*/

	return lc_pair;

}
/* function to get class pair */
unordered_map<string,int>  get_class_pair(vector<pair<vector<string>,string> >& input_data,float& total_v)
{
	unordered_map<string,int> lc_pair;

	for(pair<vector<string>,string> p : input_data)
	{
		if(lc_pair.find(p.second)==lc_pair.end())
		{
			lc_pair.insert(make_pair(p.second,1));
		}
		else
		{
			lc_pair[p.second]++;
		}
		total_v+=lc_pair[p.second];
	}

	return lc_pair;
}
/* function to form decision tree */

Tries* form_decision_tree(vector<pair<vector<string>,string> >& input_data,unordered_map<string,Attribute_feature*> attributes,vector<string>& target_values,int& leaf)
{
	string target_val;

	int left_leaf=0,right_leaf=0;
	//cout<<"DATA ===============================\n";
	//	display_refine_data(input_data);

//		cout<<"DATA END===============================\n";

	if(input_data.size()<=LIMIT)
	{
		//Tries *node = new Tries(vector<string>(),"Majority value");
		string majority_value=get_majority_target(input_data);
		if(majority_value.empty()==false)
		{
			Tries *end_node = new Tries(vector<string>(),majority_value);
			end_node->is_end=true;
			leaf++;
			return end_node;
		}
		else
		{
			Tries *end_node = new Tries(vector<string>(),get_predicted_value(target_values));
			end_node->is_end=true;
			leaf++;
			return end_node;
		}
	} 
	if(is_single_target(input_data,target_val) || is_empty_attribute(input_data,target_val,attributes))
	{
		Tries *end_node = new Tries(vector<string>(),target_val);
		end_node->is_end=true;
		leaf++;
		return end_node;
	}
	else
	{
		
		string split_attribute=get_split_attribute_ig(attributes,input_data,target_values);
		
		//cout<<split_attribute<<"\n";
		
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
			// may differ if handling done for continous target attribute too , but id3 not good in that
			node->class_pair=get_class_pair(input_data,node->total_v);

			//cout<<"===========LEFT========================  "<<mid<<"\t"<<split_attribute<<"\n";
			vector<pair<vector<string>,string> > process_data_left=refine_data_cont(input_data,split_attribute,start,mid);
			//display_refine_data(process_data_left);
			if(process_data_left.empty()==false)	
			node->value[0].first=form_decision_tree(process_data_left,attributes,target_values,left_leaf);
			else
			{
				// get majority value
			//	cout<<"Empty value \n";
				
				Tries *end_node = new Tries(vector<string>(),get_majority_target(input_data));
				node->value[0].first=end_node;
				node->is_end=true;
				leaf++;
			}

			//cout<<"===========RIGHT======================== "<<mid<<"\t"<<split_attribute<<"\n";
			vector<pair<vector<string>,string> > process_data_right=refine_data_cont(input_data,split_attribute,mid,end);	
			//display_refine_data(process_data_right);
			if(process_data_right.empty()==false)
			node->value[1].first=form_decision_tree(process_data_right,attributes,target_values,right_leaf);
			else
			{
				// get majority value
			//	cout<<"Empty value \n";

				Tries *end_node = new Tries(vector<string>(),get_majority_target(input_data));
				node->value[1].first=end_node;
				node->is_end=true;
				leaf++;
			}
			
			//cout<<min_value<<"\t"<<max_value<<"\n";
			node->num_leaves=leaf+left_leaf+right_leaf;
			leaf=node->num_leaves;
			return node;
		}
		else
		{
			
			Tries *node = new Tries(attributes[split_attribute]->attribute_values,split_attribute);
			node->class_pair=get_class_pair(input_data,node->total_v);
			attributes.erase(split_attribute);
			for(int i=0;i<node->value.size();i++)
			{
				int al_leaf=0;
				vector<pair<vector<string>,string> > process_data=refine_data(input_data,split_attribute,node->value[i].second);
				node->value[i].first=form_decision_tree(process_data,attributes,target_values,al_leaf);
				node->num_leaves+=al_leaf;
			}

			node->num_leaves+=leaf;
			leaf=node->num_leaves;
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
		if(_att[get_attribute_colm].empty()==false)
		{
			if(stof(_att[get_attribute_colm])>start && stof(_att[get_attribute_colm])<=end)
			{
				process_data.push_back(input_data[i]);
			}
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

	int get_attribute_colm=-1;
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

