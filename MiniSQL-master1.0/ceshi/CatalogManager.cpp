#include "CatalogManager.h"
#include "Declaration.h"

bool CatalogManager::readCatalog(string a_name) {
	//�򿪶�Ӧ���ݿ��.catalog�ļ�����ȡ��Ϣ 
	dbName = a_name;
	string filename = a_name + ".catalog";
	fstream fin(filename.c_str(), ios::in); 
	if (!fin) { //û�и����ݿ� 
		cout << "Database " << a_name << " does not Exist!" << endl;
		return false;
	}
	//�и����ݿ�ʱ����������� 
	fin >> tableNum;
	
	//���������ݱ��������������������Ϣ 
	for (int i = 0; i < tableNum; i++) {
		TableInfo temp_table; //����һ��TableInfo�Ķ������ڴ洢����Ϣ 
		fin >> temp_table.name;
		fin >> temp_table.attrNum;
		fin >> temp_table.totalLength;
		
		//�������������Ե�������������������Ϣ 
		for (int j = 0; j < temp_table.attrNum; j++) {
			AttrInfo temp_attr;
			string a_type;
			fin >> temp_attr.name;
			fin >> a_type;
			if (a_type == "int")
				temp_attr.type = INT;
			else if (a_type == "float")
				temp_attr.type = FLOAT;
			else
				temp_attr.type = CHAR;
			//���Եĸ�����Ϣ 
			fin >> temp_attr.length;
			fin >> temp_attr.isPrimeryKey;
			fin >> temp_attr.isUnique;
			fin >> temp_attr.indexName;
			if (temp_attr.indexName != "NULL")
				indexs.push_back(
						Index(temp_attr.indexName, temp_table.name,
								temp_attr.name));
			temp_table.attrs.push_back(temp_attr);
		}
		tables.push_back(temp_table);
	}
	indexNum = indexs.size();  //��ȡ������Ϣ 
	fin.close(); //�رն�ȡ 
	return true;
}

bool CatalogManager::writeCatalog() {
	string filename = dbName + ".catalog"; //�洢���ݿ��ļ������� 
	fstream fout(filename.c_str(), ios::out);
	if (!fout) { //�����ݿⲻ���� 
		cout << "Database " << dbName << " does not Exist!" << endl;
		return false;
	}
	fout << tableNum << endl; //д��������
	//���������ݱ�����������ļ������д������Ϣ 
	for (int i = 0; i < tableNum; i++) {
		fout << tables[i].name << " ";
		fout << tables[i].attrNum << " ";
		fout << tables[i].totalLength << endl;
		
		//�������������Ե����������ļ������д�����Ե���Ϣ
		for (int j = 0; j < tables[i].attrNum; j++) {
			fout << tables[i].attrs[j].name << " ";
			string a_type;
			//д�����Ե����ͣ���int��float��char(length) 
			if (tables[i].attrs[j].type == INT)
				a_type.append("int");
			else if (tables[i].attrs[j].type == FLOAT)
				a_type.append("float");
			else {
				fout << "char(";
				fout << tables[i].attrs[j].length;
				a_type.append(")");
			}
			fout << a_type << " ";
			fout << tables[i].attrs[j].length << " ";
			fout << tables[i].attrs[j].isPrimeryKey << " ";
			fout << tables[i].attrs[j].isUnique << " ";
			fout << tables[i].attrs[j].indexName << endl;
		}
	}
	fout.close();
	return true;
}

bool CatalogManager::isDBinUse() {
	//�ж��Ƿ������ݿ����ڱ�ʹ�� 
	//ͨ��dbName�Ĵ���������ж� 
	if (dbName.empty()) {
		cout << "No database is being used!" << endl;
		return false;
	} else
		return true;
}

//�ж����ַ��� DB_NameΪ���Ƶ����ݿ��Ƿ���� 
bool CatalogManager::Exist_Database(string DB_Name) {
	//ͨ���򿪶�Ӧ���Ƶ��ļ����ж��Ƿ��Ѿ����� 
	string filename = DB_Name + ".catalog";
	fstream file(filename.c_str(), ios::in);
	if (!file)
		return false;
	file.close();
	return true;
}

//�ж����ַ��� Table_NameΪ���Ƶı��Ƿ���ڣ��������λ�÷���ָ��position�� 
bool CatalogManager::Exist_Table(string Table_Name, int &position) {
	//����鿴������� 
	for (int i = 0; i < tableNum; i++)
		if (tables[i].name == Table_Name) { //���ڵĻ��ѱ��λ�÷���positionָ���� 
			position = i;
			return true;
		}
	return false;
}

