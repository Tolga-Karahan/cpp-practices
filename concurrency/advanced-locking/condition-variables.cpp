/* Bir kosul degiskeni baska bir thread kendisin uyandirana dek bekleyen
bir dizi threadi yonetir. Kosul degiskeni uzerinde bekleyecek olan bir
thread ilk olarak lock ı edinmelidir. Thread kosul uzerinde beklemeye
basladiginda lock serbest birakilir ve thread uyandiginda ise lock
tekrar edinilir. */

#include <mutex>
#include <thread>
#include <iostream>
#include <chrono>
#include <condition_variable>

struct BoundedBuffer
{
	int* buffer;
	int capacity;

	int front;
	int rear;
	int count;

	std::mutex lock;
	std::condition_variable not_full;
	std::condition_variable not_empty;

	BoundedBuffer(int capacity) : capacity(capacity), front(0),
		rear(0), count(0){
			buffer = new int[capacity];
	}

	~BoundedBuffer(){
		delete [] buffer;
	}

	void deposit(int data){
		std::unique_lock<std::mutex> u_lock(lock);

		not_full.wait(u_lock, [this](){return count != capacity;});

		buffer[rear] = data;
		rear = (rear + 1) % capacity;
		++count;

		u_lock.unlock();
		not_empty.notify_one();
	}

	int fetch(){
		std::unique_lock<std::mutex> u_lock(lock);

		not_empty.wait(u_lock, [this](){return count != 0;});

		int result = buffer[front];
		front = (front + 1) % capacity;
		--count;

		u_lock.unlock();
		not_full.notify_one();

		return result;
	}
};

void consumer(int id, BoundedBuffer& buffer){
	for(int i = 0; i < 50; i++){
		int result = buffer.fetch();
		std::cout << "Consumer thread " << id << " consumes " << result << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
}

void producer(int id, BoundedBuffer& buffer){
	for(int i = 0; i < 75; i++){
		buffer.deposit(i);
		std::cout << "Producer thread " << id << " produces " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main(){

	BoundedBuffer buffer(200);

	std::thread c1(consumer, 0, std::ref(buffer));
	std::thread c2(consumer, 1, std::ref(buffer));
	std::thread c3(consumer, 2, std::ref(buffer));	
	std::thread p1(producer, 0, std::ref(buffer));
	std::thread p2(producer, 1, std::ref(buffer));

	c1.join();
	c2.join();
	c3.join();
	p1.join();
	p2.join();

	return 0;
}