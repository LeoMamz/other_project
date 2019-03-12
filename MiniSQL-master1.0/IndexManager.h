#ifndef INDEXMANAGER_H_
#define INDEXMANAGER_H_

#include "Declaration.h"
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
using namespace std;

int search_one(string database, string table_name, struct index_info & inform);

set<int> search_many(string database, string table_name, int type, struct index_info& inform);

void insert_one(string database, string table_name, struct index_info & inform);

void delete_one(string database, string table_name, struct index_info & inform);





int getLocation(index_info& inform, string info, int type);

void insertInLeaf(string database, string table_name, struct index_info & inform, int blockNum);

void insertInParent(string database, string table_name, struct index_info & inform, int N, string K, int N1);

int findFather(string database, string table_name, index_info inform, int n);

void deleteEntry(string database, string table_name, struct index_info & inform, int n, string K, int nod);

int getN(index_info& inform, bool isLeaf);

int getValueNum(string info);

int getLength(index_info inform);

string toLength(index_info inform);

int find_prev_leaf_sibling(string database, string table_name,struct index_info inform, int nodenum);

int find_next_leaf_sibling(string database, string table_name,struct index_info inform, int nodenum);

int find_left_child(string database, string table_name,struct index_info inform);

int find_right_child(string database, string table_name,struct index_info inform);

int get_new_freeblocknum(string database, string table_name,struct index_info& inform);

void deleteBlock(string database, string table_name, struct index_info& inform,int n);

//void constructList(string database, string table_name,struct index_info& inform);

void updateNode(blockInfo* b, string s);

string int_to_str(long value, int length);

int str_to_int(string str);

float str_to_float(string str);

int compareStr(string str1, string str2);

int compareInt(string int1, string int2);

int compareFloat(string float1, string float2);

#endif /* INDEXMANAGER_H_ */

