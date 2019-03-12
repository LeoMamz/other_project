
#include "interpreter.h"

Interpreter::Interpreter()
{
	SQL="0";
	sql="0";
	start=0;
}

string Interpreter::read_input()
{
	string SQL;
	string temp;
	string str;
	bool finish = false;
	int start=0,end;
	//while(!finish)
	//{
	getline(cin,str);
	if(str.at(str.length()-1)!=';')
	{
		cout<<"syntax error: missing ; !"<<endl;
		SQL="99";
	}
	else
		while(!finish)
		{
			while(str.at(start)==' ')
				start++;
			end=str.find(' ',start);
			temp.assign(SQL,start,end-start);
			start=end+1;
			if(temp.at(temp.length()-1)!=';')
				SQL=SQL+" "+temp;
			else
			{
				SQL.at(SQL.length()-1)=' ';
				SQL+=";";
				finish=true;
			}
		}


	for(int i=0;SQL[i]!='\0';i++)
	{
			if(SQL[i]>='A'&&SQL[i]<='Z')
			{
				SQL[i] += 32;
				continue;
			 }
	}
	cout << "命令行输入: " << SQL << endl;

	return SQL;
}



string Interpreter::create_database(string SQL,int start)
{
	string temp;
	int index,end;
	
	while(SQL.at(start)==' ')
		start++;
	index=start;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;
	
	if(temp.empty())
	{
		cout<<"error: database name has not been specified!"<<endl;
		SQL="99";
	}
	else
	{
		while(SQL.at(start)==' ')
			start++;
	
		if(SQL.at(start)!=';'||start!=SQL.length()-1)
		{
			cout<<"error12:"<<SQL.assign(SQL,index,SQL.length()-index-2)<<"---is not a valid database name!"<<endl;
			SQL="99";
		}
		
		else
			SQL="00"+temp;
	}
	return SQL;
}


string Interpreter::get_attribute(string temp,string sql)
{
	int start=0,end,index;
	string T,C;
	temp+=" ";
	end=temp.find(' ',start);
	T.assign(temp,start,end-start);
	start=end+1;
	sql+=T+" ";
	
	while(temp.at(start)==' ')
		start++;
	end=temp.find(' ',start);
	T.assign(temp,start,end-start);
	start=end+1;
	change(T);

	if(T.empty())
	{
		cout<<"error : error in create table statement!"<<endl;
		sql="99";
		return sql;
	}
	
	else if(T=="int")
		sql+="+";
	
	else if(T=="float")
		sql+="-";
	
	else
	{
		index=T.find('(');
		C.assign(T,0,index);
		index++;
		change(C);
		
		if(C.empty())
		{
			cout<<"error: "<<T<<"---is not a valid data type definition!"<<endl;
			sql="99";
			return sql;
		}
		
		else if(C=="char")
		{
			C.assign(T,index,T.length()-index-1);
			if(C.empty())
			{
				cout<<"error: the length of the data type char has not been specified!"<<endl;
				sql="99";
				return sql;
			}
			else
				sql+=C;
		}
		
		else
		{
			cout<<"error: "<<C<<"---is not a valid key word!"<<endl;
			sql="99";
			return sql;
		}
	}
	
	while(start<temp.length()&&temp.at(start)==' ')
		start++;
	if(start<temp.length())
	{
	
		end=temp.find(' ',start);
		T.assign(temp,start,end-start);
		change(T);
		if(T=="unique")
		{
			sql+=" 1,";
		}
		
		else
		{
			cout<<"error: "<<temp<<"---is not a valid key word!"<<endl;
			sql="99";
			return sql;
		}
	}

	else
		sql+=" 0,";
	return sql;
}

