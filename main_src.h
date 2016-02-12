/* header file for main code */

#ifndef __MAIN_SRC_H__
#define __MAIN_SRC_H__

#include<iostream>
#include<vector>
#include "Tree/Tries_ds.h"
#include<unordered_map>
#include<utility>

using namespace std;

Tries *create_tree(unordered_map<string,Attribute_feature*>& attributes,vector<pair<vector<string>,string> >& input_data);
void insert_node(Tries *root,unordered_map<string,Attribute_feature*>& attributes,vector<pair<vector<string>,string> >& input_data);


string get_initial_split_attribute(unordered_map<string,Attribute_feature*>& attributes,vector<pair<vector<string>,string> >& input_data);

#endif
