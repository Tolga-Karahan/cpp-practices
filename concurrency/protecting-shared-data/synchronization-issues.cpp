/* Genellikle threadler arasinda bir takim kaynaklar paylasilir.
Bu ise senkronizasyon problemlerine yol acar. */

#include <thread>
#include <iostream>
#include <vector>

struct Counter{
	
	int value;

	Counter() : value(0){}

	void increment(){
		++value;
	}
};

int main(){

	Counter counter;
	std::vector<std::thread> threads;

	for(int i = 0; i < 5; i++){
		threads.push_back(std::thread([&counter](){
			for(int j = 0; j < 100; j++){
				counter.increment();
			}	
		}));
	}

	for(auto& thread : threads){
		thread.join();
	}

	std::cout<<"Value of the counter: " << counter.value << std::endl;

	return 0;
}

/* Programi kostugumuz zaman farkli ciktilar alabiliriz ve aslinda program
ciktisinin ne olabilecegini tahmin etmemiz mumkun degildir. Bunun sebebi
arttirma isleminin atomik olmamasidir. Dolayısıyla bir threadin icra ettiği
değişkenin değerinin okunması, arttırılması ve yazılması işlemleri arasına
başka threadlerin aynı işlemleri girebilir ve sonuç olarak yanlış değerler
elde edilir. Bu senkronizasyon problemini çözmek için semaforlar kullanılabilir. */