#include <iostream>

#include "free_strategies.h"

void FreeStrategyA(const std::string& data)
{
    std::cout << "I print " << data << " in the way A!" << std::endl;
}

void FreeStrategyB(const std::string& data)
{
    std::cout << "I print " << data << " in the way B!" << std::endl;
}
