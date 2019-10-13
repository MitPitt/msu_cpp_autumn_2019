#include "calc.h"

int main(int argc, char* argv[]){
	if (argc == 2) {
		//std::cout << argv[1] << std::endl;
		try {
			std::cout << calc(argv[1]) << std::endl;
		}
		catch (const char* str) {
			std::cerr << str << std::endl;
			return 1;
		}
		return 0;
	}
	else{
		std::cout << "too many or no arguments" << std::endl;
		return 1;
	}

	/*std::string str;
	std::getline(std::cin, str);

	try {
		std::cout << calc(str) << std::endl;
	}
	catch (const char* str) {
		std::cerr << str << std::endl;
		return 1;
	}*/

}