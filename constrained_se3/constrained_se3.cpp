// TODO: Make the check static

#include <assert.h>
#include <iostream>
#include <string_view>
#include <vector>

constexpr bool strings_equal(char const* a, char const* b) {
    return std::string_view(a) == b;
}

struct SE3 {
    SE3(double value, const std::string& goal_frame,
        const std::string& origin_frame) {
        value_ = value;
        goal_frame_ = goal_frame;
        origin_frame_ = origin_frame;
    };

    double value_{0};
    std::string goal_frame_{"undefined"};
    std::string origin_frame_{"undefined"};
};

struct Position {
    Position(double value, const std::string& frame) {
        value_ = value;
        frame_ = frame;
    };

    std::string frame_{"undefined"};
    double value_{0};
};

double operator*(const SE3& se3, const Position& pos) {
    assert(se3.origin_frame_ == pos.frame_);
    return se3.value_ * pos.value_;
}

int main() {
    SE3 w_T_b(1, "w", "b");
    Position p(2, "w");

    std::cout << w_T_b * p << std::endl;

    return 0;
}
