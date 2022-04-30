#pragma once

#include "fintamath/numbers/Integer.hpp"
#include "fintamath/numbers/Rational.hpp"

namespace fintamath {
  class Real : public NumberImpl<Real> {
  public:
    Real() = default;

    explicit Real(const std::string_view &str);

    explicit Real(const Rational &val, size_t precision);

    Real(const Rational &val);

    Real(Integer val);

    Real(int64_t val);

    std::string toString() const override;

    Real round(size_t precision) const;

    Real floor(size_t precision) const;

    Real sqrt() const;

  protected:
    bool equals(const Real &rhs) const override;

    bool less(const Real &rhs) const override;

    bool more(const Real &rhs) const override;

    Real &add(const Real &rhs) override;

    Real &substract(const Real &rhs) override;

    Real &multiply(const Real &rhs) override;

    Real &divide(const Real &rhs) override;

    Real &negate() override;

    Real &increase() override;

    Real &decrease() override;

  private:
    void parse(const std::string_view &str);

    void parse(const Rational &val, size_t precision);

    static constexpr int64_t DEFAULT_PRECISION = 36;

    Integer intVal;
    int64_t scale{};
    int64_t precision = DEFAULT_PRECISION;
  };
}
