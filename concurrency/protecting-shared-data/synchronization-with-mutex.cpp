/* Kaynaklari paylasan threadler arasindaki senkronizasyon mutex
denilen ozel tipteki semaforlar ile saglanabilir */

#include <vector>
#include <iostream>
#include <thread>
#include <mutex>

struct Counter{
	
	int value;

	Counter() : value(0) {}

	void increment(){
		++value;
	}
};

struct ConcurrentCounter{

	std::mutex mutex;
	Counter counter;

	void increment(){
		std::lock_guard<std::mutex> guard(mutex);
		counter.increment();
	}
};

int main(){

	ConcurrentCounter my_counter;
	std::vector<std::thread> threads;

	for(int i = 0; i < 5; i++){
		threads.push_back(std::thread([&my_counter](){
			for(int j = 0; j < 100; j++){
				my_counter.increment();
			}
		}));
	}

	for(auto& thread : threads){
		thread.join();
	}

	std::cout << "Value of the counter: " << my_counter.counter.value << std::endl;
	
	return 0;
}