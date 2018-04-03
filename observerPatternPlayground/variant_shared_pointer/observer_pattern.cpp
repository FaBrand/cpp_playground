#include "concrete_observer.h"
#include "low_level_input.h"
#include "subject.h"

int main()
{
    auto outer_user{std::make_shared<ConcreteObserver>()};

    {
        Subject data_provider{};
        data_provider(LowLevelInput{1, 2.5f});

        outer_user->Subscribe(data_provider);

        auto parallel_user{std::make_shared<ConcreteObserver>()};
        parallel_user->Subscribe(data_provider);

        {
            auto inner_user{std::make_shared<ConcreteObserver>()};

            inner_user->Subscribe(data_provider);

            data_provider(LowLevelInput{1, 2.5f});

            std::cout << "Here the scope is left where client is meant to be alive" << '\n';
        }

        data_provider(LowLevelInput{2, 3.5f});
    }

    return 0;
}
