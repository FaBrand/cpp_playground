#include <memory>

#include <mvc/controller.h>
#include <mvc/model.h>
#include <mvc/view.h>

#include <iostream>

int main(int, char**)
{
    // Since the model and the view are handled as pointers, they could easily be replaced by
    // e.g. Composite or Proxy objects to extend their functionality.
    // For example the composite pattern can be used to plug multiple views into the same controller without changing
    // the controller itself
    //
    // The proxy pattern could be used to have a cached model for example if it would rely on database access for
    // example.
    auto model = std::make_unique<Model>();
    auto view = std::make_unique<View>(model.get());

    auto controller = std::make_unique<Controller>(model.get(), view.get());

    controller->set_data_5();
    controller->set_data_42();

    // Fake event from view, would normally be raised from within the view
    // e.g. by user input
    // This in term then notifies the controller that there is new received data from the outside.
    // The controller then updates the model accordingly with the data it got from the view.
    // The model then notifies the view to update itself because the changed data needs to be displayed.
    // I'm not sure whether this should rather be in the responsibility of the controller.
    view->on_received(84);

    return 0;
}
