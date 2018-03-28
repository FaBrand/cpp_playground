#include "client.h"
#include "subject.h"

int main()
{
    Subject data_provider;
    Client subject_user;

    subject_user.Subscribe(data_provider);

    data_provider.UpdateSubjectWith(LowLevelInput{1, 2.5f});

    subject_user.CancelSubscription(data_provider);

    data_provider.UpdateSubjectWith(LowLevelInput{1, 2.5f});

    return 0;
}
