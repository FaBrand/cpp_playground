#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <mvc/model.h>
#include <mvc/view.h>

class Controller
{
  public:
    Controller(Model*, View*);

    void update_view();
    void set_data_5();
    void set_data_42();

  private:
    Model* model_{nullptr};
    View* view_{nullptr};

    Signal<int> set_data{};

    void handle_received(int);
};

#endif /* CONTROLLER_H */
