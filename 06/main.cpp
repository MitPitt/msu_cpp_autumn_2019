#include <iostream>
#include <sstream>
#include <cassert>

enum class Error {
	NoError,
	CorruptedArchive
};

class Serializer {
public:
	explicit Serializer(std::ostream& out) : out_(out) {}

	template <class T>
	Error save(T& object) {	return object.serialize(*this); }

	template <class... ArgsT>
	Error operator()(ArgsT... args) { return process(args...); }

private:
	static constexpr char Separator = ' ';
	std::ostream& out_;

	template<class T>
	Error process(T &value) { return save_(value); }

	template<class T, class... Args>
	Error process(T val, Args... args) {
		if (save_(val) == Error::CorruptedArchive)
			return Error::CorruptedArchive;
		else
			return process(args...);
	}

	template<class T>
	Error save_(T) { return Error::CorruptedArchive; }

	Error save_(bool value) {
		if (value)
			out_ << "true" << Separator;
		else
			out_ << "false" << Separator;
		return Error::NoError;
	}
	
	Error save_(uint64_t value) {
		out_ << value << Separator;
		return Error::NoError;
	}
};

class Deserializer {
public:
	explicit Deserializer(std::istream &in) : in_(in) {}

	template<class T>
	Error load(T &object) {	return object.serialize(*this);	}

	template<class... ArgsT>
	Error operator()(ArgsT &... args) {	return process(args...); }

private:
	std::istream &in_;

	template<class T>
	Error process(T &value) {	return load_(value); }

	template<class T, class... Args>
	Error process(T &value, Args &... args) {
		if (load_(value) == Error::CorruptedArchive) {
			return Error::CorruptedArchive;
		}
		else {
			return process(args...);
		}
	}

	template<class T>
	Error load_(T) { return Error::CorruptedArchive; }

	Error load_(bool &value) {
		std::string text;
		in_ >> text;

		if (text == " ")
			return Error::NoError;
		if (text == "true")
			value = true;
		else if (text == "false")
			value = false;
		else
			return Error::CorruptedArchive;

		return Error::NoError;
	}

	Error load_(uint64_t &value) {
		in_ >> value;
		return Error::NoError;
	}
};

struct Data {
	uint64_t a;
	bool b;
	uint64_t c;

	template <class Serializer>
	Error serialize(Serializer& serializer) {
		return serializer(a, b, c);
	}
};

int main() {
	Data x{ 1, true, 2 }; 
	std::stringstream stream;

	Serializer serializer(stream);
	serializer.save(x);

	Data y{ 0, false, 0 };

	Deserializer deserializer(stream);
	const Error err = deserializer.load(y);

	assert(err == Error::NoError);

	assert(x.a == y.a);
	assert(x.b == y.b);
	assert(x.c == y.c);

	std::cout << "done.\n";
	return 0;
}
