#include <iostream>
#include <vector>

class Member
{
  public:
    Member(int val) : val_(val) {}
    Member() = default;
    Member(const Member&) = default;
    Member(Member&&) = default;
    Member& operator=(const Member&) = default;
    Member& operator=(Member&&) = default;

  private:
    int val_;
};

class AClass
{
  public:
    AClass()
    {
        std::cout << "  Default ctor" << std::endl;
    }

    explicit AClass(int val) : member(val)
    {
        std::cout << "  Regular ctor" << std::endl;
    }

    AClass(const AClass& val) : member(val.member)
    {
        std::cout << "  Copy ctor" << std::endl;
    }

    AClass(AClass&& val) : member(std::move(val.member))
    {
        std::cout << "  Move ctor" << std::endl;
    }

    AClass& operator=(const AClass& other)
    {
        std::cout << "  Assignment op" << std::endl;
        member = other.member;
        return *this;
    }

    AClass& operator=(AClass&& other)
    {
        std::cout << "  Move Assignment op" << std::endl;
        member = std::move(other.member);
        return *this;
    }

  private:
    Member member;
};

AClass CreateAClass()
{
    AClass a{42};
    return a;
}

#define RUN_EXAMPLE(description, command) \
    {                                     \
        std::cout << description << '\n'; \
        command;                          \
        std::cout << '\n';                \
    }

using AVec = std::vector<AClass>;

int main()
{

    RUN_EXAMPLE("Regular ctor", AClass foo{42});

    RUN_EXAMPLE("Copying", AClass foo{}; AClass bar{foo});

    RUN_EXAMPLE("Copying", AClass foo{}; AClass bar = foo;);

    RUN_EXAMPLE("Copy elision", AClass foo{AClass{42}});

    RUN_EXAMPLE("Copy elision", AClass foo = AClass{42});

    RUN_EXAMPLE("Create function: Assignment", AClass foo = CreateAClass(););

    RUN_EXAMPLE("Create function: Direct init", AClass foo{CreateAClass()};);

    RUN_EXAMPLE("Move Construction", AClass foo{std::move(AClass{})};);

    RUN_EXAMPLE("Assignment operation", AClass foo{}; AClass bar{}; foo = bar;);

    RUN_EXAMPLE("Move assignment", AClass foo{}; foo = AClass{};);

    RUN_EXAMPLE("Emplace back", AVec x; x.emplace_back(15););

    RUN_EXAMPLE("Emplace back", AVec x; x.emplace_back(AClass{}););

    RUN_EXAMPLE("Push back", AVec x; x.push_back(AClass{}););

    RUN_EXAMPLE("Dynamically resizing a vector", {
        AVec x;
        for (int i = 0; i < 3; ++i)
        {
            std::cout << "  Push back #" << i << '\n';
            std::cout << "    Size/Capacity " << x.size() << '/' << x.capacity() << '\n';
            x.push_back(AClass(16));
        }
    });

    RUN_EXAMPLE("Dynamically resizing a vector with reserved elements", {
        AVec x;
        x.reserve(3);
        for (int i = 0; i < 5; ++i)
        {
            std::cout << "  Push back #" << i << '\n';
            std::cout << "    Size/Capacity " << x.size() << '/' << x.capacity() << '\n';
            x.push_back(AClass(16));
        }
    });

    return 0;
}
