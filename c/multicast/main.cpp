#include <iostream>
#include <algorithm>
//using namespace std;

struct Foo
{
    Foo()      { std::cout << "Foo::Foo\n";  }
    ~Foo()     { std::cout << "Foo::~Foo\n"; }
    void bar() { std::cout << "Foo::bar\n";  }
};

void f(const Foo &)
{
    std::cout << "f(const Foo&)\n";
}

void check(std::string& s1, std::string& s2)
{
    auto result = std::mismatch(s2.begin(), s2.end(), s1.begin());

    std::cout << "'" << s1 << "' starts with '" << s2 << "'";
    if (*result.first == '\0')
    {
        std::cout << " OK\n";
    }
    else
    {
        std::cout << " FAIL!\n";
    }
}

void apply(std::unique_ptr<int> pi)
{
    std::cout << "int: " << *pi << std::endl;
}

void apply(std::unique_ptr<float> pi)
{
    std::cout << "int: " << *pi << std::endl;
}

template <typename T>
std::unique_ptr<T> parse(std::string& s1)
{
    if (s1 == "1.0")
    {
        float* f = new float{1.0};
        return std::unique_ptr<float>{f};
    }
    else
    {
        int* i = new int{1};
        return std::unique_ptr<int>{i};
    }
}

int main()
{
//    std::unique_ptr<Foo> p1(new Foo);
//    std::unique_ptr<Foo> p2(std::move(p1));
//
//    p2->bar();
    Foo f;

    f.bar();

    std::string a1 = "aeron:udp";
    std::string a2 = "aeron:";

    check(a1, a2);

    std::string b1 = "aeronf:udp";
    std::string b2 = "aeron:";

    check(b1, b2);

    std::string c1 = "aeron";
    std::string c2 = "aeron:";

    check(c1, c2);

    std::string d1 = "aeron:";
    std::string d2 = "aeron:";

    check(d1, d2);

    std::string e1;

    e1.append("a");
    e1.append("b");
    e1.append("c");

    std::cout << e1 << '\n';

    std::string s = "1.0";

//    apply(parse(s));

    return 0;
}