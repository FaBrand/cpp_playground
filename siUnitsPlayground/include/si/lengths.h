#ifndef LENGHTS_H
#define LENGHTS_H

#include <cstdint>
#include <ratio>

namespace detail
{
template <typename T>
struct ratio_helper
{
    constexpr static const double factor = static_cast<double>(T::num) / T::den;
    constexpr static const double inverse_factor = static_cast<double>(T::den) / T::num;
    using inverse_ratio = std::ratio<T::den, T::num>;
};

template <typename A, typename B>
struct same_base_unit
{
    constexpr static bool value = std::is_same<typename A::base, typename B::base>::value;
};

struct base_unit
{
};

template <typename T>
constexpr double scale(const double& value)
{
    return value * T::num / T::den;
}

template <typename T>
constexpr double scale(const double&& value)
{
    return value * T::num / T::den;
}

template <typename T, typename base_type, typename base_ratio = std::ratio<1>>
class unit_type
{
  public:
    using base = base_type;
    using ratio = base_ratio;
    using type = T;

    static_assert(std::is_base_of<base_unit, base_type>::value, "Base type must publicly derive from base_unit");
    static_assert(std::is_fundamental<T>::value, "Value representation must be fundamental!");

    unit_type() = default;

    template <typename ration>
    constexpr unit_type(unit_type<type, base, ration> const& other) : value_(other.to<unit_type>())
    {
    }

    template <typename ration>
    constexpr unit_type(unit_type<type, base, ration>&& other) : value_(other.to<unit_type>())
    {
    }

    template <typename ration>
    constexpr unit_type& operator=(unit_type<type, base, ration> const& other)
    {
        value_ = other.to<unit_type>();
        return *this;
    }

    template <typename ration>
    constexpr unit_type& operator=(unit_type<type, base, ration>&& other)
    {
        value_ = other.to<unit_type>();
        return *this;
    }

    constexpr explicit unit_type(T val) : value_(val){};

    // FIXME Doesnt work when instantiated in cast function
    template <typename U>
    constexpr type to() const
    {
        static_assert(same_base_unit<unit_type, U>::value, "Can't convert types because of different base units!");
        using inverse_ratio = typename ratio_helper<typename U::ratio>::inverse_ratio;
        return scale<inverse_ratio>(count());
    }

    constexpr type count() const
    {
        return scale<ratio>(value_);
    };

  private:
    T value_{};
};
}  // detail

template <typename T, typename U>
constexpr auto cast(const U& castee) -> decltype(auto)
{
    static_assert(detail::same_base_unit<T, U>::value, "Can't convert types because of different base units!");
    using inverse_ratio = typename detail::ratio_helper<typename T::ratio>::inverse_ratio;
    return detail::scale<inverse_ratio>(castee.count());
    // FIXME Reuse would be favourable but it doesnt work yet
    // return castee.to<T>();
}

struct base_meter : public detail::base_unit
{
};

struct base_seconds : public detail::base_unit
{
};

using meter = detail::unit_type<double, base_meter>;
using dezimeter = detail::unit_type<double, base_meter, std::deci>;
using kilometer = detail::unit_type<double, base_meter, std::kilo>;
using millimeter = detail::unit_type<double, base_meter, std::milli>;

using nanoseconds = detail::unit_type<double, base_seconds, std::nano>;
using microseconds = detail::unit_type<double, base_seconds, std::micro>;
using milliseconds = detail::unit_type<double, base_seconds, std::milli>;
using seconds = detail::unit_type<double, base_seconds>;
using minutes = detail::unit_type<double, base_seconds, std::ratio<60>>;
using hours = detail::unit_type<double, base_seconds, std::ratio_multiply<std::ratio<60>, minutes::ratio>>;
using days = detail::unit_type<double, base_seconds, std::ratio_multiply<std::ratio<24>, hours::ratio>>;
using weeks = detail::unit_type<double, base_seconds, std::ratio_multiply<std::ratio<7>, days::ratio>>;
using year = detail::unit_type<double, base_seconds, std::ratio_multiply<std::ratio<522>, weeks::ratio>>;

#endif  // LENGHTS_H
