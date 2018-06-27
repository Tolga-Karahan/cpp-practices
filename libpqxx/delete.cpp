#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]){

	char* sql;

	try{
		// Baglantiyi olustur
		connection connect("dbname=test user=postgres password=asd hostaddr=127.0.0.1 port=5432");

		if(connect.is_open()){
			cout << connect.dbname() << "opened successfully" << endl;
		}else{
			cerr << "Connection failed" << endl;
			return 1;
		}

		// SQL sorgusunu olustur
		sql = "DELETE from employee where name='Andy Morris'";

		// Transaction nesnesini olustur
		work transact(connect);

		// Sorguyu icra et
		transact.exec(sql);
		transact.commit();

		connect.disconnect();
		
	}catch(const std::exception &e){
		cerr << e.what() << endl;
		return 1;
	}
}