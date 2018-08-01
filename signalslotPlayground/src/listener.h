#ifndef LISTENER_H
#define LISTENER_H

#include <iostream>
#include "topic.h"

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

    void slot(topic const& concrete_topic)
    {
        std::cout << "Called topic slot:"<< concrete_topic.some_data << std::endl;
    }
};

#endif /* LISTENER_H */
