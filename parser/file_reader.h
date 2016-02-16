/* Header file for file parser */

#ifndef __FILE_READER_H__
#define __FILE_READER_H__

#include "../main_src.h"

using namespace std;

bool read_control_file(string file_name,vector<string>& a_list,vector<pair<string,bool> >& is_cont,int& data_size);
void read_data_file(string file_name,vector<pair<vector<string>,string> >& input_data,bool is_target_front,unordered_map<string,Attribute_feature*>& attributes,vector<string>& a_list,vector<string>& target_values,vector<pair<vector<string>,string> >& test_data,int data_size);
void fill_cont_values(vector<pair<string,bool> >& is_cont,unordered_map<string,Attribute_feature*>& attributes);
void pre_process_data(vector<pair<vector<string>,string> > input_data);
#endif
