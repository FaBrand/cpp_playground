#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
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

    void Subscribe(std::shared_ptr<Subject> model);
    void CancelSubscription();

    void update(LowLevelInput const& data) override;

  private:
    std::weak_ptr<Subject> subject_{nullptr};
};

#endif /* CLIENT_H */
