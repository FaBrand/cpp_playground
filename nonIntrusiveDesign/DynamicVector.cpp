#include <iostream>
#include <vector>

void Draw(int shape)
{
    std::cout << shape << '\n';
}

void Draw(std::vector<int> const& shapes)
{
    for (auto const& shape : shapes)
    {
        Draw(shape);
    }
}

int main()
{
    std::vector<int> shapes;
    shapes.push_back(0);
    shapes.push_back(1);
    shapes.push_back("two");
    Draw(shapes);
}
