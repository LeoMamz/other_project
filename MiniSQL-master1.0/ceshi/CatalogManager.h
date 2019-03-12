/*CatalogManager.h
 * 马铭泽
 * 版本：2018-6-3.9.1 
 *
 * 我这部分不存在去调用你们其他人的任何函数
 * 但是api中会有很多部分需要我的函数
 * 我按照最开始的要求写好了你指定的函数接口
 */

#ifndef CATALOGMANAGER_H_
#define CATALOGMANAGER_H_
#pragma once

#include "Declaration.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class CatalogManager // CatalogManager类 
{
private:
	string dbName; // 数据名字 
	vector<TableInfo> tables; //表 
	int tableNum; //表数量 
	vector<Index> indexs; //索引 
	int indexNum; //索引数量 
public:
	CatalogManager(){}; // CatalogManager类型的对象
	CatalogManager(string a_name){readCatalog(a_name);};
	~CatalogManager(){if(dbName != "")writeCatalog();};
	string getdbName(){ //返回数据库的名称 
		return dbName;
	}
	int gettableNum(){ //返回表的数量 
		return tableNum;
	}
	int getindexNum(){ //返回索引的数量 
		return indexNum;
	}
	
	/*接下来的两个函数与解释器declartion.h部分有交互
	用到Tableinfo和Index两个类 */ 
	TableInfo getTable(int tableP){ //获取表的具体信息 
		if(tableP >= tableNum)
		{
			cout << "Out of range of tables!" << endl;
			return TableInfo();
		}
		return tables[tableP];
	}
	Index getIndex(int indexP){ // 获取索引的具体信息
		if(indexP >= indexNum)
		{
			cout << "Out of range of indexs1" << endl;
			return Index();
		}
		return indexs[indexP];
	}
	
	//打开对应数据库的.catalog文件，读取信息 
	bool readCatalog(string a_name);
	//打开对应数据库的.catalog文件，写入数据库的信息 
	bool writeCatalog();
	//判断是否有数据库正在被使用 
	bool isDBinUse();
	//判断以字符串 DB_Name为名称的数据库是否存在 
	bool Exist_Database(string DB_Name);
	//判断以字符串 Table_Name为名称的表是否存在，并将表的位置放在指针position中 
	bool Exist_Table(string Table_Name, int &position);
	//判断表中以字符串 Attr_Name为名称的属性是否存在，并将属性的位置放在指针position中 
	bool Exist_Attr(string Attr_Name, int &position, int &tableP);
	//判断表中以字符串 Index_Name为名称的索引是否存在，并将索引的位置放在指针position中
	bool Exist_Index(string Index_Name, int &position);
	//判断属性的类型与属性的数据是否匹配（这里只检测int 和 float 的差别） 
	bool Check_Type(int type, int length, string &data, string attr);

	// 检测数据库的使用情况，开始使用数据库
	bool Use_Database(string DB_Name);
	//结束数据库的使用
	bool Exit_Database();
	//创建新的数据库 
	void Create_Database(string DB_Name);
	//创建表
	void Create_Table(string Table_Name, vector<AttrInfo> newAttrs, vector<string> primaryKeys);
	//创建索引
	void Create_Index(string Index_Name, string Table_Name, string Attr_Name, int &tableP, int &attrP);
	//删除数据库
	void Drop_Database(string DB_Name);
	//删除表
	void Drop_Table(string Table_Name);
	//删除索引
	void Drop_Index(string Index_Name);
	//关于搜索语句的处理
	bool Select(string Table_Name, vector<string> Attrs, vector<Condition> &Condition, vector<int> &IndexP, int &tableP);
	//关于插入语句的处理
	bool Insert(string Table_Name, int &tableP, vector<string> &Data);
	//关于删除操作的函数
	bool Delete(string Table_Name,vector<Condition> &Condition, vector<int> &IndexP, int &tableP);
	//关于删除表操作的函数
	bool Delete_Table(string Table_Name);
};
#endif /* CATALOGMANAGER_H_ */
