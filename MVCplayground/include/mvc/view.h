#ifndef VIEW_H
#define VIEW_H

#include <signals/signal.h>
#include "model.h"

class View
{
  public:
    explicit View(Model* model);

    // Slots -> Sinks on which the view reacts
    void provide_to_outside();

    // Signals -> Events that can be raised by the view.
    // This should be connected with the controller
    Signal<int> on_received;

  protected:
    Model* model_{nullptr};
};

#endif /* VIEW_H */
