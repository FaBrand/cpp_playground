#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <signals/signal.h>

class Model
{
  public:
    int get_data();
    int get_last_received();
    void set_data(int);
    void set_last_received(int);

    Signal<> on_new_received;

  private:
    int data_{};
    int last_received_{};
};

#endif /* MODEL_H */
