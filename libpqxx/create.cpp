#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]){
	
	char* sql;

	try{
		// Baglantiyi ac
		connection connect("dbname = test user = postgres password = asd hostaddr= 127.0.0.1 port = 5432");

		if(connect.is_open()){
			cout << connect.dbname() << " opened successfully" << endl;
		}else{
			cout << "Connection failed" << endl;
			return 1;
		}

		sql = "CREATE TABLE employees(" \
			  "ID INT PRIMARY KEY NOT NULL," \
			  "NAME TEXT NOT NULL,"\
			  "AGE INT NOT NULL," \
			  "ADDRESS CHAR(50)," \
			  "SALARY REAL);";

	 	// Transaction nesnesini olustur		  
		work transact(connect);

		// SQL sorgusunu icra et
		transact.exec(sql);
		transact.commit();

		cout << "Table is created successfully" << endl;

		connect.disconnect();

	}catch(const std::exception &e){
		cout << cerr << e.what() << endl;
		return 1;
	}

	return 0;
}