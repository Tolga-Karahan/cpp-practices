#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

using namespace boost::property_tree;

int main(){
	ptree root;

	root.put_child("~.Documents.staj", ptree{"3 files"});
	root.put_child("~.Downloads", ptree{"10 files"});
	root.add_child("~.Desktop", ptree{"15 files"});

	json_parser::write_json("file.json", root);

	ptree &child = root.get_child("~");
	for(const std::pair<std::string, ptree> &p : child){
		std::cout << p.second.get_value<std::string>() << std::endl;
	}	
}