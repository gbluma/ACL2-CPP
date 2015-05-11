
#include <iostream>
#include <vector>

#define FUNDAMENTAL_TYPES\
    X(bool)\
    X(signed char)\
    X(unsigned char)\
    X(char)\
    X(wchar_t)\
    X(char16_t)\
    X(char32_t)\
    X(short)\
    X(unsigned short)\
    X(int)\
    X(unsigned)\
    X(long)\
    X(unsigned long)\
    X(long long)\
    X(unsigned long long)

// generic monoid
template<class T>
struct monoid {
    // T empty()
    // T append(T,T)
    static constexpr bool is_instance = false;
};



template<class T>
struct default_monoid
{
    static T empty(){return T{};}
    static T append(const T& lhs, const T& rhs){return lhs+rhs;}
    static constexpr bool is_instance = true;
};
 
// make all fundamental types monoids
#define X(a)\
    template<>\
    struct monoid<a> : public default_monoid<a>\
    {};
FUNDAMENTAL_TYPES;
#undef X

template<class T>
struct monoid<std::vector<T>>
{
    static std::vector<T> empty(){return std::vector<T>{};}
 
    static std::vector<T> append(std::vector<T> lhs, const std::vector<T>& rhs)
    {
        for (const T& t : rhs)
            lhs.push_back(t);
 
        return lhs;
    }
 
    static constexpr bool is_instance = true;
};


// make strings monoids
template<>
struct monoid<std::string> : public default_monoid<std::string>
{};

template< 
    class T,
    class = typename std::enable_if<monoid<T>::is_instance>::type
    >
T accumulator(const std::vector<T>& in)
{
    T out{monoid<T>::empty()};
 
    for (const T& t : in)
        out = monoid<T>::append(out,t);
 
    return out;
}

// (monoid/+ [int 1 2 3])
// (monoid/+ [std::string "Hello " "There"])
// (monoid/+ [std::vector<int> [int 1 2 3] [int 4 5 6]])

// (defmonoid monoid/* int 1 (*))

// [int 1 2 3] -> std::vector<int>{1,2,3}
// c:/MinGW/lib/gcc/mingw32/4.8.1/include/c++\cmath:44

