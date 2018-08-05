// Stream ve string kutuphaneleri
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

// Veritabani islemleri icin kutuphane
#include <pqxx/pqxx>

// Boost kutuphaneleri
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>

// Standart kutuphane
#include <stdlib.h>

// Islem yapilacak veritabanlarinin bilgisini tutacak sinif
#include "Database_Info.hpp"

using namespace std;
using namespace pqxx;
using namespace boost::property_tree;

void parse_json(const char*);
void backup(string, connection*);
void write(result&, string);
void add_to_archive(char*);
void restore(string, connection*);
void create_table(string, vector<string>, vector<string>, connection*);
void insert_row(string, vector<string>, vector<string>, connection*);
unique_ptr<connection> connect(Database_Info&);
void disconnect(connection*);

// Veritabani bilgilerinin bulundugu nesneleri tutan vektor
vector<Database_Info> databases;

int main(int argc, char* argv[]){
	
	// arguman sayisi 3 ve islem argumani backup ise yedekleme
	// arguman sayisi 4 ve islem argumani restore ise restore islemi yapilir
	if(argc == 3 && string(argv[1]) == "-backup"){	

		// Yedeklenecek tablolarin bulunacagi dizini olustur
		boost::filesystem::path p{".//backups"};
		boost::filesystem::create_directory(p);
	
		// Yedekleneme yapilacak veritabani bilgilerini JSON'dan parse et
		parse_json(argv[2]);
		
		// Tablolari yedekle
		for(auto& database : databases){
			
			unique_ptr<connection> con = connect(database);

			for(auto& table: database.tables){
				backup(table, con.get());
			}

			disconnect(con.get());
		}

		// Yedeklenen dosyalari arsive ekle
		add_to_archive(argv[2]);

			
	}else if(argc == 3 && string(argv[1]) == "-restore"){
	
		stringstream command;
		command << "tar -xzf " << argv[2];

		// Archive dosyasini cikart
		system(command.str().c_str());		
		
		// Restore edilecek veritabani bilgilerini JSON'dan parse et			  
		parse_json(string(".//backups//example.json").c_str());
		
		// Tablolari restore et
		for(auto& database : databases){
			
			unique_ptr<connection> con = connect(database);
			
			for(auto& table: database.tables){
				restore(table, con.get());
			}
		
			disconnect(con.get());
		}										  	
	}else{
		cout << "Usage: <operation(backup/restore)> <json-path/archive-path>" << endl;
		return 1;
	}
	
	return 0;
}

// JSON da belirtilen veritabanları ve tabloları cek
void parse_json(const char* path){
	
	// Koku olustur ve JSON dosyasini oku
	ptree root;
	read_json(path, root);
	
	// JSON ı parse et 
	for(auto& database : root){
	
		string dbname		= database.second.get<string>("dbname");
		string user  	        = database.second.get<string>("user");
		string password 	= database.second.get<string>("password");
		string hostaddr		= database.second.get<string>("hostaddr");
		string port		= database.second.get<string>("port");
		
		Database_Info info(dbname, user, password, hostaddr, port);

		// Yedeklenecek tablolarin bilgisini vektore ekle
		for(auto& table : database.second.get_child("tables")){
			info.add_table(table.second.get_value<string>());
		}

		databases.push_back(info);
	}		
	
}

// Veritabani baglantisini kur
unique_ptr<connection> connect(Database_Info &database){

		stringstream connection_string;

		// Baglanti stringini olustur
		connection_string << "dbname=" << database.dbname << " user=" << database.user << " password=" << database.password << " hostaddr=" << database.hostaddr << " port=" << database.port; 

		// Veritabanina baglantiyi kur
		unique_ptr<connection> con(new connection(connection_string.str()));
		
		// Veritabanina baglanti kurulamadiysa sonlandir
		if(!(con->is_open())){
			cerr << "Connection didn't established" << endl;
			exit(1);
		}

		return move(con);
}	

// Veritabani baglantisini sonlandir
void disconnect(connection* con){
	con->disconnect();
}

// Arguman olarak alinan veritabani ve tablolarini yedekle
void backup(string table, connection* con){
	 
	try{	
		// Sorguyu olustur	
		stringstream temp;
	        temp << "SELECT * FROM " << table << ";";
		string query = temp.str();
		temp.str("");
			
		// Nontransaction nesnesini olustur
		nontransaction ntransact(*con);

		// Sorguyu icra et
		result res(ntransact.exec(query));

		// Dosyaya yaz 
		write(res, table);

	}catch(const exception &e){
		cerr << e.what() << endl;
		exit(1);
	}
}	

