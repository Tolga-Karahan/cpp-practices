#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]){
try {
	  // Baglantiyi kur
      connection connect("dbname = test user = postgres password = asd \
      hostaddr = 127.0.0.1 port = 5432");

      if (connect.is_open()) {
         cout << "Opened database successfully: " << connect.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }
      connect.disconnect ();
   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }	

}