#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include <cassert>

template <class T>
void process(std::vector<std::string>& res, T&& value) {
	std::stringstream out;
	out << value;
	res.push_back(out.str());
}

template <class T, class... Args>
void process(std::vector<std::string>& res, T&& value, Args&&... args) {
	std::stringstream out;
	out << value;
	res.push_back(out.str());
	process(res, std::forward<Args>(args)...);
}

std::string format(const std::string& str){
	return str;
}

template <class... Args>
std::string format(const std::string& str, Args&&... args) {
	std::vector<std::string> res;
	std::stringstream out;
	process(res, std::forward<Args>(args)...);
	size_t len = str.size();
	for (int i = 0; i < len; i++) {
		if (str[i] == '{') {
			int argnum = 0;
			i++;
			while((i < len) && (str[i] != '}')) {
				if (!std::isdigit(str[i]))
				throw std::runtime_error("argnum is not a number");
				argnum = argnum*10 + str[i] - '0';
				i++;
			}
			if (i == len)
				throw std::runtime_error("missing closing bracket");
			if (argnum >= res.size())
				throw std::runtime_error("incorrect number of args");
			out << res[argnum];
		}
		else if (str[i] == '}')
			throw std::runtime_error("closing bracket found before opening bracket");
		else
			out << str[i];
		}
	return out.str();
}

#define checkEqual(x, y)  do { if ((x) != y) std::cout << "line " << __LINE__ << ": expected " << y << " got " << (x) << '\n'; } while(0)
#define checkThrow(x) do { try { (x); } catch (const std::runtime_error& err) { std::cout << "Runtime error: " << err.what() << "\n"; break ; } catch (...) { } std::cout << "expected runtime_error at line " << __LINE__ << '\n'; } while(0)

struct Test{
};

std::ostream& operator<<(std::ostream& out, const Test&) {
	out << "test";
	return out;
}

int main() {
	std::cout << "Checking answers...\n";
	
	checkEqual(format(""), "");
	checkEqual(format("1"), "1");
	checkEqual(format("{1}", 1, 2), "2");
	checkEqual(format("{0}{0}", "a"), "aa");
	checkEqual(format("{0}", Test()), "test");
	const Test test;
	checkEqual(format("{0}", test), "test");
	checkEqual(format("{0}", const_cast<Test&>(test)), "test");
	checkEqual(format("{1} aaa {0}{2} {1}", 1, test, "kek"), "test aaa 1kek test");
	
	std::cout << "Checking throws...\n";
	
	checkThrow(format("{", 1));
	checkThrow(format("{0", 1));
	checkThrow(format("}", 1));
	checkThrow(format("{1}", 1));
	checkThrow(format("{0}{1}{3}", 1, 2));
	checkThrow(format("{0{}", 1));
	checkThrow(format("{0a}", 1));
	checkThrow(format("0}", 1));
	checkThrow(format("{{0}", 1));
	
	std::cout << "done\n";
	return 0;
}