//�жϱ������ַ��� Attr_NameΪ���Ƶ������Ƿ���ڣ��������Ե�λ�÷���ָ��position�� 
bool CatalogManager::Exist_Attr(string Attr_Name, int &position, int &tableP) {
	//�ڵ� &tableP����������鿴�������� 
	for (int i = 0; i < tables[tableP].attrNum; i++)
		if (tables[tableP].attrs[i].name == Attr_Name) {
			position = i;
			return true;
		}
	return false;
}

//�жϱ������ַ��� Index_NameΪ���Ƶ������Ƿ���ڣ�����������λ�÷���ָ��position��
bool CatalogManager::Exist_Index(string Index_Name, int &position) {
	//����鿴index������ 
	for (int i = 0; i < indexNum; i++)
		if (indexs[i].name == Index_Name) {
			position = i;
			return true;
		}
	return false;
}

//�ж����Ե����������Ե������Ƿ�ƥ�� ������ֻ���int �� float �Ĳ�� 
bool CatalogManager::Check_Type(int type, int length, string &data, string attr){
	if (type == INT || type == FLOAT) {
		int countdot = 0;
		for (unsigned int j = 0; j < data.size(); j++) {
			if (data.at(j) == '.')
				countdot += 1;
			else if (data.at(j) < '0' || data.at(j) > '9') {
				cout << "The data typed does not match attribute" << attr << "!"
						<< endl;
				return false;
			}
		}
		if (countdot > 1 || (type == INT && (countdot > 0 || data.size() > 4))
				|| data.size() > 10) {
			cout << "The data typed does not match attribute" << attr << "!"
					<< endl;
			return false;
		}
	}
	//string���Ͳ��������� 
	string temp;
	for (unsigned int i = 0; i < length - data.size(); i++)
		temp += "0";
	data = temp + data;
	return true;
}

// ��ʼʹ�����ݿ� 
bool CatalogManager::Use_Database(string DB_Name) {
	//������ݿ�����Ϊ�գ������ݿ�δ��ʹ�ã���ʼʹ�����ݿ� 
	if (!dbName.empty()) writeCatalog(); 
	if (!Exist_Database(DB_Name)) { //������ݿ��Ƿ���� 
		cout << "Database " << DB_Name << " does not Exist!" << endl;
		return false;
	}
	if (dbName == DB_Name) { //���ݿ��Ѿ���ʹ�� 
		cout << "Database " << DB_Name << " is already in use!" << endl;
		return true;
	}
	if (readCatalog(DB_Name)) {
		cout << "Database " << DB_Name << " is now in use!" << endl;
		return true;
	}
	//�����������������������򱨴� 
	cout << "Unknow Error!" << endl;

	return false;
}

//�������ݿ��ʹ�� 
bool CatalogManager::Exit_Database() {
	if (isDBinUse()) {
		writeCatalog();
		dbName = "";
		cout << "Database exited successfully" << endl;
		return true;
	}
	return false;
}

//�����µ����ݿ� 
void CatalogManager::Create_Database(string DB_Name) {
	if (Exist_Database(DB_Name)) { //���ݿ��Ѿ����� 
		cout << "Database " << DB_Name << " has already existed!" << endl;
		return;
	}
	//���ݿⲻ���ڣ���ʼ�����µ����ݿ��ļ� 
	string filename = DB_Name + ".catalog";
	ofstream fout(filename.c_str(), ios::out);
	fout << 0 << endl;
	fout.close();
	string instruction = "mkdir " + DB_Name;
	system(instruction.c_str());
	cout << "Database " << DB_Name << " created successfully" << endl;
	return;
}

//������ 
void CatalogManager::Create_Table(string Table_Name, vector<AttrInfo> newAttrs,
		vector<string> primaryKeys) {
	//���ȣ��ж����ݿ�������ڱ�ʹ�� 
	if (!isDBinUse())
		return;
	//���������жϱ��Ƿ��Ѿ����� 
	string filename = dbName + "_" + Table_Name + ".table";
	string path = dbName + "//" + filename;
	fstream file;
	file.open(path.c_str(), ios::in);
	if (file) {
		file.close();
		cout << "Table " << Table_Name << " has already existed!" << endl;
		return;
	}
	//�����ڣ���ʼ�����µı� 
	file.open(path.c_str(), ios::out);
	file.seekg(4096 * 4096);
	file << "END";
	file.close();
	TableInfo table = TableInfo(Table_Name, newAttrs);
	tables.push_back(table);
	tableNum += 1;
	for (unsigned int i = 0; i < primaryKeys.size(); i++) {
		int tableP, attrP;
		//string indexName = Table_Name + "_" + primaryKeys[i];
		Create_Index(primaryKeys[i], Table_Name, primaryKeys[i], tableP, attrP);
	}
	cout << "Table " << Table_Name << " created successfully" << endl;
	writeCatalog();
	return;
}

