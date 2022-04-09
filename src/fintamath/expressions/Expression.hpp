#pragma once

#include <memory>
#include <vector>

#include "fintamath/core/MathObject.hpp"

namespace fintamath {
  class Expression : public MathObjectBase<Expression> {
  public:
    struct Elem {
      std::shared_ptr<MathObject> info;
      std::shared_ptr<Elem> left;
      std::shared_ptr<Elem> right;
    };

    Expression() = default;

    Expression(const Expression &rhs) noexcept;

    Expression &operator=(const Expression &rhs) noexcept;

    explicit Expression(const std::string &str);

    explicit Expression(const MathObject &obj);

    std::string toString() const override;

    std::string solve();

  protected:
    bool equals(const Expression &rhs) const override;

  private:
    std::shared_ptr<Elem> root;

    void makeExpression(const std::vector<std::string> &tokensVect);
  };
}