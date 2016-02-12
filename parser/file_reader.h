/* Header file for file parser */

#ifndef __FILE_READER_H__
#define __FILE_READER_H__

#include "../main_src.h"

using namespace std;

bool read_control_file(string file_name,unordered_map<string,Attribute_feature*>& attributes);
void read_data_file(string file_name,vector<pair<vector<string>,string> >& input_data,bool is_target_front);

#endif
