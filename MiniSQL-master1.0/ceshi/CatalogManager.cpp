#include "CatalogManager.h"
#include "Declaration.h"

bool CatalogManager::readCatalog(string a_name) {
	//打开对应数据库的.catalog文件，读取信息 
	dbName = a_name;
	string filename = a_name + ".catalog";
	fstream fin(filename.c_str(), ios::in); 
	if (!fin) { //没有该数据库 
		cout << "Database " << a_name << " does not Exist!" << endl;
		return false;
	}
	//有该数据库时读出表的数量 
	fin >> tableNum;
	
	//接下来根据表的数量，逐个读出表的信息 
	for (int i = 0; i < tableNum; i++) {
		TableInfo temp_table; //定义一个TableInfo的对象，用于存储表信息 
		fin >> temp_table.name;
		fin >> temp_table.attrNum;
		fin >> temp_table.totalLength;
		
		//接下来根据属性的数量，逐个读出表的信息 
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
			//属性的各种信息 
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
	indexNum = indexs.size();  //获取索引信息 
	fin.close(); //关闭读取 
	return true;
}

bool CatalogManager::writeCatalog() {
	string filename = dbName + ".catalog"; //存储数据库文件的名称 
	fstream fout(filename.c_str(), ios::out);
	if (!fout) { //该数据库不存在 
		cout << "Database " << dbName << " does not Exist!" << endl;
		return false;
	}
	fout << tableNum << endl; //写入表的数量
	//接下来根据表的数量，在文件中逐个写入表的信息 
	for (int i = 0; i < tableNum; i++) {
		fout << tables[i].name << " ";
		fout << tables[i].attrNum << " ";
		fout << tables[i].totalLength << endl;
		
		//接下来根据属性的数量，在文件中逐个写入属性的信息
		for (int j = 0; j < tables[i].attrNum; j++) {
			fout << tables[i].attrs[j].name << " ";
			string a_type;
			//写入属性的类型：有int、float和char(length) 
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
	//判断是否有数据库正在被使用 
	//通过dbName的存在与否来判断 
	if (dbName.empty()) {
		cout << "No database is being used!" << endl;
		return false;
	} else
		return true;
}

//判断以字符串 DB_Name为名称的数据库是否存在 
bool CatalogManager::Exist_Database(string DB_Name) {
	//通过打开对应名称的文件来判断是否已经存在 
	string filename = DB_Name + ".catalog";
	fstream file(filename.c_str(), ios::in);
	if (!file)
		return false;
	file.close();
	return true;
}

//判断以字符串 Table_Name为名称的表是否存在，并将表的位置放在指针position中 
bool CatalogManager::Exist_Table(string Table_Name, int &position) {
	//逐个查看表的名字 
	for (int i = 0; i < tableNum; i++)
		if (tables[i].name == Table_Name) { //存在的话把表的位置放在position指针中 
			position = i;
			return true;
		}
	return false;
}

//判断表中以字符串 Attr_Name为名称的属性是否存在，并将属性的位置放在指针position中 
bool CatalogManager::Exist_Attr(string Attr_Name, int &position, int &tableP) {
	//在第 &tableP个表中逐个查看属性名称 
	for (int i = 0; i < tables[tableP].attrNum; i++)
		if (tables[tableP].attrs[i].name == Attr_Name) {
			position = i;
			return true;
		}
	return false;
}

//判断表中以字符串 Index_Name为名称的索引是否存在，并将索引的位置放在指针position中
bool CatalogManager::Exist_Index(string Index_Name, int &position) {
	//逐个查看index的名称 
	for (int i = 0; i < indexNum; i++)
		if (indexs[i].name == Index_Name) {
			position = i;
			return true;
		}
	return false;
}

//判断属性的类型与属性的数据是否匹配 （这里只检测int 和 float 的差别） 
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
	//string类型不会有问题 
	string temp;
	for (unsigned int i = 0; i < length - data.size(); i++)
		temp += "0";
	data = temp + data;
	return true;
}

// 开始使用数据库 
bool CatalogManager::Use_Database(string DB_Name) {
	//如果数据库名称为空，则数据库未被使用；开始使用数据库 
	if (!dbName.empty()) writeCatalog(); 
	if (!Exist_Database(DB_Name)) { //检测数据库是否存在 
		cout << "Database " << DB_Name << " does not Exist!" << endl;
		return false;
	}
	if (dbName == DB_Name) { //数据库已经被使用 
		cout << "Database " << DB_Name << " is already in use!" << endl;
		return true;
	}
	if (readCatalog(DB_Name)) {
		cout << "Database " << DB_Name << " is now in use!" << endl;
		return true;
	}
	//如果发生以上情况外的情况，则报错 
	cout << "Unknow Error!" << endl;

	return false;
}

//结束数据库的使用 
bool CatalogManager::Exit_Database() {
	if (isDBinUse()) {
		writeCatalog();
		dbName = "";
		cout << "Database exited successfully" << endl;
		return true;
	}
	return false;
}

//创建新的数据库 
void CatalogManager::Create_Database(string DB_Name) {
	if (Exist_Database(DB_Name)) { //数据库已经存在 
		cout << "Database " << DB_Name << " has already existed!" << endl;
		return;
	}
	//数据库不存在，开始创建新的数据库文件 
	string filename = DB_Name + ".catalog";
	ofstream fout(filename.c_str(), ios::out);
	fout << 0 << endl;
	fout.close();
	string instruction = "mkdir " + DB_Name;
	system(instruction.c_str());
	cout << "Database " << DB_Name << " created successfully" << endl;
	return;
}

//创建表 
void CatalogManager::Create_Table(string Table_Name, vector<AttrInfo> newAttrs,
		vector<string> primaryKeys) {
	//首先，判断数据库书否正在被使用 
	if (!isDBinUse())
		return;
	//接下来，判断表是否已经存在 
	string filename = dbName + "_" + Table_Name + ".table";
	string path = dbName + "//" + filename;
	fstream file;
	file.open(path.c_str(), ios::in);
	if (file) {
		file.close();
		cout << "Table " << Table_Name << " has already existed!" << endl;
		return;
	}
	//表不存在，开始创建新的表 
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

//创建索引 
void CatalogManager::Create_Index(string Index_Name, string Table_Name,
		string Attr_Name, int &tableP, int &attrP) {
	//首先，判断数据库书否正在被使用
	if (!isDBinUse())
		return;
	int indexP;
	//接下来，判断表是否已经存在 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return;
	}
	//再判断属性是否存在 
	if (!Exist_Attr(Attr_Name, attrP, tableP)) {
		cout << "Attribute " << Attr_Name << " does not exist!" << endl;
		return;
	}
	//判断属性是否unique 
	if (!tables[tableP].attrs[attrP].isUnique) {
		cout << "Attribute " << Attr_Name
				<< " is not unique, index can not be create!" << endl;
		return;
	}
	//判断索引是否存在 
	if (Exist_Index(Index_Name, indexP)) {
		cout << "Index " << Index_Name << " has already existed!" << endl;
		return;
	}
	//判断索引是否存在 
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

//删除数据库 
void CatalogManager::Drop_Database(string DB_Name) {
	//判断数据库是否存在 
	if (!Exist_Database(DB_Name)) {
		cout << "Database " << DB_Name << " does not Exist!" << endl;
		return;
	}
	//数据库存在，打开数据库对应的文件 
	string filename = DB_Name + ".catalog";
	remove(filename.c_str());
	string instruction = "rm -rf " + DB_Name;
	system(instruction.c_str());
	cout << "Database " << DB_Name << " dropped successfully" << endl;
	return;
}

//删除表 
void CatalogManager::Drop_Table(string Table_Name) {
	//数据库书否正在被使用 
	if (!isDBinUse())
		return;
	int tableP;
	//表是否存在 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return;
	}
	//先检测每个属性是否有索引 
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

//删除索引 
void CatalogManager::Drop_Index(string Index_Name) {
	//数据库是否正在被使用 
	if (!isDBinUse())
		return;
	int tableP, attrP, indexP;
	//索引是否存在 
	if (!Exist_Index(Index_Name, indexP)) {
		cout << "Index " << Index_Name << " does not exist!" << endl;
		return;
	}
	//表是否存在 
	Exist_Table(indexs[indexP].tableName, tableP);
	//属性是否存在 
	Exist_Attr(indexs[indexP].attrName, attrP, tableP);
	//把索引设为空，重新写入索引文件中 
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

//关于搜索语句的处理 
bool CatalogManager::Select(string Table_Name, vector<string> Attrs,
		vector<Condition> &Condition, vector<int> &IndexP, int &tableP) {
	//数据库的使用情况 
	if (!isDBinUse())
		return false;
	int attrP, indexP;
	//表是否存在 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return false;
	}
	//属性是否为空 
	if (!Attrs.empty()) {
		for (unsigned int i = 0; i < Attrs.size(); i++) {
			if (!Exist_Attr(Attrs[i], attrP, tableP)) {
				cout << "Attribute " << Attrs[i] << " does not exist!" << endl;
				return false;
			}
		}
	}
	//条件是否为空 
	if (!Condition.empty()) {
		for (unsigned int i = 0; i < Condition.size(); i++) {
			//条件当中的属性是否存在 
			if (!Exist_Attr(Condition[i].attr, attrP, tableP)) {
				cout << "Attribute " << Condition[i].attr << " does not exist!"
						<< endl;
				return false;
			}
			//判断条件中属性值是否符合对应的类型 
			if (!Check_Type(tables[tableP].attrs[attrP].type,
					tables[tableP].attrs[attrP].length, Condition[i].data,
					Condition[i].attr))
				return false;
			//查看是否有索引 
			if (Exist_Index(tables[tableP].attrs[attrP].indexName, indexP))
				IndexP.push_back(indexP);
		}
	}
	return true;
}

//关于插入操作的函数 
bool CatalogManager::Insert(string Table_Name, int &tableP,
		vector<string> &Data) {
	//数据库的使用情况 
	if (!isDBinUse())
		return false;
	//表是否存在 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return false;
	}
	//判断输入的属性的数量与表的属性数量是否匹配 
	if (tables[tableP].attrNum != Data.size()) {
		cout << tables[tableP].attrNum << "       " << Data.size() << endl;
		cout << "The amount of data inputted does not match!" << endl;
		return false;
	}
	//对每个属性都去检查一下它的类型是否匹配 
	for (int i = 0; i < tables[tableP].attrNum; i++)
		if (!Check_Type(tables[tableP].attrs[i].type,
				tables[tableP].attrs[i].length, Data[i],
				tables[tableP].attrs[i].name))
			return false;
	return true;
}

//关于删除操作的函数 
bool CatalogManager::Delete(string Table_Name, vector<Condition> &Condition,
		vector<int> &IndexP, int &tableP) {
	//数据库的使用情况 
	if (!isDBinUse())
		return false;
	int attrP, indexP;
    //表是否存在 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return false;
	}
	 
	if (!Condition.empty()) {//条件不为空 
		for (unsigned int i = 0; i < Condition.size(); i++) {//对于每个条件 
			//判断输入的属性是否存在 
			if (!Exist_Attr(Condition[i].attr, attrP, tableP)) {
				cout << "Attribute " << Condition[i].attr << " does not exist!"
						<< endl;
				return false;
			}
			//检测属性的类型是否匹配 
			if (!Check_Type(tables[tableP].attrs[attrP].type,
					tables[tableP].attrs[attrP].length, Condition[i].data,
					Condition[i].attr))
				return false;
			//判断是否存在索引 
			if (Exist_Index(tables[tableP].attrs[attrP].indexName, indexP))
				IndexP.push_back(indexP);
		}
	}
	return true;
}

//关于删除表操作的函数 
bool CatalogManager::Delete_Table(string Table_Name) {
	//数据库的使用情况 
	if (!isDBinUse())
		return false;
	int tableP, attrP;
	//表是否存在 
	if (!Exist_Table(Table_Name, tableP)) {
		cout << "Table " << Table_Name << " does not exist!" << endl;
		return false;
	}
	//判断表中属性是否有索引，没索引就直接删，有索引先删索引 
	for (attrP = 0; attrP < tables[tableP].attrNum; attrP++)
		if (tables[tableP].attrs[attrP].indexName != "NULL") {
			int temp_indexP;
			//判断是否存在索引并找到位置 
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
	//找到对应的表，删除 
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
