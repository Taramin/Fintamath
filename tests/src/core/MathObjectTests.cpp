#include <gtest/gtest.h>

#include "fintamath/core/MathObject.hpp"

#include "fintamath/numbers/Integer.hpp"
#include "fintamath/numbers/Rational.hpp"
#include "fintamath/variables/Variable.hpp"

using namespace fintamath;

TEST(MathObjectTests, isTest) {
  MathObjectPtr m1 = std::make_unique<Integer>();

  EXPECT_TRUE(m1->is<Integer>());

  EXPECT_FALSE(m1->is<Rational>());
  EXPECT_FALSE(m1->is<Variable>());
}

TEST(MathObjectTests, instanceofTest) {
  MathObjectPtr m1 = std::make_unique<Integer>();

  EXPECT_TRUE(m1->instanceOf<Integer>());
  EXPECT_TRUE(m1->instanceOf<Number>());
  EXPECT_TRUE(m1->instanceOf<Arithmetic>());
  EXPECT_TRUE(m1->instanceOf<MathObject>());

  EXPECT_FALSE(m1->instanceOf<Rational>());
  EXPECT_FALSE(m1->instanceOf<Variable>());
}

TEST(MathObjectTests, toTest) {
  MathObjectPtr m1 = std::make_unique<Integer>();

  EXPECT_TRUE(m1->to<Integer>().instanceOf<Integer>());
  EXPECT_TRUE(m1->to<Number>().instanceOf<Number>());
  EXPECT_TRUE(m1->to<Arithmetic>().instanceOf<Arithmetic>());
  EXPECT_TRUE(m1->to<MathObject>().instanceOf<MathObject>());

  EXPECT_TRUE(m1->to<MathObject>().instanceOf<Integer>());

  EXPECT_THROW(m1->to<Rational>(), std::bad_cast);
  EXPECT_THROW(m1->to<Variable>(), std::bad_cast);
}

TEST(MathObjectTests, cloneTest) {
  MathObjectPtr m1 = std::make_unique<Integer>();
  MathObjectPtr m2 = std::make_unique<Integer>(1);
  MathObjectPtr m3 = m1->clone();

  EXPECT_EQ(*m3, *m1);

  EXPECT_NE(*m3, *m2);

  EXPECT_NE(m3, m1);
}

TEST(MathObjectTests, equalsTest) {
  MathObjectPtr m1 = std::make_unique<Integer>(1);
  MathObjectPtr m2 = std::make_unique<Rational>(1);
  MathObjectPtr m3 = std::make_unique<Rational>(1, 2);

  EXPECT_TRUE(*m1 == *m1);
  EXPECT_TRUE(*m2 == *m2);
  EXPECT_TRUE(*m1 == *m2);
  EXPECT_TRUE(*m2 == *m1);

  EXPECT_FALSE(*m1 == *m3);
  EXPECT_FALSE(*m2 == *m3);
  EXPECT_FALSE(*m3 == *m1);
  EXPECT_FALSE(*m3 == *m2);

  EXPECT_FALSE(*m1 == Variable("a"));
  EXPECT_FALSE(Variable("a") == *m1);

  EXPECT_TRUE(Integer() == 0);
  EXPECT_TRUE(0 == Integer());

  EXPECT_FALSE(Integer() == 1);
  EXPECT_FALSE(1 == Integer());
}

TEST(MathObjectTests, nequalsTest) {
  MathObjectPtr m1 = std::make_unique<Integer>(1);
  MathObjectPtr m2 = std::make_unique<Rational>(1);
  MathObjectPtr m3 = std::make_unique<Rational>(1, 2);

  EXPECT_FALSE(*m1 != *m1);
  EXPECT_FALSE(*m2 != *m2);
  EXPECT_FALSE(*m1 != *m2);
  EXPECT_FALSE(*m2 != *m1);

  EXPECT_TRUE(*m1 != *m3);
  EXPECT_TRUE(*m2 != *m3);
  EXPECT_TRUE(*m3 != *m1);
  EXPECT_TRUE(*m3 != *m2);

  EXPECT_TRUE(*m1 != Variable("a"));
  EXPECT_TRUE(Variable("a") != *m1);

  EXPECT_FALSE(Integer() != 0);
  EXPECT_FALSE(0 != Integer());

  EXPECT_TRUE(Integer() != 1);
  EXPECT_TRUE(1 != Integer());
}

TEST(MathObjectTests, outputTest) {
  MathObjectPtr m1 = std::make_unique<Integer>(123);
  std::stringstream out;
  out << *m1;
  EXPECT_EQ(out.str(), "123");
}
