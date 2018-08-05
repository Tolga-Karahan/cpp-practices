#ifndef Database_Info_HPP
#define	Database_Info_HPP

#include <string>
#include <vector>
using namespace std;

class Database_Info{

	public:
		string dbname;
		string user;
		string password;
		string hostaddr;
		string port;
		vector<string> tables;
		
		Database_Info(string, string, string, string, string);
		void add_table(string);
};

#endif
