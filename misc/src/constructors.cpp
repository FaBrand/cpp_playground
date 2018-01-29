#include <iostream>
#include <vector>

class Member
{
  public:
    Member(int val) : val_(val)
    {
    }

    // Member(const Member&) = delete;
    // Member(Member&&) = delete;
    // Member& operator=(const Member&) = delete;
    // Member& operator=(Member&&) = delete;

    int val_;
};

class AClass
{
  public:
    AClass(const int val) : member(val)
    {
        std::cout << "  Regular constructor" << std::endl;
    }

    AClass(const AClass& val) : member(val.member)
    {
        std::cout << "  Copy constructor" << std::endl;
    }

    AClass(AClass&& val) : member(std::move(val.member))
    {
        std::cout << "  Move ctor" << std::endl;
    }

    AClass& operator=(const AClass& other)
    {
        std::cout << "  Assignment" << std::endl;
        member = other.member;
        return *this;
    }

    AClass& operator=(AClass&& other)
    {
        std::cout << "  Move Assignment" << std::endl;

        member = std::move(other.member);
        return *this;
    }

    int Get() const
    {
        return member.val_;
    }

  private:
    Member member;
};

int main()
{
    std::cout << "Regular ctor" << std::endl;
    AClass foo{42};  // Default constructor

    std::cout << "Copying" << std::endl;
    AClass copy_ctor{foo};  // Copy constructor

    std::cout << "Copying" << std::endl;
    AClass also_copy_ctor = foo;  // Also Copy constructor

    std::cout << "Copy elision" << std::endl;
    AClass move_ctor{AClass(42 * 2)};  // Copy elision

    std::cout << "Moving" << std::endl;
    AClass anothother_move_ctor{std::move(AClass(42 * 2))};  // Move constructor

    std::cout << "Assignment" << std::endl;
    foo = also_copy_ctor;

    std::cout << "Move Assignment" << std::endl;
    foo = AClass(42 * 3);

    std::vector<AClass> x;
    std::cout << "Emplace back" << std::endl;
    x.emplace_back(AClass(15));

    std::cout << "Push back" << std::endl;
    std::vector<AClass> y;
    y.push_back(AClass(15));

    std::cout << "Dynamically resizing a vector" << std::endl;
    std::vector<AClass> z;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Push back #" << i << '\n';
        std::cout << "Size/Capacity " << z.size() << '/' << z.capacity() << '\n';
        z.push_back(AClass(16));
    }

    std::cout << "Dynamically resizing a vector with reserved elements" << std::endl;
    std::vector<AClass> xy;
    xy.reserve(9);
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Push back #" << i << '\n';
        std::cout << "Size/Capacity " << xy.size() << '/' << xy.capacity() << '\n';
        xy.push_back(AClass(16));
    }
    return 0;
}
