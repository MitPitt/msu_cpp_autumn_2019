
#include <iostream>
#include <vector>
#include <string>
#include <map>

class MyException {
	std::string mes;
public:
	virtual std::string message() {
        	return mes;
    	}
	MyException(const std::string& a) : mes(a) {}
};

enum tokenType {Number,Plus,BinMinus,UnMinus,Mult,Div};
std::map<char, tokenType> dict = { {'N',Number} ,{'+',Plus} ,{'-',BinMinus} ,{'~',UnMinus} ,{'*',Mult} ,{'/',Div} };
std::map<tokenType, int> price = { {Plus,1} ,{BinMinus,1} ,{Mult,2} ,{Div,2} };

class token {
	tokenType type;
	int Num;
public:
	tokenType Type() {
		return type;
	}
	int Value() {
		if (type == Number) return Num;
		else throw MyException("token is not a number!");
	}
	token(tokenType a) :type(a) {}
	token(tokenType a, int x) :type(a), Num(x) {}
};
void readNum(const std::string& str, int& i, std::vector<token>& tokens) {
	while (str[i] == ' ') i++;
	while (str[i] == '-') {
		tokens.push_back(token(UnMinus));
		i++;
		while (str[i] == ' ') i++;
	}
	while (str[i] == ' ') i++;
	int start = i;
	if ((str[i] > '9') || (str[i] < '0'))
		throw MyException("bad number");
	while ((str[i] <= '9') && (str[i] >= '0'))
		i++;
	std::string newnum = str.substr(start, i - start);
	int num = stoi(newnum);
	tokens.push_back(token(Number, num));
}
void readOp(const std::string& str, int& i, std::vector<token>& tokens) {
	while (str[i] == ' ') i++;
	char c = str[i];
	if ((c == '*') || (c == '/') || (c == '+') || (c == '-')) {
		tokens.push_back(token(dict[c]));
		i++;
	}
	else throw MyException("bad operation");
}
void tokenize(const std::string& str, std::vector<token>& tokens) {
	int i = 0;
	while (str[i] == ' ') i++;
	while (str[i] != '\0') {
		readNum(str, i, tokens);
		while (str[i] == ' ') i++;
		if (str[i] != '\0')
			readOp(str, i, tokens);
		while (str[i] == ' ') i++;
	}
	if (tokens.size() == 0)
		throw MyException("no tokens");
	if (tokens.back().Type() != Number)
		throw MyException("last token is an operation");
}

int poliz(const std::vector<token>& tokens) {
	std::vector<token> line;
	std::vector<token> Stack; 
	//use stack for ops
	for (auto i : tokens) {
		if (i.Type() == Number)
			line.push_back(i);
		else if (i.Type() == UnMinus)
			Stack.push_back(i);
		else {
			while ((Stack.size() != 0) && ((Stack.back().Type() == UnMinus) || (price[Stack.back().Type()] >= price[i.Type()]))) {
				line.push_back(Stack.back());
				Stack.pop_back();
			}
			Stack.push_back(i);
		}
	}
	for (auto i : Stack) {
		line.push_back(Stack.back());
		Stack.pop_back();
	}
	//calculate //use stack for nums
	for (auto i : line) {
		if (i.Type() == Number) Stack.push_back(i);
		else if ((i.Type() == Plus) || (i.Type() == BinMinus) || (i.Type() == Mult) || (i.Type() == Div)) {
			int a = Stack.back().Value();
			Stack.pop_back();
			int b = Stack.back().Value();
			Stack.pop_back();
			switch (i.Type()) {
			case Plus:
				a = b + a;
				Stack.push_back(token(Number, a));
				break;
			case BinMinus:
				a = b - a;
				Stack.push_back(token(Number, a));
				break;
			case Mult:
				a = b * a;
				Stack.push_back(token(Number, a));
				break;
			case Div:
				if (a == 0)
					throw MyException("can't divide by 0!");
				a = b / a;
				Stack.push_back(token(Number, a));
				break;
			}
		}
		else {
			int a = Stack.back().Value();
			Stack.pop_back();
			a = -a;
			Stack.push_back(token(Number, a));
		}
	}
	if (Stack.size() != 1)
		throw MyException("stack error");
	return Stack.back().Value();
}

int calc(const std::string& str) {
	std::vector<token> tokens;
	tokenize(str, tokens);
	return poliz(tokens);
}
