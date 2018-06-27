#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]){

	char* sql;

	try{

		// Baglantiyi kur
		connection connect("dbname=test user=postgres password=asd hostaddr=127.0.0.1 port=5432");

		if(connect.is_open()){
			cout << connect.dbname() << " opened successfully" << endl;
		}else{
			cout << "Connection failed" << endl;
			return 1;
		}

		// SQL sorgusunu olustur
		sql = 
			"INSERT INTO employees(id, name, age, address, salary) VALUES (112312, 'Tolga Karahan', 24, 'def', 608);"
			"INSERT INTO employees(id, name, age, address, salary) VALUES (185239,  'Esra Can', 20, 'def', 608);"
			"INSERT INTO employees(id, name, age, address, salary) VALUES (112325, 'Andy Morris', 34, 'def', 4608);";

		// Transaction nesnesi olustur
		work transact(connect);

		// Sorguyu icra et
		transact.exec(sql);
		transact.commit();

		cout << "Insertion completed successfully" << endl;
		connect.disconnect();

	}catch(const std::exception &e){
		cerr << e.what() << endl;
		return 1;
	}

	return 0;
}