#include <Eigen/Dense>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

auto solve(const Eigen::MatrixXd& H, const Eigen::VectorXd& p)
    -> Eigen::VectorXd {
    return -H.inverse() * p;
}

TEST_CASE("Simple Least Squares", "[ProxQP]") {
    Eigen::Matrix2d H;
    H << 1, 0, 0, 1;
    Eigen::Vector2d p;
    p << 0, 1;

    Eigen::VectorXd result = solve(H, p);

    Eigen::Vector2d expected_result;
    expected_result << 0, -1;
    CHECK(result.isApprox(expected_result));
}
