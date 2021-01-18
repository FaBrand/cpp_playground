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
    SO3(double value) { data = value; };

    double data{0};
    GoalFrame goal_frame_;
    OriginFrame origin_frame_;
};

template <typename GoalFrame, typename OriginFrame>
struct SE3 {
    SE3(double value) { data = value; };

    double data{0};
    GoalFrame goal_frame_;
    OriginFrame origin_frame_;
};

template <typename Frame>
struct Euclidian {
    Euclidian(double value) { data = value; };

    std::variant<World, Base, Control> frame;
    double data{0};
};

template <typename GoalFrame, typename Frame>
void Rotate(Euclidian<Frame>& vec, const SO3<GoalFrame, Frame>& rot){
    vec.data += rot.data;
    vec.frame = GoalFrame{};
}

template <typename T1, typename T2>
Euclidian<T1> operator*(const SE3<T1, T2>& se3, const Euclidian<T2>& pos) {
    Euclidian<T1> new_pos{.data = se3.data*pos.data};
    return new_pos;
}

template <typename T1, typename T2, typename T3>
SE3<T1, T3> operator*(const SE3<T1, T2>& M1, const SE3<T2, T3> M2) {
    SE3<T1, T3> new_se3{.data = M1.data*M2.data};
    return new_se3;
}

int main() {
    SE3<World, Base> w_T_b(1);
    Euclidian<Base> p_b(2);
    std::cout << (w_T_b * p_b).data << std::endl;

    SE3<Base, Foot> b_T_f(3);
    std::cout << (w_T_b * b_T_f).data << std::endl;

    SO3<Control, Base> yaw_d_R_b(4);
    Rotate(p_b, yaw_d_R_b);
    std::cout << p_b.data << std::endl;

    return 0;
}
