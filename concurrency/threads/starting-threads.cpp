/* Threadlerin baslatilmasi cok kolaydir. std::thread den bir ornek
olusturulmasi ile otomatik olarak baslayacaktir. Yeni bir thread 
olusturuldugunda, threadin icra edecegi kod saglanmalidir. Bunun icin
ilk secenek bir fonksiyon pointer ı gecirmektir. */

#include <thread>
#include <iostream>

void hello(){
	std::cout << "Hello from thread" << std::endl;
}

int main(){
	std::thread t1(hello);
	t1.join();

	return 0;
}

/* Ustteki programda kullanilan join cagrisi o an kosan thread in diger 
threadlerin bitisini beklemesini saglar. Bu cagri unutulursa belirsiz sonucla
dogabilir. */

