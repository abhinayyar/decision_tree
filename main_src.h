/* header file for main code */

#ifndef __MAIN_SRC_H__
#define __MAIN_SRC_H__

#include<iostream>
#include<vector>
#include "Tree/Tries_ds.h"
#include<unordered_map>
#include<utility>
#include<unordered_set>
#include<algorithm>
#include<cmath>
#include<set>

using namespace std;
/* function to determine all positive/negative targets */
bool is_single_target(vector<pair<vector<string>,string> >& input_data,string& target_value);
bool is_empty_attribute(vector<pair<vector<string>,string> >& input_data,string& target_value,unordered_map<string,Attribute_feature*>& attributes);
Tries* form_decision_tree(vector<pair<vector<string>,string> >& input_data,unordered_map<string,Attribute_feature*> attributes,vector<string>& target_values);
string get_majority_target(vector<pair<vector<string>,string> >& input_data);
string get_split_attribute_ig(unordered_map<string,Attribute_feature*>& attributes,vector<pair<vector<string>,string> >& input_data,vector<string>& target_values);
vector<pair<vector<string>,string> > refine_data(vector<pair<vector<string>,string> >& input_data,string split_attribute,string att_value);
int get_attribute_co(vector<string> lc_att,string split_attribute);
float get_entropy_overall(vector<pair<vector<string>,string> >& input_data);
void display_att_listing(unordered_map<string,Attribute_feature*> attributes);
void display_stack_contents(vector<string> stack);
void display_tree(Tries *root,vector<string> stack,int i,int j);
void get_mid_value(vector<pair<vector<string>,string> > input_data,string split_attribute,float& start,float& end);
vector<pair<vector<string>,string> > refine_data_cont(vector<pair<vector<string>,string> >& input_data,string split_attribute,float start,float end);
int get_value_index(Tries *root,string value);
int get_cont_value_index(Tries *root,string value);
int check_each_data(Tries *root,pair<vector<string>,string> t_data,vector<string> a_list,unordered_map<string,Attribute_feature*> attributes);
int test_decision_tree(Tries *root,vector<pair<vector<string>,string> > test_data,vector<string> a_list,unordered_map<string,Attribute_feature*> attributes);
void display_refine_data(vector<pair<vector<string>,string> > input_data);

void me_prune_tree(Tries *root);
float check_each_node(Tries *root,float prev_total);

#endif