string Interpreter::create_table(string SQL,int start)
{
	string temp,sql,T;
	int index,end,length;
	while(SQL.at(start)==' ')
		start++;
	index=start;
	if((end=SQL.find('(',start))==-1)
	{
		cout<<"error: missing ( in the statement!"<<endl;
		SQL="99";
		return SQL;
	}
	temp.assign(SQL,start,end-start);
	start=end+1;
	if(!temp.empty())
	{
		while(SQL.at(start)==' ')
			start++;
		length=temp.length()-1;
		while(temp.at(length)==' ')
			length--;
		temp.assign(temp,0,length+1);
	}
	if(temp.empty())
	{
		cout<<"error: error in create table statement!"<<endl;
		SQL="99";
		return SQL;
	}
	
	else if(temp.find(' ')!=-1)
	{
		cout<<"error: "<<temp<<"---is not a valid table name!"<<endl;
		SQL="99";
		return SQL;
	}
	else
	{
		sql=temp+",";
		
		while((end=SQL.find(',',start))!=-1)
		{
			temp.assign(SQL,start,end-start);
			start=end+1;
			
			if(temp.empty())
			{
				cout<<"error: error in create table statement!"<<endl;
				SQL="99";
				return SQL;
			}
		
			else
			{
				sql=get_attribute(temp,sql);
				if(sql=="99")
					return sql;
			}
			while(SQL.at(start)==' ')
				start++;
		}
	
		temp.assign(SQL,start,SQL.length()-start-1);
		length=temp.length()-1;
		while(temp.at(length)!=')'&&length>=0)
			length--;
	
		if(length<1)
		{
			cout<<"error: error in create table statement!"<<endl;
			SQL="99";
			return SQL;
		}
	
		else
		{
			temp.assign(temp,0,length);
			end=SQL.find(' ',start);
			T.assign(SQL,start,end-start);
			start=end+1;
			change(T);
		
			if(T=="primary")
			{
	
				temp+=")";
				while(SQL.at(start)==' ')
					start++;
				end=SQL.find('(',start);
				T.assign(SQL,start,end-start);
				start=end+1;
				length=T.length()-1;
				while(T.at(length)==' ')
					length--;
				T.assign(T,0,length+1);
				change(T);
			
				if(T.empty())
				{
					cout<<"syntax error: syntax error in create table statement!"<<endl;
					cout<<"\t missing key word key!"<<endl;
					SQL="99";
					return SQL;
				}
		
				else if(T=="key")
				{
				
					while(SQL.at(start)==' ')
						start++;
					end=SQL.find(')',start);
					T.assign(SQL,start,end-start);
					length=T.length()-1;
					while(T.at(length)==' ')
						length--;
					T.assign(T,0,length+1);
				
					if(T.empty())
					{
						cout<<"error : missing primary key attribute name!"<<endl;
						SQL="99";
						return SQL;
					}
				
					else if(T.find(' ')!=-1)
					{
						cout<<"error : "<<T<<"---is not a valid primary key attribute name!"<<endl;
						SQL="99";
						return SQL;
					}
				
					else
					{
						sql+=T+" #,";
						SQL="01"+sql;
					}
				}
			
				else
				{
					cout<<"error : "<<T<<"---is not a valid key word!"<<endl;
					SQL="99";
					return SQL;
				}
			}
		
			else
			{
				sql=get_attribute(temp,sql);
				if(sql=="99")
				{
					SQL="99";
					return SQL;
				}
				else
					SQL="01"+sql;
			}
		}
	}
	return SQL;
}

string Interpreter::create_index_on(string SQL,int start,string sql)
{
	string temp;
	int end,length;
	while(SQL.at(start)==' ')
		start++;
	end=SQL.find('(',start);
	temp.assign(SQL,start,end-start);
	start=end+1;
	if(temp.empty())
	{
		cout<<"syntax error: syntax error for create index statement!"<<endl;
		cout<<"\t missing ( !"<<endl;
		SQL="99";
		return SQL;
	}
	else
	{
		length=temp.length()-1;
		while(temp.at(length)==' ')
			length--;
		temp.assign(temp,0,length+1);
	
		if(temp.find(' ')==-1)
		{
			sql+=" "+temp;
			
			while(SQL.at(start)=='('||SQL.at(start)==' ')
				start++;
			end=SQL.find(')',start);
			temp.assign(SQL,start,end-start);
			start=end+1;
		
			if(temp.empty())
			{
				cout<<"syntax error: syntax error for create index statement!"<<endl;
				cout<<"\t missing ) !"<<endl;
				SQL="99";
				return SQL;
			}
			else
			{
			
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
			
				if(temp.find(' ')==-1)
				{
					sql+=" "+temp;
					while(SQL.at(start)==' ')
						start++;
					if(SQL.at(start)!=';'||start!=SQL.length()-1)
					{
						cout<<"syntax error: syntax error for quit!"<<endl;
						SQL="99";
						return SQL;
					}
				
					else
						SQL="02"+sql;
				}
				
				else
				{
					cout<<"error:"<<" "<<temp<<"---is not a valid attribute name!"<<endl;
					SQL="99";
					return SQL;
				}
			}
		}
	
		else
		{
			cout<<"error:"<<" "<<temp<<"---is not a valid table name!"<<endl;
			SQL="99";
			return SQL;
		}
	}
	return SQL;
}


string Interpreter::create_index(string SQL,int start)
{
	string temp,sql;
	int end;
	
	while(SQL.at(start)==' ')
		start++;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;
	
	if(temp.empty())
	{
		cout<<"syntax error: syntax error for create index statement!"<<endl;
		SQL="99";
	}
	else
	{
		sql=temp;
	
		while(SQL.at(start)==' ')
			start++;
		end=SQL.find(' ',start);
		temp.assign(SQL,start,end-start);
		start=end+1;
		change(temp);
	
		if(temp.empty())
		{
			cout<<"syntax error: syntax error for create index statement!"<<endl;
			SQL="99";
		}
	
		else if(temp=="on")
			SQL=create_index_on(SQL,start,sql);
	
		else
		{
			cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
			SQL="99";
		}
	}
	return SQL;
}


