#include <assert.h>
#include <iostream>
#include <string_view>
#include <type_traits>
#include <vector>
#include <variant>

struct Base {};
struct Foot {};
struct World {};
struct Control {};


template <typename GoalFrame, typename OriginFrame>
struct SO3 {
    SO3(double value) { value_ = value; };

    double value_{0};
    GoalFrame goal_frame_;
    OriginFrame origin_frame_;
};

template <typename GoalFrame, typename OriginFrame>
struct SE3 {
    SE3(double value) { value_ = value; };

    double value_{0};
    GoalFrame goal_frame_;
    OriginFrame origin_frame_;
};

template <typename Frame>
struct Position {
    Position(double value) { value_ = value; };

    template <typename GoalFrame>
    void Rotate(const SO3<GoalFrame, Frame>& rot){
        value_ += rot.value_;
        frame_ = GoalFrame{};
    }

    std::variant<World, Base, Control> frame_;
    double value_{0};
};

template <typename T1, typename T2>
Position<T1> operator*(const SE3<T1, T2>& se3, const Position<T2>& pos) {

    Position<T1> new_pos{.value_ = se3.value_*pos.value_};
    return new_pos;
}

template <typename T1, typename T2, typename T3>
SE3<T1, T3> operator*(const SE3<T1, T2>& M1, const SE3<T2, T3> M2) {
    SE3<T1, T3> new_se3{.value_ = M1.value_*M2.value_};
    return new_se3;
}

int main() {
    SE3<World, Base> w_T_b(1);
    SO3<Control, Base> c_R_b(1);
    SE3<Base, Foot> b_T_f(3);
    Position<Base> p_b(2);

    std::cout << (w_T_b * p_b).value_ << std::endl;
    std::cout << (w_T_b * b_T_f).value_ << std::endl;
    p_b.Rotate(c_R_b);
    std::cout << p_b.value_ << std::endl;

    return 0;
}
