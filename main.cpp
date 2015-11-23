#include "LectureDemo.h"
#include <string>
#include <iostream>
#include <vector>

void call(std::string&& str)
{
	std::cout << "Got rvalue ref of: " << str << std::endl;
}

void call(std::string& str)
{
	std::cout << "Got const ref of: " << str << std::endl;
}

class Movable
{
public:
	Movable(int size): size(size), contents(new float[size])
	{
		std::cout << "Constructing Movable \r\n";
	}
	~Movable()
	{
		delete contents;
	}

	Movable(const Movable& rhs): size(rhs.size), contents(new float[size])
	{
		std::copy(rhs.contents, rhs.contents + size, contents);
		std::cout << "Copying Movable \r\n";
	}
	Movable(Movable&& rhs): size(rhs.size), contents(rhs.contents)
	{
		rhs.contents = nullptr;
		std::cout << "Moving Movable \r\n";
	}
	Movable& operator =(Movable&& rhs)
	{
		contents = rhs.contents;
		rhs.contents = nullptr;
		size = rhs.size;
		std::cout << "Move-assigning Movable \r\n";
		return *this;
	}

	static Movable build(int size)
	{
		Movable movable(size);
		std::cout << "Building a new Movable of size " << movable.size << std::endl ;
		return movable;
	}

	float* contents; // assume heavy object
	int size;
};

int main()
{
	std::string name = "Hiro";

	call(name);
	call(name + ", hello!");

	call(std::move(name));
//	call(name); // ERROR! undefined behaviour invoked!!

	{
		Movable m(1000);
		m = Movable(1000);
		Movable m2 = m; // create a copy. Why?
		Movable m3 = Movable::build(1000); // use a factory method uses RVO
		Movable m4 = std::move(m); // move constructs
	}

	// create an instance of the demo class
	demo::LectureDemo demo;

	// kick-off the demo
	demo.run();

	return 0;
}
