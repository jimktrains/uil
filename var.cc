#include <iostream>
#include <typeinfo>
#include <type_traits>

template< typename... CONDITIONS >
struct all_false;

template<>
struct all_false<>
{
    const static bool value = true;
};

template< typename CONDITION, typename... CONDITIONS >
struct all_false< CONDITION, CONDITIONS... >
{
    const static bool value = !CONDITION::value && all_false<CONDITIONS...>::value;
};

struct Thing 
{
    template<typename ...ExcludedTypes> 
    struct Proxy 
    {
        Thing *p; 
        
			template<typename T>
			using NoDuplicates =
            typename std::enable_if< 
                all_false< std::is_same<ExcludedTypes, T>... >::value
                >::type;

      template<typename T, NoDuplicates<std::decay<T>>* = nullptr>
      Proxy<ExcludedTypes..., std::decay<T>> func(T &&t)
      {
          p->func(std::forward<T>(t));
          return {p};
      }
    };

    template<typename T>
    Proxy<std::decay<T>> func(T &&t)
    { 
        /* do something */ 
        return {this}; 
    }
};

class A {};

class B : public A {};

class C : public A {};

int main()
{
	Thing t = Thing();

	auto x = t.func(1);
	std::cout << typeid(x).name() << std::endl;

	auto y = x.func(t);
	std::cout << typeid(y).name() << std::endl;

	auto z = y.func("this is a test");
	std::cout << typeid(z).name() << std::endl;

	auto a = z.func("testing!   ");
	std::cout << typeid(a).name() << std::endl;

	auto b = a.func(1.5);
	std::cout << typeid(b).name() << std::endl;
}
