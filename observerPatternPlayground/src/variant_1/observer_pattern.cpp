#include "client.h"
#include "subject.h"

int main()
{
    auto data_provider{std::make_shared<Subject>()};
    data_provider->UpdateSubjectWith(LowLevelInput{1, 2.5f});

    {
        auto subject_user{std::make_shared<Client>()};

        subject_user->Subscribe(data_provider);

        data_provider->UpdateSubjectWith(LowLevelInput{1, 2.5f});

        std::cout << "Here the scope is left where client is meant to be alive" << '\n';
    }

    data_provider->UpdateSubjectWith(LowLevelInput{1, 2.5f});

    return 0;
}
