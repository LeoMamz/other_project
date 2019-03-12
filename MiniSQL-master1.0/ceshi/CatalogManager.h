/*CatalogManager.h
 * ������
 * �汾��2018-6-3.9.1 
 *
 * ���ⲿ�ֲ�����ȥ�������������˵��κκ���
 * ����api�л��кܶಿ����Ҫ�ҵĺ���
 * �Ұ����ʼ��Ҫ��д������ָ���ĺ����ӿ�
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

class CatalogManager // CatalogManager�� 
{
private:
	string dbName; // �������� 
	vector<TableInfo> tables; //�� 
	int tableNum; //������ 
	vector<Index> indexs; //���� 
	int indexNum; //�������� 
public:
	CatalogManager(){}; // CatalogManager���͵Ķ���
	CatalogManager(string a_name){readCatalog(a_name);};
	~CatalogManager(){if(dbName != "")writeCatalog();};
	string getdbName(){ //�������ݿ������ 
		return dbName;
	}
	int gettableNum(){ //���ر������ 
		return tableNum;
	}
	int getindexNum(){ //�������������� 
		return indexNum;
	}
	
	/*�����������������������declartion.h�����н���
	�õ�Tableinfo��Index������ */ 
	TableInfo getTable(int tableP){ //��ȡ��ľ�����Ϣ 
		if(tableP >= tableNum)
		{
			cout << "Out of range of tables!" << endl;
			return TableInfo();
		}
		return tables[tableP];
	}
	Index getIndex(int indexP){ // ��ȡ�����ľ�����Ϣ
		if(indexP >= indexNum)
		{
			cout << "Out of range of indexs1" << endl;
			return Index();
		}
		return indexs[indexP];
	}
	
	//�򿪶�Ӧ���ݿ��.catalog�ļ�����ȡ��Ϣ 
	bool readCatalog(string a_name);
	//�򿪶�Ӧ���ݿ��.catalog�ļ���д�����ݿ����Ϣ 
	bool writeCatalog();
	//�ж��Ƿ������ݿ����ڱ�ʹ�� 
	bool isDBinUse();
	//�ж����ַ��� DB_NameΪ���Ƶ����ݿ��Ƿ���� 
	bool Exist_Database(string DB_Name);
	//�ж����ַ��� Table_NameΪ���Ƶı��Ƿ���ڣ��������λ�÷���ָ��position�� 
	bool Exist_Table(string Table_Name, int &position);
	//�жϱ������ַ��� Attr_NameΪ���Ƶ������Ƿ���ڣ��������Ե�λ�÷���ָ��position�� 
	bool Exist_Attr(string Attr_Name, int &position, int &tableP);
	//�жϱ������ַ��� Index_NameΪ���Ƶ������Ƿ���ڣ�����������λ�÷���ָ��position��
	bool Exist_Index(string Index_Name, int &position);
	//�ж����Ե����������Ե������Ƿ�ƥ�䣨����ֻ���int �� float �Ĳ�� 
	bool Check_Type(int type, int length, string &data, string attr);

	// ������ݿ��ʹ���������ʼʹ�����ݿ�
	bool Use_Database(string DB_Name);
	//�������ݿ��ʹ��
	bool Exit_Database();
	//�����µ����ݿ� 
	void Create_Database(string DB_Name);
	//������
	void Create_Table(string Table_Name, vector<AttrInfo> newAttrs, vector<string> primaryKeys);
	//��������
	void Create_Index(string Index_Name, string Table_Name, string Attr_Name, int &tableP, int &attrP);
	//ɾ�����ݿ�
	void Drop_Database(string DB_Name);
	//ɾ����
	void Drop_Table(string Table_Name);
	//ɾ������
	void Drop_Index(string Index_Name);
	//�����������Ĵ���
	bool Select(string Table_Name, vector<string> Attrs, vector<Condition> &Condition, vector<int> &IndexP, int &tableP);
	//���ڲ������Ĵ���
	bool Insert(string Table_Name, int &tableP, vector<string> &Data);
	//����ɾ�������ĺ���
	bool Delete(string Table_Name,vector<Condition> &Condition, vector<int> &IndexP, int &tableP);
	//����ɾ��������ĺ���
	bool Delete_Table(string Table_Name);
};
#endif /* CATALOGMANAGER_H_ */
