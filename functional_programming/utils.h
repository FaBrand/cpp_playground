//================================================================================================
// Copyright (C) 2019 Arrival Robotics.
//
// This file is part of Lemo project
//================================================================================================

#ifndef LEMO_UTILS_H
#define LEMO_UTILS_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace lemo::utils {

inline static double lerp(double a, double b, double t) {
  return (1. - t) * a + t * b;
}

/// \brief Applies a rate limited step from the current value towards the desired value, given a
/// maximum rate.
///
/// \param[in] cur - The current value
/// \param[in] des - The desired value
/// \param[in] dt  - The delta time over which the step should be applied
/// \param[in] max_rate - The maximum time derivative of the step
///
/// \return A tuple indicating the new value and the rate used to arrive there.
inline static std::tuple<double, double> slewRateLimit(double cur, double des, double dt,
                                                       double max_rate) {
  assert(dt > 0);

  if (dt <= 0) {
    return std::tuple<double, double>(cur, 0.);
  }

  max_rate = fabs(max_rate);
  const double rate = std::clamp((des - cur) / dt, -max_rate, max_rate);
  return std::tuple<double, double>(cur + rate * dt, rate);
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
struct Limits {
  Limits() = default;
  Limits(T lim) : min(-lim), max(lim) {
  }
  Limits(T min, T max) : min(min), max(max) {
  }

  T min{ -std::numeric_limits<T>::infinity() };
  T max{ std::numeric_limits<T>::infinity() };

  T clamp(T val, bool* saturated = nullptr) const {
    if (saturated != nullptr) {
      *saturated = val > max || val < min;
    }
    return std::clamp(val, min, max);
  }

  bool contains(T val) const {
    return val >= min && val <= max;
  }
};

/// \brief Recenters a value to be within +- a given amplitude and centered about a given value.
///
/// \param[in] value - The value to be recentered
/// \param[in] center - The center point of the new value
/// \param[in] amplitude - The amplitude (+-) of the value
///
/// \return A new value that is centered about `center`, given the supplied amplitude
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
T recenterValue(T value, T center, T amplitude) {
  center -= 0.5 * amplitude;
  return value - amplitude * floor((value - center) / amplitude);
}

/// \brief Recenters an angle about a specified center point.
///
/// \param[in] angle - The angle to be recentered
/// \param[in] center - The new center point [default = 0]
///
/// \return An angle on the interval [center - pi, center + pi)
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
T recenterAngle(T angle, T center = 0) {
  return recenterValue(angle, center, 2 * M_PI);
}

/// \brief Checks the input for Nans
///
/// \param[in] value - The value to be checked
///
/// \return True if NaN, false otherwise
template <typename T>
bool isNaN(T value) {
  return value != value;
}

namespace string {

/// \brief Splits a string into segments at the delimiter.
///
/// \param[in] s - The string to be split
/// \param[in] delimiter - Character on which to split [default='.']
///
/// \return A vector of strings making up the parts of 's'
inline std::vector<std::string> split(const std::string& s, char delimiter = '.') {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream token_stream(s);
  while (std::getline(token_stream, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

/// \brief Splits the string on the first instance of a delimiter
///
/// \param[in] s - The input string
/// \param[in] delimiter - String on which to split [default="."]
///
/// \return A tuple with the two parts of the string
inline std::pair<std::string, std::string> splitHead(const std::string& s,
                                                     std::string delimiter = ".") {
  const auto pos = s.find(delimiter);
  const auto head = s.substr(0, pos);
  const auto tail = pos == std::string::npos ? "" : s.substr(pos + delimiter.length());

  return std::pair<std::string, std::string>(head, tail);
}

inline std::string getParent(const std::string& s, char delimiter = '.') {
  const auto pos = s.rfind(delimiter);
  return pos == std::string::npos ? s : s.substr(0, pos);
}

}  // namespace string

}  // namespace lemo::utils

#endif  // LEMO_UTILS_H
