#include <boost/property_tree/ptree.hpp>
#include <iostream>

using boost::property_tree::ptree;

int main(){
	// JSON formatinda veri yazmak icin ptree tipinde bir kok tanimliyoruz
	ptree root;
	// . token ı ile branchler olusturarak en alttaki branch e deger atiyoruz
	root.put("~.Documents.staj", "3 files");

	// Staj branch inin degerini aliyoruz ve yazdiriyoruz
	// Get_child fonksiyonu uzerinde cagirildigi tipte bir nesneye referans dondurur
	// Dolayisiyla dondurulen tiplerde ptree olur
	ptree &branch = root.get_child("~").get_child("Documents").get_child("staj");
	std::cout << branch.get_value<std::string>() << std:: endl;

}