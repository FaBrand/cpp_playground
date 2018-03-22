#include <iostream>
#include <vector>

class Drawables
{
  public:
    explicit Drawables(int shape)
    {
        int_shape_ = shape;
    }
    Drawables(std::string shape)
    {
        string_shape_ = shape;
    }

  private:
    int int_shape_;
    std::string string_shape_;
};

void Draw(Drawables shape)
{
    std::cout << shape << '\n';
}

void Draw(std::vector<Drawables> const& shapes)
{
    for (auto const& shape : shapes)
    {
        Draw(shape);
    }
}

int main()
{
    std::vector<Drawables> shapes;
    shapes.push_back(Drawables{0});
    shapes.push_back(Drawables{1});
    // shapes.push_back(2.0);
    // shapes.push_back("two");
    Draw(shapes);
}