// Tablolari dosyaya yaz
void write(result &res, string table){
	
	try{
		stringstream path;
		path  << "backups/" << table; 
		ofstream write;
		write.open(path.str());
			
		// Sutun tiplerini dosyaya ekle
		for(int i = 0; i < res.columns(); i++){
			write << res.column_type(i) << "\t";
		}

		write << "\n";
			
		// Sutun basliklarini dosyaya ekle
		for(int i = 0; i < res.columns(); i++){
			write << res.column_name(i) << "\t";
		}
							
		write << "\n";

		for(int iter = 0; iter != res.size(); iter++){	

			// Satir verilerini dosyaya ekle
			for(int i = 0; i < res.columns(); i++){
				write << res[iter][i] << "\t";
			}
			write << "\n";
		}

	// Dosyayi kapat
	write.close();

	}catch(const exception &e){
		cerr << e.what() << endl;
		exit(1);
	}
}

// Yedeklenen tablolari arsivle
void add_to_archive(char* path){
	
	// JSON dosyasini dizine tasi
	stringstream command;
	command << "cp " << path << " .//backups//" << path;
	system(command.str().c_str());
	
	// Dizindeki dosyalari tar a ekle
	string com1("tar -czf backups.tar.gz .//backups");
	system(com1.c_str());

	// Dizini sil
	string com2("rm -rf .//backups");
	system(com2.c_str());	
}	
	
// Parse edilen tablolari restore et
void restore(string table, connection* con){

	string line;
	vector<string> oids;
	vector<string> column_headers;
	boost::char_separator<char> separator{"\t"};
	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;	

	// Dosyayi oku ve verileri al
	stringstream path;
	path << ".//backups//" << table;
	ifstream read(path.str());
	
	if(read.is_open()){

		// oid leri dosyadan oku
		getline(read, line);

		// Her bir sutun icin oid degerlerini tokenize et
		// oid degerleri restore isleminde sutun tipini belirlemek icin kullaniliyor
		tokenizer tokenize_oids{line, separator};
		
		for(tokenizer::iterator iter = tokenize_oids.begin(); iter != tokenize_oids.end(); iter++){
			oids.push_back(*iter);
		}
		
		// Sutun basliklarini dosyadan oku
		getline(read, line);

		// Her bir sutun icin sutun basliklarini tokenize et
		tokenizer tokenize_column_headers{line, separator};
		
		for(tokenizer::iterator iter = tokenize_column_headers.begin(); 
			iter != tokenize_column_headers.end(); iter++){
			column_headers.push_back(*iter);
		}
		
		// oid ve sutun baslik bilgileri alinan tabloyu olustur
	        create_table(table, oids, column_headers, con);

		// Satirlari tabloya ekle 
		while(getline(read, line)){
			
			vector<string> row;
			tokenizer tokenize_row{line, separator}; 
		
			for(tokenizer::iterator iter = tokenize_row.begin();
				iter != tokenize_row.end();iter++){
				row.push_back(*iter);
			}
			insert_row(table, column_headers, row, con);
		}

		read.close();
	}
} 		
				
void create_table(string table, vector<string> oids,
	vector<string> column_headers, connection* con){
	
	try{
		stringstream query;
		query << "CREATE TABLE " << table << " (";

		for(int i = 0; i < oids.size(); i++){
	
			// Sutun tipini ogrenmek icin sorguyu olustur
			stringstream temp;
			temp << "SELECT typname FROM pg_type where oid=" << oids.at(i) << ";";
			
			// non-transaction nesnesini olustur
			work ntransact(*con);

			// Tipi al
			result res(ntransact.exec(temp.str()));
			string type =  res.begin()[0].as<string>();
			query << column_headers.at(i) << " " << type;			

                        // Sorgunun sonu degilse virgul, sorgu sonu ise noktali virgul koy 
			if(i != (oids.size()- 1)){
				query <<  ", ";
			}else{
				query << ");";
			} 
		} 
		
		work transact(*con);
		transact.exec(query);
		transact.commit();

	}catch(const exception &e){
		cerr << e.what();
		exit(1);
	}								
}						

void insert_row(string table, vector<string> column_headers,
	vector<string> row, connection* con){
	
	try{
		stringstream query;
		query << "INSERT INTO " << table << " (";
		
		for(int i = 0; i < column_headers.size(); i++){
			query << column_headers.at(i);
		
			if(i != column_headers.size() - 1){
				query << ",";
			}else{
				query << ") ";
			}
		} 

 		query << "VALUES (";

		for(int i = 0; i < row.size(); i++){
			
			query << "'" << row.at(i) << "'";

			if(i != row.size() - 1){
				query << ", ";
			}else{
				query << ");";
			}
	
		}
 		
		work transact(*con);
		transact.exec(query);
		transact.commit();	

	}catch(const exception &e){
		cerr << e.what();
		exit(1);
	}
}