//�������� 
void CatalogManager::Create_Index(string Index_Name, string Table_Name,
		string Attr_Name, int &tableP, int &attrP) {
	//���ȣ��ж����ݿ�������ڱ�ʹ��
	if (!isDBinUse())
		return;
	int indexP;
	//���������жϱ��Ƿ��Ѿ����� 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return;
	}
	//���ж������Ƿ���� 
	if (!Exist_Attr(Attr_Name, attrP, tableP)) {
		cout << "Attribute " << Attr_Name << " does not exist!" << endl;
		return;
	}
	//�ж������Ƿ�unique 
	if (!tables[tableP].attrs[attrP].isUnique) {
		cout << "Attribute " << Attr_Name
				<< " is not unique, index can not be create!" << endl;
		return;
	}
	//�ж������Ƿ���� 
	if (Exist_Index(Index_Name, indexP)) {
		cout << "Index " << Index_Name << " has already existed!" << endl;
		return;
	}
	//�ж������Ƿ���� 
	if (tables[tableP].attrs[attrP].indexName != "NULL") {
		cout << "Attribute " << Attr_Name << " has already had index!" << endl;
		return;
	}
	tables[tableP].attrs[attrP].setIndex(Index_Name);
	string filename = dbName + "_" + tables[tableP].name + "_" + Index_Name
			+ ".index";
	string path = dbName + "//" + filename;
	fstream file;
	file.open(path.c_str(), ios::out);
	file.seekg(4096 * 4096);
	file << "END";
	file.close();
	Index index = Index(Index_Name, Table_Name, Attr_Name);
	indexs.push_back(index);
	indexNum += 1;
	cout << "Index " << Index_Name << " created successfully" << endl;
	writeCatalog();
	return;
}

//ɾ�����ݿ� 
void CatalogManager::Drop_Database(string DB_Name) {
	//�ж����ݿ��Ƿ���� 
	if (!Exist_Database(DB_Name)) {
		cout << "Database " << DB_Name << " does not Exist!" << endl;
		return;
	}
	//���ݿ���ڣ������ݿ��Ӧ���ļ� 
	string filename = DB_Name + ".catalog";
	remove(filename.c_str());
	string instruction = "rm -rf " + DB_Name;
	system(instruction.c_str());
	cout << "Database " << DB_Name << " dropped successfully" << endl;
	return;
}

//ɾ���� 
void CatalogManager::Drop_Table(string Table_Name) {
	//���ݿ�������ڱ�ʹ�� 
	if (!isDBinUse())
		return;
	int tableP;
	//���Ƿ���� 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return;
	}
	//�ȼ��ÿ�������Ƿ������� 
	for (int attrP = 0; attrP < tables[tableP].attrNum; attrP++)
		if (tables[tableP].attrs[attrP].indexName != "NULL")
			Drop_Index(tables[tableP].attrs[attrP].indexName);
	string filename = dbName + "_" + Table_Name + ".table";
	string path = dbName + "//" + filename;
	remove(path.c_str());
	tables.erase(tables.begin() + tableP);
	tableNum -= 1;
	cout << "Table " << Table_Name << " dropped successfully" << endl;
	writeCatalog();
	return;
}

//ɾ������ 
void CatalogManager::Drop_Index(string Index_Name) {
	//���ݿ��Ƿ����ڱ�ʹ�� 
	if (!isDBinUse())
		return;
	int tableP, attrP, indexP;
	//�����Ƿ���� 
	if (!Exist_Index(Index_Name, indexP)) {
		cout << "Index " << Index_Name << " does not exist!" << endl;
		return;
	}
	//���Ƿ���� 
	Exist_Table(indexs[indexP].tableName, tableP);
	//�����Ƿ���� 
	Exist_Attr(indexs[indexP].attrName, attrP, tableP);
	//��������Ϊ�գ�����д�������ļ��� 
	tables[tableP].attrs[attrP].indexName = "NULL";
	string filename = dbName + "_" + tables[tableP].name + "_" + Index_Name
			+ ".index";
	string path = dbName + "//" + filename;
	remove(path.c_str());
	indexs.erase(indexs.begin() + indexP);
	indexNum -= 1;
	cout << "Index " << Index_Name << " dropped successfully" << endl;
	writeCatalog();
	return;
}

