#include <mvc/model.h>

void Model::set_data(int new_data)
{
    data_ = new_data;
}

int Model::get_data()
{
    return data_;
}

void Model::set_last_received(int new_data)
{
    last_received_ = new_data;
    on_new_received();
}

int Model::get_last_received()
{
    return last_received_;
}
