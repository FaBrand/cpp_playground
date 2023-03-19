
class BusSignal {
    std::string node_name = "";
};

class Node {
    std::string node_name = "";
}

// cfg.read("scale", scale_);
// cfg.read("offset", offset_);
// void LinearInput::update() {
//     value_ = (input_->getValue() - offset_) * scale_ + bias_;
// }

static
operator>>(const Node& input) {
    std::cout << "input.node_name:" << input.node_name << std::endl;
}

int main() {
    BusSignal motor_position;
    Node motor_position_wrapped;
    Node motor_position;
    Node motor_velocity;
    Node motor_vel_filt;

    motor_position >> motor_position_wrapped >> motor_position >>
        motor_velocity >> motor_vel_filt;
}
