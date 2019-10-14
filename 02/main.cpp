#include "calc.h"

int main(int argc, char* argv[]){
	if (argc == 2) {
		try {
			std::cout << calc(argv[1]) << std::endl;
		}
		catch (MyException error) {
			std::cerr << error.message() << std::endl;
			return 1;
		}
		return 0;
	}
	else{
		std::cout << "too many or no arguments" << std::endl;
		return 1;
	}
}
