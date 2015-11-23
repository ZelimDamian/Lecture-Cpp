//
// Created by Zelim on 02/11/2015.
//
#include "LectureDemo.h"

#include <iostream>
#include <tuple>
#include <vector>
#include <memory>
#include <assert.h>
#include <algorithm>
#include <functional>

using namespace demo;

namespace some
{
	namespace deep
	{
		namespace name_space
		{
			struct type { };
		}
	}
}

struct Person
{
	std::string name;
};

Person find(const std::string& name)
{
	return Person { name };
}

template <class T>
void write(const T& m)
{
	std::cout << m << std::endl;
}

class VariadicTemplates
{
public:
	template <typename ... Args>
	static auto make_tuple(Args&& ... args) -> std::tuple<Args...>
	{
		return std::tuple<Args...> { std::forward<Args>(args) ...};
	};

	template <typename ... Args>
	static void print( Args&& ... args)
	{
		const static int variadicPackSize = sizeof...(Args);

		// pass the arguments further
		make_tuple(args...);

		(void) (int[]) { (print_one(std::forward<Args>(args)), 0)... };
	}


private:
	template <typename T>
	static void print_one(const T& value)
	{
		std::cout << std::to_string(value) << std::endl;
	}
};

// function doesn't assume what the return type is and return different types based on argument types
template <class T, class S>
auto multiply(const T& a, const S& b) -> decltype(a * b)
{
	return a * b;
};

void isValid(char* ptr) { }
void isValid(int val) { }

template<typename Container, typename Index> // works, but
auto indexInto(Container& c, Index i) // requires
			-> decltype(c[i]) // refinement
{
	return c[i];
}


// dummy function that can eat anything
template <class T>
void eat(const T& food) {}

void work()
;

void findMinMaxAvg1(const std::vector<float>& values, float& min, float& max, float& avg)
;

auto findMinMaxAvg2(const std::vector<float>& values) -> std::tuple<float, float, float>
;

void LectureDemo::run()
{
	int x1; // potentially uninitialized
//	auto x2; // error! initializer required
	auto x3 = 0; // fine, x's value is well-defined

	// auto keyword usage
	auto x = 42; // decltype(x) is int
	auto y = 42.0f;
	auto z = 42.0;
	auto s = "Hello";
	auto c = 'C';
	auto bob = Person { "Bob" };
	auto alice = find("Alice");

	auto alicePtr = &alice;
	auto* aliceExpPtr = &alice; // identical

	auto& bobRef = bob;
	bobRef.name = "Peter";

	assert(bobRef.name == bob.name && bob.name == "Peter");

	// impose constness
	const auto* constBobPtr = &bob;
	const auto& constBobRef = bob;
	std::cout << constBobRef.name << std::endl;
//	bob = 2;


	{
		// curly brace initialization
		int i1 = 1;
		int i2 (2);
		int i3 {3}; // new
		int i4 = {4}; // new
	}


	// a simple variadic print
	VariadicTemplates::print(1000L, 0, 0.1f);

	// a variadic factory function + note there is no type specified at all
	auto tuple = VariadicTemplates::make_tuple(1, 2, "hello", 'c', 3.1415926535f);

	// use of decltype to find out the type of the implicitly typed variable
	using Tuple = decltype(tuple);
	static const size_t LAST = std::tuple_size<Tuple>::value - 1;
	// note that we could use constexpr instead but VS2013 doesn't support it
//	constexpr size_t LAST = std::tuple_size<Tuple>::value - 1;

	// access the last element
	std::cout << std::get<LAST>(tuple) << "\n";

	std::vector<std::string> names;

	std::vector<std::string>::const_reverse_iterator itor1 = names.crbegin();
	auto itor2 = names.crbegin();
	decltype(names.crbegin()) itor3 = names.crbegin();

	typedef std::vector<some::deep::name_space::type> DeepTypeTF;

	using DeepTypeU = std::vector<some::deep::name_space::type>;

	DeepTypeTF values1;
	DeepTypeU values2;

	{
		auto x1 = 10000000000.0;
		auto x2 = 10000000000.0;

//		int i1 { x1 * x2 };
		int i2 = x1 * x2;

//		printf("%i", i1);
		printf("%i", i2);
	}

	// type alias used to hide a template parameter
	std::vector<some::deep::name_space::type> unqiueSortedValues;

	std::vector<some::deep::name_space::type>::const_iterator itor;

	for (itor = unqiueSortedValues.begin(); itor != unqiueSortedValues.end(); ++itor)
	{
		eat(*itor);
	}

	for (auto& item : unqiueSortedValues)
	{
		eat(item);
	}

	struct Address
	{
		int no { 0 };
		std::string street { "Unknown" };
	};

	struct House
	{
		Address address;
	};

	House house(Address());

	work();
}
//////////////////////////////////////////////////////////////////////////////
////// override and final
//////////////////////////////////////////////////////////////////////////////

class Parent
{
public:
	virtual void cry() { write("Parent is crying"); }
	virtual void work() { write("Parent is working"); }
};

class Child: public Parent
{
public:
//	void cri() override { write("Child is crying"); }
	void cry() override { write("Child is crying"); }
	void work() final { write("Child is working"); }
};

class GrandChild: public Child
{
	void cry() override { write("GrandChild is crying"); }
//	void work() override { write("Child is working"); }
};

//
//////////////////////////////////////////////////////////////////////////////
////// lamdas
//////////////////////////////////////////////////////////////////////////////

class Functor
{
public:
	Functor (int x) : _x( x ) {}
	int operator() (int y) { return _x + y; }
private:
	int _x;
};

