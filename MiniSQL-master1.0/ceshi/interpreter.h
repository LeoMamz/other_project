#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <string>
#include <iostream>
#include "Declaration.h"
using namespace std;


class Interpreter
{
private:
	string SQL;
	int start;
	string sql;
public:
	Interpreter();
	string read_input();
	string create_clause(string, int);
	string create_database(string, int);
	string create_table(string, int);
	string get_attribute(string, string);
	string create_index(string, int);
	string create_index_on(string, int, string);
	string drop_clause(string, int);
	string drop_database(string, int);
	string drop_table(string, int); 
	string drop_index(string, int);

	string select_clause(string, int);
	string select_from_where(string, int, string);
	string insert_clause(string, int);
	string insert_into_values(string, int, string);
	string delete_clause(string, int);
	string delete_from_where(string, int, string);
	string execfile_clause(string, int);
	string quit_clause(string, int);
	string use_clause(string, int);
	string interpreter(string);
	void change(string&);
};

#endif /* INTERPRETER_H_ */