string Interpreter::create_clause(string SQL,int start)
{
	string temp;
	int end;

	while(SQL.at(start)==' ')
		start++;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;
	change(temp);

	if(temp.empty())
	{
		cout<<"syntax error: syntax error for create statement!"<<endl;
		SQL="99";
	}

	else if(temp=="database")
		SQL=create_database(SQL,start);

	else if(temp=="table")
		SQL=create_table(SQL,start);

	else if(temp=="index")
		SQL=create_index(SQL,start);

	else
	{
		cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
		SQL="99";
	}

	return SQL;
}


string Interpreter::drop_database(string SQL,int start)
{
	string temp;
	int index,end;

	while(SQL.at(start)==' ')
		start++;
	index=start;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;

	if(temp.empty())
	{
		cout<<"error: database name has not been specified!"<<endl;
		SQL="99";
	}
	else
	{
		while(SQL.at(start)==' ')
			start++;

		if(SQL.at(start)!=';'||start!=SQL.length()-1)
		{
			cout<<"error:"<<SQL.assign(SQL,index,SQL.length()-index-2)<<"---is not a valid database name!"<<endl;
			SQL="99";
		}

		else
			SQL="10"+temp;
	}
	return SQL;
}



string Interpreter::drop_table(string SQL,int start)
{
	string temp;
	int index,end;

	while(SQL.at(start)==' ')
		start++;
	index=start;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;

	if(temp.empty())
	{
		cout<<"error: table name has not been specified!"<<endl;
		SQL="99";
	}
	else
	{
		while(SQL.at(start)==' ')
			start++;

		if(SQL.at(start)!=';'||start!=SQL.length()-1)
		{
			cout<<"error:"<<SQL.assign(SQL,index,SQL.length()-index-2)<<"---is not a valid table name!"<<endl;
			SQL="99";
		}

		else
			SQL="11"+temp;
	}
	return SQL;
}


string Interpreter::drop_index(string SQL,int start)
{
	string temp;
	int index,end;

	while(SQL.at(start)==' ')
		start++;
	index=start;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;

	if(temp.empty())
	{
		cout<<"error: index name has not been specified!"<<endl;
		SQL="99";
	}
	else
	{
		while(SQL.at(start)==' ')
			start++;

		if(SQL.at(start)!=';'||start!=SQL.length()-1)
		{
			cout<<"error:"<<SQL.assign(SQL,index,SQL.length()-index-2)<<"---is not a valid index name!"<<endl;
			SQL="99";
		}

		else
			SQL="12"+temp;
	}
	return SQL;
}

string Interpreter::drop_clause(string SQL,int start)
{
	string temp;
	int end;

	while(SQL.at(start)==' ')
		start++;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;
	change(temp);

	if(temp.empty())
	{
		cout<<"syntax error: syntax error for drop statement!"<<endl;
		SQL="99";
	}

	else if(temp=="database")
		SQL=drop_database(SQL,start);

	else if(temp=="table")
		SQL=drop_table(SQL,start);

	else if(temp=="index")
		SQL=drop_index(SQL,start);

	else
	{
		cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
		SQL="99";
	}

	return SQL;
}


