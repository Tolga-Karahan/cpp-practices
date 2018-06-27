#include <boost/property_tree/ptree.hpp>
#include <utility>
#include <iostream>

int main(){

	typedef boost::property_tree::basic_ptree<std::string, int> ptree;

	// Koku olustur
	ptree root;
	// Linux path seperator ı kullanarak branch olustur
	root.put(ptree::path_type("~//Documents//staj", '//'), 20);
	root.put(ptree::path_type("~//Documents//files", '//'), 30);

	// Documents branch ini al
	ptree &main_branch = root.get_child(ptree::path_type("~//Documents", '//'));
	int files = 0;

	for(const std::pair<std::string, ptree> &p : main_branch){
		files += p.second.get_value<int>();
	}

	std::cout << files << std::endl;




}