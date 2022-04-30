#include "fintamath/numbers/Real.hpp"

#include <algorithm>
#include <stdexcept>

namespace fintamath {
  Real::Real(const std::string_view &str) {
    parse(str);
  }

  Real::Real(const Rational &val, size_t precision) {
    // TODO
  }

  Real::Real(const Rational &val) : Real(val, DEFAULT_PRECISION) {
  }

  Real::Real(Integer val) : intVal(std::move(val)) {
  }

  Real::Real(int64_t val) : intVal(val) {
  }

  Real Real::round(size_t precision) const {
    // TODO
  }

  Real Real::floor(size_t precision) const {
    // TODO
  }

  std::string Real::toString() const {
    // TODO
  }

  bool Real::equals(const Real &rhs) const {
    // TODO
  }

  bool Real::less(const Real &rhs) const {
    // TODO
  }

  bool Real::more(const Real &rhs) const {
    // TODO
  }

  Real &Real::add(const Real &rhs) {
    // TODO
  }

  Real &Real::substract(const Real &rhs) {
    // TODO
  }

  Real &Real::multiply(const Real &rhs) {
    // TODO
  }

  Real &Real::divide(const Real &rhs) {
    // TODO
  }

  Real &Real::negate() {
    intVal = -intVal;
    return *this;
  }

  Real &Real::increase() {
    return *this += 1;
  }

  Real &Real::decrease() {
    return *this -= 1;
  }

  void Real::parse(const std::string_view &str) {
    // TODO
  }

  void Real::parse(const Rational &val, size_t precision) {
    // TODO
  }
}
