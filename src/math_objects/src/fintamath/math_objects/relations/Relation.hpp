#pragma once

#include "fintamath/math_objects/MathObject.hpp"

class Relation : public MathObject {
public:
  ~Relation() override = 0;
};

inline Relation::~Relation() = default;
