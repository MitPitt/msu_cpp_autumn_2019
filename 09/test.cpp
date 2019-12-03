#include "Threads.h"

#include <iostream>

struct A { };

int foo(const A &) { return 7; }

int main() {
    ThreadPool pool(8);

    auto task0 = pool.exec(foo, A());
	std::cout << task0.get() << std::endl;
	
	auto task1 = pool.exec([]() { return 1; });
	std::cout << task1.get() << std::endl;
	
	for (int i = 2; i < 10; ++i) {
		 std::cout << pool.exec([i]() { return i; }).get() << std::endl;
	}
    
    std::cout << "done\n";
    return 0;
}