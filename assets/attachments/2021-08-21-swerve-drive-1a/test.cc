#include <cmath>
#include <cstddef>
#include <iostream>

#include <Eigen/Core>
#include <Eigen/QR>

int main(int argc, char **argv) {
  Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> m_(8, 3);
  m_ <<
      1, 0, -1,
      0, 1,  1,
      1, 0,  1,
      0, 1,  1,
      1, 0,  1,
      0, 1, -1,
      1, 0, -1,
      0, 1, -1;
  auto qr_ = m_.householderQr();

  for (std::size_t i = 0; i < 100; ++i) {
    Eigen::Vector<double, Eigen::Dynamic> v_(8);
    v_.setRandom();
    auto err_ = v_ - m_ * qr_.solve(v_);
    assert(fabs(err_.dot(v_ - err_)) < 1e-15);
  }

  return 0;
}