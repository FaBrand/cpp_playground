#include <iostream>
#include <mvc/controller.h>

Controller::Controller(Model* model, View* view) : model_(model), view_(view)
{
    if (view_)
    {
        connect(view_->on_received, [this](int data) { handle_received(data); });
    }

    if (model_ && view_)
    {
        // set_data does not have to be signal
        // A member function would be sufficient
        connect(set_data, [this](auto data) {
            model_->set_data(data);
            update_view();
        });
    }
}

void Controller::handle_received(int data)
{
    std::cout << "Handled received: " << data << std::endl;
    model_->set_last_received(data);
}

void Controller::update_view()
{
    if (view_)
    {
        view_->provide_to_outside();
    }
}

void Controller::set_data_5()
{
    set_data(5);
}

void Controller::set_data_42()
{
    set_data(42);
}
