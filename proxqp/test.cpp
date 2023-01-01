#include <Eigen/Dense>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

auto solveLagrangian(const Eigen::MatrixXd& H,  //
                     const Eigen::VectorXd& g,  //
                     const Eigen::MatrixXd& A,  //
                     const Eigen::VectorXd& b   //
                     ) -> Eigen::VectorXd {
    constexpr double mue = 0.1;
    Eigen::Vector2d x = Eigen::Vector2d::Zero();
    Eigen::Vector2d y = Eigen::Vector2d::Zero();

    for (size_t i = 0; i < 10; i++) {
        Eigen::MatrixXd Ha = H + 1.0 / mue * A.transpose() * A;
        Eigen::VectorXd ga = g + A.transpose() * (y - mue * b);
        x = -Ha.inverse() * ga;
        y = y + 1.0 / mue * (A * x - b);
    }
    return x;
}

TEST_CASE("Simple Least Squares", "[ProxQP]") {
    Eigen::Matrix2d H;
    H << 1, 0, 0, 1;
    Eigen::Vector2d g;
    g << 0, 1;
    Eigen::Matrix2d A;
    A << 1, 0, 0, 1;
    Eigen::Vector2d b;
    b << 0, -1;

    Eigen::VectorXd result = solveLagrangian(H, g, A, b);

    Eigen::Vector2d expected_result;
    expected_result << 0, -1;
    CHECK(result.isApprox(expected_result));
}
