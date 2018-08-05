1) Programin Amaci
	Programin amaci kullanicindan aldigi argumanlara gore json dosyasinda belirtilen
veritabanlarini yedeklemek ve ya restore etmektir.

2) Programin Calistirilmasi
	./db-tool <islem> <json/tar> seklinde calistirilabilir. Programa iki tane arguman verilmesi gerekir.

3) Argumanlar
	Ilk arguman ile backup ve ya restore islemlerinden hangisinin yapilacagi belirtilir. -backup argumani
ile yedekleme, -restore argumani ile restore islemi yapar.

	Ilk argumana ek olarak ikinci bir arguman verilmelidir. Eger yedekleme islemi yapilacaksa yedeklenecek
veritabanlarinin bulundugu json dosyasi, restore islemi yapilacaksa restore edilecek veritabani tablolarinin
bulundugu tar arsivi arguman olarak verilmelidir. 

4) JSON Formati

	Yedeklenecek veritabanlarinin bilgileri asagidaki JSON ornegindeki gibi saglanmalidir:

{
  "db1": {
    "dbname":"db1",
    "user":"postgres",
    "password":"postgres",
    "hostaddr":"0.0.0.0",
    "port":"5432",
    "tables":{
	"table1":"tb1",
	"table2":"tb2",
	"table3":"tb3",
	"table4":"tb4"
     }
  }, 

  {
  "db2": {
    "dbname":"db2",
    "user":"postgres",
    "password":"postgres",
    "hostaddr":"0.0.0.0",
    "port":"5432",
    "tables":{
	"table1":"tb1",
	"table2":"tb2",
	"table3":"tb3",
	"table4":"tb4"
     }
  }  
}
 
}
