#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>
#include <iostream>
#include <cstdlib>

struct string_to_int_translator{
	typedef std::string internal_type;
	typedef int external_type;

	boost::optional<int> get_value(const std::string &s){
		char *c;
		long l = std::strtol(s.c_str(), &c, 10);
		return boost::make_optional(c != s.c_str(), static_cast<int>(l));
	}
};

int main(){

	typedef boost::property_tree::iptree ptree;
	ptree pt;

	pt.put(ptree::path_type{"~//Documents//staj", '//'}, "20 files");
	pt.put(ptree::path_type{"~//Documents//files", '//'}, "30 files");

	string_to_int_translator translator;
	int files =
		pt.get<int>(ptree::path_type{"~//Documents//staj", '//'}, translator) +
		pt.get<int>(ptree::path_type{"~//Documents//files", '//'}, translator);
	std::cout << files << std::endl;	
	return 0;
}