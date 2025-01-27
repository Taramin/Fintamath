#include "fintamath/functions/Operator.hpp"

#include <regex>

#include "fintamath/functions/NamespaceFunctions.hpp"

namespace fintamath {
  Operator::Operator(const std::string &str) {
    parse(str);
  }

  Rational Operator::solve(const Rational &lhs, const Rational &rhs, int64_t precision) const {
    switch (name) {
    case '+':
      return lhs + rhs;
    case '-':
      return lhs - rhs;
    case '*':
      return lhs * rhs;
    case '/':
      return lhs / rhs;
    case '^':
      return functions::pow(lhs, rhs, precision);
    default:
      throw std::invalid_argument("Operator invalid input");
    }
  }

  std::string Operator::toString() const {
    return std::string(1, name);
  }

  bool Operator::equals(const Operator &rhs) const {
    return name == rhs.name;
  }

  void Operator::parse(const std::string &str) {
    if (!types::isOperator(str)) {
      throw std::invalid_argument("Operator invalid input");
    }
    name = *str.begin();
  }

  namespace types {
    bool isOperator(const std::string &str) {
      std::regex reg(R"(\+|\-|\*|\/|\^)");
      return regex_search(str, reg);
    }
  }
}
