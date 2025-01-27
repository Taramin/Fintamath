#pragma once

#include "fintamath/core/MathObject.hpp"

#include <vector>

namespace fintamath {
  class Function;
  using FunctionPtr = std::unique_ptr<Function>;

  class Function : virtual public MathObject {
  public:
    ~Function() override = default;

    template <typename... Args>
    MathObjectPtr operator()(const Args &...args) const {
      std::vector<std::reference_wrapper<const MathObject>> argsVect = {args...};
      return call(argsVect);
    }

  protected:
    virtual MathObjectPtr call(const std::vector<std::reference_wrapper<const MathObject>> &argsVect) const = 0;
  };

  template <typename Derived>
  class FunctionImpl : virtual public Function, virtual public MathObjectImpl<Derived> {
  public:
    ~FunctionImpl() override = default;
  };
}
