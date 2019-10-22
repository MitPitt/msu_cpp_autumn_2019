
#include <iostream>
#include <cstring>

class LinearAllocator
{
	char* base;
	size_t maxSize, curSize;
public:
	LinearAllocator(size_t maxSize) : maxSize(maxSize), curSize(0) { base = new char[maxSize]; }
	char* alloc(size_t size) {
		if (curSize + size > maxSize)
			return nullptr;
		else {
			curSize += size;
			return base + curSize - size;
		}
	}
	void reset() { curSize = 0; }
	~LinearAllocator() { delete base; }
};

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "no parameters";
		return 1;
	}
	else {
		if (atoi(argv[1]) <= 0) {
			std::cout << "incorrect maxSize";
			return 1;
		}
		else {
			auto a = LinearAllocator(atoi(argv[1]));
			for (int i = 2; i < argc; i++) {
				if (strcmp(argv[i], "reset") == 0){
					a.reset();
					std::cout << "r";
				}
				else if (atoi(argv[i]) > 0){
					if(a.alloc(atoi(argv[i])) == nullptr)
						std::cout << "!";
					else
						std::cout << ".";
				}
				else {
					std::cout << "incorrect size or unknown command";
					return 1;
				}
			}
			return 0;
		}
	}
}