string Interpreter::select_from_where(string SQL,int start,string sql)
{
	string temp,Temp;
	int end,length,end1,end2,end3,end4,k;
	bool finish=false;

	while(SQL.at(start)==' ')
		start++;
	end=SQL.find(';',start);
	temp.assign(SQL,start,end-start);
	//start=end+1;
	if(temp.empty())
	{
		cout<<"syntax error: syntax error for select statement!"<<endl;
		cout<<"\t missing ; !"<<endl;
		SQL="99";
		return SQL;
	}
	else
	{

		length=temp.length()-1;
		while(temp.at(length)==' ')
			length--;
		temp.assign(temp,0,length+1);

		if(temp.find(' ')==-1&&temp.length()>1)
		{
			//放弃了while(SQL.at(start)==' ')
				//start++;
			end1=SQL.find('<',start);
			end2=SQL.find('>',start);
			end3=SQL.find('=',start);
			end4=SQL.find('!',start);
			if(end1>0&&end2<0&&end3<0&&end4<0)
			{
				temp.assign(SQL,start,end1-start);
				start=end1+1;
				sql+=","+temp+" <";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2>0&&end3<0&&end4<0)
			{
				temp.assign(SQL,start,end2-start);
				start=end2+1;
				sql+=","+temp+" >";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2<0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start);
				start=end3+1;
				sql+=","+temp+" =";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1>0&&end2<0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start-1);
				start=end3+1;
				sql+=","+temp+" <=";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2>0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start-1);
				start=end3+1;
				sql+=","+temp+" >=";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1>0&&end2>0&&end3<0&&end4<0)
			{
				if(end1<end2)
				{
					temp.assign(SQL,start,end2-start-1);
					start=end2+1;
					sql+=","+temp+" <>";
					temp.assign(SQL,start,end-start);
					sql+=" "+temp;
				}
				else
				{
					temp.assign(SQL,start,end1-start-1);
					start=end1+1;
					sql+=","+temp+" ><";
					temp.assign(SQL,start,end-start);
					sql+=" "+temp;
				}
			}
			else if(end1<0&&end2<0&&end3>0&&end4>0)
			{
				temp.assign(SQL,start,end3-start-1);
				start=end3+1;
				sql+=","+temp+" !=";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else
			{
				cout<<"error:"<<" missing > < = <= >= <> >< != !"<<endl;
				SQL="99";
				return SQL;
			}

		}
		else if(temp.find(' ')==-1)
		{
			cout<<"error:"<<" "<<temp<<"---iss not a valid condition!"<<endl;
			SQL="99";
			return SQL;
		}
		else if(temp.find("and")==-1)
		{
			end1=SQL.find('<',start);
			end2=SQL.find('>',start);
			end3=SQL.find('=',start);
			end4=SQL.find('!',start);
			if(end1>0&&end2<0&&end3<0&&end4<0)
			{
				temp.assign(SQL,start,end1-start);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end1+1;
				sql+=","+temp+" <";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2>0&&end3<0&&end4<0)
			{
				temp.assign(SQL,start,end2-start);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end2+1;
				sql+=","+temp+" >";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2<0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end3+1;
				sql+=","+temp+" =";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1>0&&end2<0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start-1);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end3+1;
				sql+=","+temp+" <=";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2>0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start-1);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end3+1;
				sql+=","+temp+" >=";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1>0&&end2>0&&end3<0&&end4<0)
			{
				if(end1<end2)
				{
					temp.assign(SQL,start,end2-start-1);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end2+1;
					sql+=","+temp+" <>";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start);
					sql+=" "+temp;
				}
				else
				{
					temp.assign(SQL,start,end1-start-1);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end1+1;
					sql+=","+temp+" <>";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start);
					sql+=" "+temp;
				}
			}
			else if(end1<0&&end2<0&&end3>0&&end4>0)
			{
				temp.assign(SQL,start,end3-start-1);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end3+1;
				sql+=","+temp+" !=";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else
			{
				cout<<"error:"<<" missing > < = <= >= <> >< != !"<<endl;
				SQL="99";
				return SQL;
			}
		}

		else if(temp.find("and")>0)
		{
			sql+=",";
			do{
				if(SQL.find("and",start)!=-1)
				{
					end=SQL.find("and",start)+3;
					Temp.assign(SQL,start,end-start-3);
					k=3;
				}
				else
				{
					end=SQL.find(';',start);
					Temp.assign(SQL,start,end-start);
					finish=true;
					k=0;
				}
				end1=Temp.find('<',0);
				end2=Temp.find('>',0);
				end3=Temp.find('=',0);
				end4=Temp.find('!',0);
				if(end1>0&&end2<0&&end3<0&&end4<0)
				{
					end1=SQL.find('<',start);
					temp.assign(SQL,start,end1-start);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end1+1;
					sql+=temp+" < ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else if(end1<0&&end2>0&&end3<0&&end4<0)
				{
					end2=SQL.find('>',start);
					temp.assign(SQL,start,end2-start);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end2+1;
					sql+=temp+" > ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else if(end1<0&&end2<0&&end3>0&&end4<0)
				{
					end3=SQL.find('=',start);
					temp.assign(SQL,start,end3-start);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end3+1;
					sql+=temp+" = ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else if(end1>0&&end2<0&&end3>0&&end4<0)
				{
					end3=SQL.find('=',start);
					temp.assign(SQL,start,end3-start-1);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end3+1;
					sql+=temp+" <= ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else if(end1<0&&end2>0&&end3>0&&end4<0)
				{
					end3=SQL.find('=',start);
					temp.assign(SQL,start,end3-start-1);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end3+1;
					sql+=temp+" >= ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else if(end1>0&&end2>0&&end3<0&&end4<0)
				{
					if(end1<end2)
					{
						end2=SQL.find('>',start);
						temp.assign(SQL,start,end2-start-1);
						length=temp.length()-1;
						while(temp.at(length)==' ')
							length--;
						temp.assign(temp,0,length+1);
						start=end2+1;
						sql+=temp+" <> ";
						while(SQL.at(start)==' ')
							start++;
						temp.assign(SQL,start,end-start-k);
						sql+=temp;
					}
					else
					{
						end1=SQL.find('<',start);
						temp.assign(SQL,start,end1-start-1);
						length=temp.length()-1;
						while(temp.at(length)==' ')
							length--;
						temp.assign(temp,0,length+1);
						start=end1+1;
						sql+=temp+" >< ";
						while(SQL.at(start)==' ')
							start++;
						temp.assign(SQL,start,end-start-k);
						sql+=temp;
					}
				}
				else if(end1<0&&end2<0&&end3>0&&end4>0)
				{
					end3=SQL.find('=',start);
					temp.assign(SQL,start,end3-start-1);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end3+1;
					sql+=temp+" != ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else
				{
					cout<<"error:"<<" missing > < = <= >= <> >< != !"<<endl;
					SQL="99";
					return SQL;
				}
				if(end<(SQL.length()-1))
				{
					start=end+1;
					while(SQL.at(start)==' ')
						start++;
				}
				sql+=" and ";
			}while(!finish);
			sql.assign(sql,0,sql.length()-5);
		}
		else
		{
			cout<<"error:"<<" wrong condition"<<endl;
			SQL="99";
			return SQL;
		}
	}
	SQL = "20" + sql;
	return SQL;
}




string Interpreter::select_clause(string SQL,int start)
{
	string temp,temp1,sql;
	int end,length;
	bool finish = false;

	if(SQL.find(',')==-1)
	{
		while(SQL.at(start)==' ')
			start++;
		end=SQL.find(' ',start);
		temp.assign(SQL,start,end-start);
		start=end+1;

		if(temp.empty())
		{
			cout<<"syntax error: syntax error for select statement!"<<endl;
			SQL="99";
		}
		else if(temp=="*")
		{

			while(SQL.at(start)==' ')
				start++;
			end=SQL.find(' ',start);
			temp.assign(SQL,start,end-start);
			start=end+1;

			if(temp.empty())
			{
				cout<<"syntax error: syntax error for select statement!"<<endl;
				SQL="99";
			}
			else if(temp=="from")
			{
				while(SQL.at(start)==' ')
					start++;
				end=SQL.find(' ',start);
				temp.assign(SQL,start,end-start);
				start=end+1;
				if(temp.empty())
				{
					cout<<"syntax error: syntax error for select statement!"<<endl;
					SQL="99";
				}
				else
				{
					sql=temp;

					while(SQL.at(start)==' ')
						start++;
					end=SQL.find(' ',start);
					temp.assign(SQL,start,end-start);
					start=end+1;
					
					if(temp==";")
						SQL="20"+sql;
					else if(temp=="where")
						SQL=select_from_where(SQL,start,sql);
			
					else
					{
						cout<<"syntax error:"<<" "<<temp<<"---is not a valid key world!"<<endl;
						SQL="99";
					}
				}

			}
		
			else
			{
				cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
				SQL="99";
			}

		}
		
		else
		{
			sql=temp;
		
			while(SQL.at(start)==' ')
				start++;
			end=SQL.find(' ',start);
			temp.assign(SQL,start,end-start);
			start=end+1;
			change(temp);
		
			if(temp.empty())
			{
				cout<<"syntax error: syntax error for select statement!"<<endl;
				SQL="99";
			}
			else if(temp=="from")
			{
				
				while(SQL.at(start)==' ')
					start++;
				end=SQL.find(' ',start);
				temp.assign(SQL,start,end-start);
				start=end+1;
			
				if(temp.empty())
				{
					cout<<"syntax error: syntax error for select statement!"<<endl;
					SQL="99";
				}
				else
				{
					sql=sql+' '+temp;
				
					while(SQL.at(start)==' ')
						start++;
					end=SQL.find(' ',start);
					temp.assign(SQL,start,end-start);
					start=end+1;
					change(temp);
				
					if(temp==";")
						SQL="20"+sql;
					else if(temp=="where")
						SQL=select_from_where(SQL,start,sql);
				
					else
					{
						cout<<"syntax error:"<<" "<<temp<<"---is not a valid key world!"<<endl;
						SQL="99";
					}
				}

			}
		
			else
			{
				cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
				SQL="99";
			}

		}
	}

	else
	{

		while(!finish)
		{
			while(SQL.at(start)==' ')
				start++;
			if((end=SQL.find(',',start))==-1)
			{
				end=SQL.find(' ',start);
				temp.assign(SQL,start,end-start);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end+1;
				sql+=temp+" ";
				finish=true;
			}
			else
			{
				end=SQL.find(',',start);
				temp.assign(SQL,start,end-start);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end+1;
				sql+=temp+',';
			}

		}
	
		while(SQL.at(start)==' ')
			start++;
		end=SQL.find(' ',start);
		temp.assign(SQL,start,end-start);
		start=end+1;
		change(temp);
	
		if(temp.empty())
		{
			cout<<"syntax error: syntax error for select statement!"<<endl;
			SQL="99";
		}
		else if(temp=="from")
		{
		
			while(SQL.at(start)==' ')
				start++;
			end=SQL.find(' ',start);
			temp.assign(SQL,start,end-start);
			start=end+1;
			change(temp);
		
			if(temp.empty())
			{
				cout<<"syntax error: syntax error for select statement!"<<endl;
				SQL="99";
			}
			else
			{
				sql+=temp;
			
				while(SQL.at(start)==' ')
					start++;
				end=SQL.find(' ',start);
				temp.assign(SQL,start,end-start);
				start=end+1;
			
				if(temp==";")
					SQL="20"+sql;
				else if(temp=="where")
					SQL=select_from_where(SQL,start,sql);
			
				else
				{
					cout<<"syntax error:"<<" "<<temp<<"---is not a valid key world!"<<endl;
					SQL="99";
				}
			}

		}
	
		else
		{
			cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
			SQL="99";
		}

	}

	return SQL;
}



string Interpreter::insert_into_values(string SQL,int start,string sql)
{
	string temp;
	int end;
	while(SQL.at(start)=='(')
		start++;

	while((end=SQL.find(',',start))!=-1)
	{
		temp.assign(SQL,start,end-start);
		start=end+1;
		
		if(temp.empty())
		{
			cout<<"error: error in insert into values statement!"<<endl;
			SQL="99";
			return SQL;
		}
	
		else
			sql+=" "+temp;
		while(SQL.at(start)==' ')
			start++;
	}
	end = SQL.find(')', start);
	temp.assign(SQL,start,end-start);
	start=end+1;

	if(temp.empty())
	{
		cout<<"error: error in insert into values statement!"<<endl;
		SQL="99";
		return SQL;
	}

	else
		sql+=" "+temp;
	SQL="30"+sql;
	return SQL;
}



string Interpreter::insert_clause(string SQL,int start)
{
	string temp,sql;
	int end;

	while(SQL.at(start)==' ')
		start++;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;
	while(SQL.at(start)==' ')
		start++;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;

	if(temp.empty())
	{
		cout<<"syntax error: syntax error for insert statement!"<<endl;
		SQL="99";
	}
	else
	{
		sql=temp;
	
		while(SQL.at(start)==' ')
			start++;
		end=SQL.find('(',start);
		temp.assign(SQL,start,end-start);
		start=end+1;
		change(temp);
		
		if(temp.empty())
		{
			cout<<"syntax error: syntax error for insert statement!"<<endl;
			SQL="99";
		}
	
		else if(temp=="values")
			SQL=insert_into_values(SQL,start,sql);
	
		else
		{
			cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
			SQL="99";
		}
	}
	return SQL;
}


string Interpreter::delete_from_where(string SQL,int start,string sql)
{
	string temp,Temp;
	int end,length,end1,end2,end3,end4,k;
	bool finish=false;

	while(SQL.at(start)==' ')
		start++;
	end=SQL.find(';',start);
	temp.assign(SQL,start,end-start);

	if(temp.empty())
	{
		cout<<"syntax error: syntax error for select statement!"<<endl;
		cout<<"\t missing ; !"<<endl;
		SQL="99";
		return SQL;
	}
	else
	{

		length=temp.length()-1;
		while(temp.at(length)==' ')
			length--;
		temp.assign(temp,0,length+1);

		if(temp.find(' ')==-1&&temp.length()>1)
		{
			//while(SQL.at(start)==' ')
				//start++;
			end1=SQL.find('<',start);
			end2=SQL.find('>',start);
			end3=SQL.find('=',start);
			end4=SQL.find('!',start);
			if(end1>0&&end2<0&&end3<0&&end4<0)
			{
				temp.assign(SQL,start,end1-start);
				start=end1+1;
				sql+=","+temp+" <";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2>0&&end3<0&&end4<0)
			{
				temp.assign(SQL,start,end2-start);
				start=end2+1;
				sql+=","+temp+" >";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2<0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start);
				start=end3+1;
				sql+=","+temp+" =";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1>0&&end2<0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start-1);
				start=end3+1;
				sql+=","+temp+" <=";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2>0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start-1);
				start=end3+1;
				sql+=","+temp+" >=";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1>0&&end2>0&&end3<0&&end4<0)
			{
				if(end1<end2)
				{
					temp.assign(SQL,start,end2-start-1);
					start=end2+1;
					sql+=","+temp+" <>";
					temp.assign(SQL,start,end-start);
					sql+=" "+temp;
				}
				else
				{
					temp.assign(SQL,start,end1-start-1);
					start=end1+1;
					sql+=","+temp+" ><";
					temp.assign(SQL,start,end-start);
					sql+=" "+temp;
				}
			}
			else if(end1<0&&end2<0&&end3>0&&end4>0)
			{
				temp.assign(SQL,start,end3-start-1);
				start=end3+1;
				sql+=","+temp+" !=";
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else
			{
				cout<<"error:"<<" missing > < = <= >= <> >< != !"<<endl;
				SQL="99";
				return SQL;
			}

		}

		else if(temp.find(' ')==-1)
		{
			cout<<"error:"<<" "<<temp<<"---iss not a valid condition!"<<endl;
			SQL="99";
			return SQL;
		}

		else if(temp.find("and")==-1)
		{
			end1=SQL.find('<',start);
			end2=SQL.find('>',start);
			end3=SQL.find('=',start);
			end4=SQL.find('!',start);
			if(end1>0&&end2<0&&end3<0&&end4<0)
			{
				temp.assign(SQL,start,end1-start);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end1+1;
				sql+=","+temp+" <";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2>0&&end3<0&&end4<0)
			{
				temp.assign(SQL,start,end2-start);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end2+1;
				sql+=","+temp+" >";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2<0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end3+1;
				sql+=","+temp+" =";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1>0&&end2<0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start-1);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end3+1;
				sql+=","+temp+" <=";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1<0&&end2>0&&end3>0&&end4<0)
			{
				temp.assign(SQL,start,end3-start-1);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end3+1;
				sql+=","+temp+" >=";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else if(end1>0&&end2>0&&end3<0&&end4<0)
			{
				if(end1<end2)
				{
					temp.assign(SQL,start,end2-start-1);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end2+1;
					sql+=","+temp+" <>";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start);
					sql+=" "+temp;
				}
				else
				{
					temp.assign(SQL,start,end1-start-1);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end1+1;
					sql+=","+temp+" <>";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start);
					sql+=" "+temp;
				}
			}
			else if(end1<0&&end2<0&&end3>0&&end4>0)
			{
				temp.assign(SQL,start,end3-start-1);
				length=temp.length()-1;
				while(temp.at(length)==' ')
					length--;
				temp.assign(temp,0,length+1);
				start=end3+1;
				sql+=","+temp+" !=";
				while(SQL.at(start)==' ')
					start++;
				temp.assign(SQL,start,end-start);
				sql+=" "+temp;
			}
			else
			{
				cout<<"error:"<<" missing > < = <= >= <> >< != !"<<endl;
				SQL="99";
				return SQL;
			}
		}

		else if(temp.find("and")>0)
		{
			sql+=",";
			do{
				if(SQL.find("and",start)!=-1)
				{
					end=SQL.find("and",start)+3;
					Temp.assign(SQL,start,end-start-3);
					k=3;
				}
				else
				{
					end=SQL.find(';',start);
					Temp.assign(SQL,start,end-start);
					finish=true;
					k=0;
				}
				end1=Temp.find('<',0);
				end2=Temp.find('>',0);
				end3=Temp.find('=',0);
				end4=Temp.find('!',0);
				if(end1>0&&end2<0&&end3<0&&end4<0)
				{
					end1=SQL.find('<',start);
					temp.assign(SQL,start,end1-start);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end1+1;
					sql+=temp+" < ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else if(end1<0&&end2>0&&end3<0&&end4<0)
				{
					end2=SQL.find('>',start);
					temp.assign(SQL,start,end2-start);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end2+1;
					sql+=temp+" > ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else if(end1<0&&end2<0&&end3>0&&end4<0)
				{
					end3=SQL.find('=',start);
					temp.assign(SQL,start,end3-start);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end3+1;
					sql+=temp+" = ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else if(end1>0&&end2<0&&end3>0&&end4<0)
				{
					end3=SQL.find('=',start);
					temp.assign(SQL,start,end3-start-1);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end3+1;
					sql+=temp+" <= ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else if(end1<0&&end2>0&&end3>0&&end4<0)
				{
					end3=SQL.find('=',start);
					temp.assign(SQL,start,end3-start-1);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end3+1;
					sql+=temp+" >= ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else if(end1>0&&end2>0&&end3<0&&end4<0)
				{
					if(end1<end2)
					{
						end2=SQL.find('>',start);
						temp.assign(SQL,start,end2-start-1);
						length=temp.length()-1;
						while(temp.at(length)==' ')
							length--;
						temp.assign(temp,0,length+1);
						start=end2+1;
						sql+=temp+" <> ";
						while(SQL.at(start)==' ')
							start++;
						temp.assign(SQL,start,end-start-k);
						sql+=temp;
					}
					else
					{
						end1=SQL.find('<',start);
						temp.assign(SQL,start,end1-start-1);
						length=temp.length()-1;
						while(temp.at(length)==' ')
							length--;
						temp.assign(temp,0,length+1);
						start=end1+1;
						sql+=temp+" >< ";
						while(SQL.at(start)==' ')
							start++;
						temp.assign(SQL,start,end-start-k);
						sql+=temp;
					}
				}
				else if(end1<0&&end2<0&&end3>0&&end4>0)
				{
					end3=SQL.find('=',start);
					temp.assign(SQL,start,end3-start-1);
					length=temp.length()-1;
					while(temp.at(length)==' ')
						length--;
					temp.assign(temp,0,length+1);
					start=end3+1;
					sql+=temp+" != ";
					while(SQL.at(start)==' ')
						start++;
					temp.assign(SQL,start,end-start-k);
					sql+=temp;
				}
				else
				{
					cout<<"error:"<<" missing > < = <= >= <> >< != !"<<endl;
					SQL="99";
					return SQL;
				}
				if(end<(SQL.length()-1))
				{
					start=end+1;
					while(SQL.at(start)==' ')
						start++;
				}
				sql+=" and ";
			}while(!finish);
			sql.assign(sql,0,sql.length()-5);
		}
		else
		{
			cout<<"error:"<<" wrong condition"<<endl;
			SQL="99";
			return SQL;
		}
	}
	SQL = "40" + sql;
	return SQL;
}


string Interpreter::delete_clause(string SQL,int start)
{
	string temp,sql;
	int end;
		while(SQL.at(start)==' ')
			start++;
		end=SQL.find(' ',start);
		temp.assign(SQL,start,end-start);
		start=end+1;
		change(temp);

		if(temp.empty())
		{
			cout<<"syntax error: syntax error for select statement!"<<endl;
			SQL="99";
		}
		else if(temp=="from")
		{
	
			while(SQL.at(start)==' ')
				start++;
			end=SQL.find(' ',start);
			temp.assign(SQL,start,end-start);
			start=end+1;
		
			if(temp.empty())
			{
				cout<<"syntax error: syntax error for select statement!"<<endl;
				SQL="99";
			}
			else
			{
				sql=temp;
		
				while(SQL.at(start)==' ')
					start++;
				end=SQL.find(' ',start);
				temp.assign(SQL,start,end-start);
				start=end+1;
				change(temp);
			
				if(temp==";")
					SQL="40"+sql;
			
				else if(temp=="where")
					SQL=delete_from_where(SQL,start,sql);
			
				else
				{
					cout<<"syntax error:"<<" "<<temp<<"---is not a valid key world!"<<endl;
					SQL="99";
				}
			}

		}
	
		else
		{
			cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
			SQL="99";
		}


	return SQL;
}


string Interpreter::quit_clause(string SQL,int start)
{
	string temp;
	int end;

	while(SQL.at(start)==' ')
		start++;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;

	if(temp==";")
		SQL="50";

	else
	{
		cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
		SQL="99";
	}
	return SQL;
}

string Interpreter::execfile_clause(string SQL,int start)
{
	string temp;
	int end;

	while(SQL.at(start)==' ')
		start++;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;

	if(temp.empty())
	{
		cout<<"syntax error: syntax error for execfile statement!"<<endl;
		SQL="99";
	}
	else
		SQL="60"+temp;

	return SQL;
}

string Interpreter::use_clause(string SQL,int start)
{
	string temp;
	int end;
	while(SQL.at(start)==' ')
		start++;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;

	if(temp.empty())
	{
		cout<<"syntax error: syntax error for use database statement!"<<endl;
		SQL="99";
	}
	else
		SQL="70"+temp;

	return SQL;
}

string Interpreter::interpreter(string statement)
{
	string SQL;
	string temp;
	string sql;

	if(statement == "" )
	{
		SQL="99";
		return SQL;
	}
	if(statement.find_first_of(";") == -1)
	{
		SQL="98";
		return SQL;
	}
	statement = " " + statement;
	statement.insert(statement.find_first_of(";"), " ");

	int start=0,end;
	if(statement.empty())
		SQL=read_input();
	else
		SQL=statement;
	while(SQL.at(start)==' ')
		start++;
	end=SQL.find(' ',start);
	temp.assign(SQL,start,end-start);
	start=end+1;
	change(temp);
	//cout<<temp<<endl;
	//cout<<SQL<<endl;
	if(temp.empty())
	{
		cout<<"syntax error: empty statement!"<<endl;
		SQL="99";
	}
	
	else if(temp=="create")
		SQL=create_clause(SQL,start);
	else if(temp=="drop")
		SQL=drop_clause(SQL,start);

	else if(temp=="select")
		SQL=select_clause(SQL,start);

	else if(temp=="insert")
		SQL=insert_clause(SQL,start);

	else if(temp=="delete")
		SQL=delete_clause(SQL,start);

	else if(temp=="execfile")
		SQL=execfile_clause(SQL,start);

	else if(temp=="quit")
		SQL=quit_clause(SQL,start);

	else if(temp=="use")
		SQL=use_clause(SQL,start);

	else
	{
		cout<<"syntax error:"<<" "<<temp<<"---is not a valid key word!"<<endl;
		SQL="99";
	}

	return SQL;
}

void Interpreter::change(string &SQL){
	for(unsigned int i=0;i < SQL.size();i++)
	{
		if(SQL[i]>='A'&&SQL[i]<='Z')
		{
			SQL[i] += 32;
			continue;
		 }
	}
};