void work()
{
	int x = 5;

	{
		// function object

		Functor addFive( x );
		std::cout << addFive(6) << std::endl; // prints 11
	}
	{
		// equivalent lambda
		auto addFive = [x] (int y) { return x + y; };
		std::cout << addFive(6) << std::endl; // prints 11
	}
	{
		// using lamdas with STL

		int maxLength = 6;
		int minLength = 3;

		// before C++11
		{
			std::vector<std::string> names;
			names.push_back("Margo");
			names.push_back("Edith");
			names.push_back("Agness");
			names.push_back("Gru");
			names.push_back("Lucy");
		}

		std::vector<std::string> names = { "Margo", "Edith", "Agness", "Gru", "Lucy" };



		auto count = std::count_if(names.begin(), names.end(),[maxLength, minLength] (const std::string& name) {
			return name.length() < maxLength && name.length() > minLength;
		});

		auto found = std::find_if(names.begin(), names.end(),[maxLength, minLength] (const std::string& name) {
			return name.length() < maxLength && name.length() > minLength;
		});

		if(found != names.end()) { write("Found name!"); }

	}
	{
		// dangling reference capture

		std::function<void()> f;
		{
			std::string local = "Joe";

			f = [&local]
			{
				write(local);
			};
		}
//		f(); // error!!! dangling ref in lambda closure
	}
	{
		// mutability

		int ref = 1;
		int value = 1;

		auto lamda = [&ref, value] ()
		{
//			value = 42;
			ref = 42;
		};

		auto mutLamda = [&ref, value] () mutable
		{
			value = 42;
			ref = 42;
		};

		lamda();
		mutLamda();

		write(value);
		write(ref);
	}
	{
		// constexpr
		constexpr float x = 42.0;
		constexpr float y{108};
		constexpr float z = exp(5.3);
//		constexpr int i; // Error! Not initialized
		int j = 0;
//		constexpr int k = j + 1; //Error! j not a constant expression

		// Recursive constexpr function
//		constexpr int fac(int n)
//		{
//			return n == 1 ? 1 : n*fac(n - 1);
//		}

		// Recursive constexpr function
		class Bar
		{
		public:
			constexpr explicit Bar(int i) : _i(i) {}
			constexpr int value()
			{
				return _i;
			}
		private:
			int _i;
		};

		constexpr Bar bar(4);
		constexpr int val = bar.value();
	}

	/////////////////////////////////////////////////////////
	/// Libarry features
	/////////////////////////////////////////////////////////
	{
		// library features: std::array
		std::array<float, 5> a { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };

		a[3] = 1; // no bounds check

		try {
			a.at(6) = 1;
		}
		catch(...)
		{
			write("Tried to write to out of bounds");
		}

		for (int i = 0; i < a.size(); ++i)
		{
			write(a[i]);
		}
	}
	{
		// std::tuple + std::tie
		float min, max, avg;
		std::vector<float> v { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
		// old way
		{
			findMinMaxAvg1(v, min, max, avg);
			VariadicTemplates::print(min, max, avg);
		}
		// new way
		{
			std::tie(min, max, avg) = findMinMaxAvg2(v);
			VariadicTemplates::print(min, max, avg);
		}
	}
	{
		// smart pointers


		struct Person
		{
			std::string name;
		};

		// old way
		{
			Person* person = new Person { "Mr. Dynamic" };
			delete person; // must not forget to delete!!!

			struct Factory
			{
				static float* create() { return new float[1000]; }
			};

			float* data = Factory::create(); // who should delete data? what if implementation is hidden?
			delete data; // must not forget to delete!!!
		}
		// using unique_ptr
		{

			// can use aliases
			using PersonPtr = std::unique_ptr<Person>;

			struct Factory
			{
				static PersonPtr create() { return std::unique_ptr<Person>(new Person { "New person" }); }
			};

			struct PersonConsumer
			{
				static void consume(PersonPtr person) { /* consume person */ }
			};

			auto person = Factory::create(); // person has exclusive ownership of data
			// no need to delete as person will do that on scope exit
//			auto person2 = person; // FAIL! unique_ptr is not copyable. Why?
			auto person3 = std::move(person); // FAIL! unique_ptr is not copyable
			PersonConsumer::consume(person3); // can't pass it as argument either
		}
		// using shared_ptr
		{
			using PersonPtr = std::shared_ptr<Person>;

			struct Factory
			{
				static PersonPtr create() { return std::make_shared<Person>("New person"); }
			};

			struct PersonConsumer
			{
				static void consume(PersonPtr person) { /* consume person */ }
			};

			struct PersonStorer
			{
				PersonPtr person;
			};

			auto person = Factory::create(); // Ref count = 1; person has shared ownership of data
			auto person2 = person; // Ref count = 2; can copy which will increment ref count
			auto person3 = std::move(person); // Ref count = 2; can move doesn't touch ref count
			PersonConsumer::consume(person3); // Ref count = 3 and then 2; can pass wherever we want
			PersonStorer jail { person }; // Ref count = 3

			// no need to delete as person will do that on scope exit
			// person, person2, person3 and jail are destructed here, which makes Ref count = 0
		}
	}
}

void findMinMaxAvg1(const std::vector<float>& values, float& min, float& max, float& avg)
{
	min = *std::min_element(values.begin(), values.end());
	max = *std::max_element(values.begin(), values.end());
	avg = (std::accumulate(values.begin(), values.end(), 0.0f)) / values.size();
}

auto findMinMaxAvg2(const std::vector<float>& values) -> std::tuple<float, float, float>
{
	return std::make_tuple(
			*std::min_element(values.begin(), values.end()),
			*std::max_element(values.begin(), values.end()),
			(std::accumulate(values.begin(), values.end(), 0.0f)) / values.size()
	);
}