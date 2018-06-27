#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]){

	char* sql;

	try{
		// Baglantiyi olustur
		connection connect("dbname= test user=postgres password=asd hostaddr=127.0.0.1 port=5432");

		if(connect.is_open()){
			cout << connect.dbname() << " opened successfully" << endl;
		}else{
			cout << "Connection failed" << endl;
			return 1;
		}

		// SQL sorgusunu olustur
		sql = "SELECT * FROM employees";

		// Nontransaction nesnesi olustur
		nontransaction nontransact(connect);
		result res(nontransact.exec(sql));

		// Satirlar uzerinde iterasyon yap, verileri cek
		for(result::const_iterator iter = res.begin(); iter != res.end(); iter++){
			cout << endl;
			cout << "ID: "   		<< iter[0].as<int>() << "\t";
			cout << "Name: " 		<< iter[1].as<string>() << "\t";
			cout << "Age: "  		<< iter[2].as<int>() << "\t";
			cout << "Address: " 	<< iter[3].as<string>() << "\t";
			cout << "Salary: "		<< iter[4].as<float>() << "\t";
		}

		cout << endl;
		connect.disconnect();

		}catch(const std::exception &e){
			cerr << e.what() << endl;
			return 1;
		}	
	
	return 0;
}