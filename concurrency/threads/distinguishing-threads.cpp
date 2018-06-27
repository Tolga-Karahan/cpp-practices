/* Her bir thread onlari ayirt etmemizi saglayan bir id ye sahiptir.
std::thread sinifinin get_id() fonksiyonu thread e ait unique id
degerini dondurur. */

#include <thread>
#include <iostream>
#include <vector>

void hello(){
	std::cout<<"Hello from thread " << std::this_thread::get_id() << std::endl;
}

int main(){
	std::vector<std::thread> threads;

	for(int i = 0; i < 5; i++){
		threads.push_back(std::thread(hello));
	}

	for(auto& thread : threads){
		thread.join();
	}

	return 0;
}	

