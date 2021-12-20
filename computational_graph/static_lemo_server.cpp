// TODO:
// - write base class

class MotorControllerNode {
    BusSignals bus_signals;
    Node position_wrapped;
    Node position;
    Node velocity;
    Node vel_filt;
    Node joint_position;
};

int main() {
    MotorControllerNode mc_node;

    bus_signals.motor_position() >> motor_position_wrapped >> motor_position >>
        motor_velocity >> motor_vel_filt;
    motor_position >> motor_output_position;
    mc_node_joint_position() >> position;
}
