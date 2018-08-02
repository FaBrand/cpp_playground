#include <iostream>
#include <mvc/view.h>

View::View(Model* model) : model_(model)
{
    if (model_)
    {
        connect(model_->on_new_received, [this]() {
            std::cout << "Now displaying last received data: " << model_->get_last_received() << std::endl;
        });
    }
}

// Slots -> Sinks on which the view reacts
void View::provide_to_outside()
{
    const auto data = model_->get_data();
    std::cout << "Providing new data:" << data << std::endl;
}
