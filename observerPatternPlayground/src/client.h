#ifndef CLIENT_H
#define CLIENT_H

#include "observer.h"
#include "subject.h"

class Client : public Observer
{
  public:
    Client() = default;
    Client(const Client&) = default;
    Client(Client&&) = default;
    Client& operator=(const Client&) = default;
    Client& operator=(Client&&) = default;
    ~Client();

    void Subscribe(Subject& model);
    void CancelSubscription(Subject& model);

    void update(LowLevelInput const& data) override;

  private:
    Subject* subject_{nullptr};
};

#endif /* CLIENT_H */
