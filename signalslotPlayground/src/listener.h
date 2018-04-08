#ifndef LISTENER_H
#define LISTENER_H

#include <iostream>

class Listener
{
  public:
    void slot1()
    {
        std::cout << "Called slot1" << '\n';
    }

    void slot2(std::string const& text)
    {
        std::cout << "Called slot2 with: " << text << '\n';
    }
};

#endif /* LISTENER_H */
