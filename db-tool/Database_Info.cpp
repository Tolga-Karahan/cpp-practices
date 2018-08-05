#include "Database_Info.hpp"

using namespace std;

Database_Info::Database_Info(string dbname, string user, 
	string password, string hostaddr, string port){

	this->dbname 	= dbname;
	this->user   	= user;
	this->password 	= password;
	this->hostaddr	= hostaddr;
	this->port	= port;
}

void Database_Info::add_table(string table){
	
	tables.push_back(table);
}

