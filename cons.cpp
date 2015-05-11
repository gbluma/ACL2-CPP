// © 2005 Karl A. Knechtel (Zahlman)
// This code may be used and modified freely as long as appropriate credit is given.
// origin: http://www.gamedev.net/topic/329912-statically-typed-lisp-style-cons-lists-in-c-o/

// Modified © 2015 Garrett J. Bluma
// This code may be used and modified freely as long as appropriate credit is given.

#include <utility>
#include <iostream>
#include <string>

struct _ {};
    
template <typename car_t, typename cdr_t>
  class conslist
  {
    const std::pair<const car_t, const cdr_t> storage;
    public:
      conslist(const car_t& x, const cdr_t& y) : storage(std::make_pair(x, y)) {}
      const car_t car() const { return storage.first; }
      const cdr_t cdr() const { return storage.second; }
      friend std::ostream& operator<<(std::ostream& os, const conslist<_,_>& l);
  };

template <>
  class conslist<_, _>
  {
    public:
      conslist<_, _> car() const { return *this; }
      conslist<_, _> cdr() const { return *this; }
      friend std::ostream& operator<<(std::ostream& os, const conslist<_,_>& l); 
  };

typedef conslist<_, _> cnil;

template <typename car_t>
  conslist<car_t, cnil>
  snoc(const car_t& last)
  {
    return conslist<car_t, cnil>(last, cnil());
  }

// Cons an item onto the list. The operator += is used rather than + because
// it has the needed associativity (even though the semantics are completely
// different from what += normally implies).

template <typename car_t, typename cdar_t, typename cddr_t>
  const conslist<car_t, conslist<cdar_t, cddr_t>>
  cons (const car_t& x, const conslist<cdar_t, cddr_t>& y)
  {
    return conslist<car_t, conslist<cdar_t, cddr_t> >(x, y);
  }

template <typename car_t, typename cdar_t>
  const car_t car ( const conslist<car_t, cdar_t>& y)
  {
    return y.car();
  }

template <typename car_t, typename cdar_t>
  const cdar_t cdr ( const conslist<car_t, cdar_t>& y)
  {
    return y.cdr();
  }


// An example of writing a 'varargs' function using the above framework
template <typename car_t, typename cdr_t>
  void prn(const conslist<car_t, cdr_t>& l, std::string space = "")
  {
    // TODO: introspection to ensure that T is a conslist<A, B> for some A and B
    std::cout << space << l.car();
    prn(l.cdr(), " ");
  }

template <typename car_t>
  void prn(const conslist<car_t,cnil>& n, std::string space)
  {
    std::cout << space << n.car() << std::endl;
  }

template <typename T>
void prn(const T& t)
{
std::cout << t << std::endl;
}


// Define endp. If list has contents, return true. If at last item, return true;
template <typename T> bool endp      (const T& l)    { return false; }
template <>           bool endp<cnil>(const cnil& n) { return true; }

std::ostream& operator<<(std::ostream& os, const conslist<_,_>& l)
{
  prn(l); // HAH as if this would work. // linker error
  return os;
}
  