//�����������Ĵ��� 
bool CatalogManager::Select(string Table_Name, vector<string> Attrs,
		vector<Condition> &Condition, vector<int> &IndexP, int &tableP) {
	//���ݿ��ʹ����� 
	if (!isDBinUse())
		return false;
	int attrP, indexP;
	//���Ƿ���� 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return false;
	}
	//�����Ƿ�Ϊ�� 
	if (!Attrs.empty()) {
		for (unsigned int i = 0; i < Attrs.size(); i++) {
			if (!Exist_Attr(Attrs[i], attrP, tableP)) {
				cout << "Attribute " << Attrs[i] << " does not exist!" << endl;
				return false;
			}
		}
	}
	//�����Ƿ�Ϊ�� 
	if (!Condition.empty()) {
		for (unsigned int i = 0; i < Condition.size(); i++) {
			//�������е������Ƿ���� 
			if (!Exist_Attr(Condition[i].attr, attrP, tableP)) {
				cout << "Attribute " << Condition[i].attr << " does not exist!"
						<< endl;
				return false;
			}
			//�ж�����������ֵ�Ƿ���϶�Ӧ������ 
			if (!Check_Type(tables[tableP].attrs[attrP].type,
					tables[tableP].attrs[attrP].length, Condition[i].data,
					Condition[i].attr))
				return false;
			//�鿴�Ƿ������� 
			if (Exist_Index(tables[tableP].attrs[attrP].indexName, indexP))
				IndexP.push_back(indexP);
		}
	}
	return true;
}

//���ڲ�������ĺ��� 
bool CatalogManager::Insert(string Table_Name, int &tableP,
		vector<string> &Data) {
	//���ݿ��ʹ����� 
	if (!isDBinUse())
		return false;
	//���Ƿ���� 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return false;
	}
	//�ж���������Ե������������������Ƿ�ƥ�� 
	if (tables[tableP].attrNum != Data.size()) {
		cout << tables[tableP].attrNum << "       " << Data.size() << endl;
		cout << "The amount of data inputted does not match!" << endl;
		return false;
	}
	//��ÿ�����Զ�ȥ���һ�����������Ƿ�ƥ�� 
	for (int i = 0; i < tables[tableP].attrNum; i++)
		if (!Check_Type(tables[tableP].attrs[i].type,
				tables[tableP].attrs[i].length, Data[i],
				tables[tableP].attrs[i].name))
			return false;
	return true;
}

//����ɾ�������ĺ��� 
bool CatalogManager::Delete(string Table_Name, vector<Condition> &Condition,
		vector<int> &IndexP, int &tableP) {
	//���ݿ��ʹ����� 
	if (!isDBinUse())
		return false;
	int attrP, indexP;
    //���Ƿ���� 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return false;
	}
	 
	if (!Condition.empty()) {//������Ϊ�� 
		for (unsigned int i = 0; i < Condition.size(); i++) {//����ÿ������ 
			//�ж�����������Ƿ���� 
			if (!Exist_Attr(Condition[i].attr, attrP, tableP)) {
				cout << "Attribute " << Condition[i].attr << " does not exist!"
						<< endl;
				return false;
			}
			//������Ե������Ƿ�ƥ�� 
			if (!Check_Type(tables[tableP].attrs[attrP].type,
					tables[tableP].attrs[attrP].length, Condition[i].data,
					Condition[i].attr))
				return false;
			//�ж��Ƿ�������� 
			if (Exist_Index(tables[tableP].attrs[attrP].indexName, indexP))
				IndexP.push_back(indexP);
		}
	}
	return true;
}

//����ɾ��������ĺ��� 
bool CatalogManager::Delete_Table(string Table_Name) {
	//���ݿ��ʹ����� 
	if (!isDBinUse())
		return false;
	int tableP, attrP;
	//���Ƿ���� 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return false;
	}
	//�жϱ��������Ƿ���������û������ֱ��ɾ����������ɾ���� 
	for (attrP = 0; attrP < tables[tableP].attrNum; attrP++)
		if (tables[tableP].attrs[attrP].indexName != "NULL") {
			int temp_indexP;
			//�ж��Ƿ�����������ҵ�λ�� 
			Exist_Index(tables[tableP].attrs[attrP].indexName, temp_indexP);
			string filename = dbName + "_" + tables[tableP].name + "_"
					+ tables[tableP].attrs[attrP].indexName + ".index";
			string path = dbName + "//" + filename;
			fstream file;
			file.open(path.c_str(), ios::out);
			file.seekg(4096 * 4096);
			file << "END";
			file.close();
		}
	//�ҵ���Ӧ�ı�ɾ�� 
	string filename = dbName + "_" + Table_Name + ".table";
	string path = dbName + "//" + filename;
	fstream file;
	file.open(path.c_str(), ios::out);
	file.seekg(4096 * 4096);
	file << "END";
	cout << "Table " << Table_Name << " deleted successfully" << endl;
	writeCatalog();
	return true;
}
