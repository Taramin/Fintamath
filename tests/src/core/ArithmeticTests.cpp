#include <gtest/gtest.h>

#include "fintamath/core/Arithmetic.hpp"

#include "fintamath/numbers/Integer.hpp"
#include "fintamath/numbers/Rational.hpp"

using namespace fintamath;

namespace fintamath::tests {
  class TestArithmetic : public ArithmeticImpl<TestArithmetic> {
  public:
    std::string toString() const override {
      return {};
    }

  protected:
    bool equals(const TestArithmetic & /* rhs */) const override {
      return true;
    }

    TestArithmetic &add(const TestArithmetic & /* rhs */) override {
      return *this;
    }

    TestArithmetic &substract(const TestArithmetic & /* rhs */) override {
      return *this;
    }

    TestArithmetic &multiply(const TestArithmetic & /* rhs */) override {
      return *this;
    }

    TestArithmetic &divide(const TestArithmetic & /* rhs */) override {
      return *this;
    }

    TestArithmetic &negate() override {
      return *this;
    }
  };
}

TEST(ArithmeticTests, addTest) {
  ArithmeticPtr m1 = std::make_unique<Integer>(1);
  ArithmeticPtr m2 = std::make_unique<Rational>(2);
  ArithmeticPtr m3 = std::make_unique<Rational>(1, 2);

  EXPECT_EQ((*m1 + *m1)->toString(), "2");
  EXPECT_EQ((*m2 + *m2)->toString(), "4");
  EXPECT_EQ((*m1 + *m2)->toString(), "3");
  EXPECT_EQ((*m2 + *m1)->toString(), "3");

  EXPECT_EQ((*m1 + *m3)->toString(), "3/2");
  EXPECT_EQ((*m2 + *m3)->toString(), "5/2");
  EXPECT_EQ((*m3 + *m1)->toString(), "3/2");
  EXPECT_EQ((*m3 + *m2)->toString(), "5/2");

  EXPECT_THROW(*m1 + tests::TestArithmetic(), std::invalid_argument);
  EXPECT_THROW(tests::TestArithmetic() + *m1, std::invalid_argument);

  Integer a;
  EXPECT_EQ((a += 3).toString(), "3");
  EXPECT_EQ((Rational() + 1).toString(), "1");
  EXPECT_EQ((-1 + Rational()).toString(), "-1");
}

TEST(ArithmeticTests, subTest) {
  ArithmeticPtr m1 = std::make_unique<Integer>(1);
  ArithmeticPtr m2 = std::make_unique<Rational>(2);
  ArithmeticPtr m3 = std::make_unique<Rational>(1, 2);

  EXPECT_EQ((*m1 - *m1)->toString(), "0");
  EXPECT_EQ((*m2 - *m2)->toString(), "0");
  EXPECT_EQ((*m1 - *m2)->toString(), "-1");
  EXPECT_EQ((*m2 - *m1)->toString(), "1");

  EXPECT_EQ((*m1 - *m3)->toString(), "1/2");
  EXPECT_EQ((*m2 - *m3)->toString(), "3/2");
  EXPECT_EQ((*m3 - *m1)->toString(), "-1/2");
  EXPECT_EQ((*m3 - *m2)->toString(), "-3/2");

  EXPECT_THROW(*m1 - tests::TestArithmetic(), std::invalid_argument);
  EXPECT_THROW(tests::TestArithmetic() - *m1, std::invalid_argument);

  Integer a;
  EXPECT_EQ((a -= 3).toString(), "-3");
  EXPECT_EQ((Integer() - 1).toString(), "-1");
  EXPECT_EQ((-1 - Integer()).toString(), "-1");
}

TEST(ArithmeticTests, mulTest) {
  ArithmeticPtr m1 = std::make_unique<Integer>(1);
  ArithmeticPtr m2 = std::make_unique<Rational>(2);
  ArithmeticPtr m3 = std::make_unique<Rational>(1, 2);

  EXPECT_EQ((*m1 * *m1)->toString(), "1");
  EXPECT_EQ((*m2 * *m2)->toString(), "4");
  EXPECT_EQ((*m1 * *m2)->toString(), "2");
  EXPECT_EQ((*m2 * *m1)->toString(), "2");

  EXPECT_EQ((*m1 * *m3)->toString(), "1/2");
  EXPECT_EQ((*m2 * *m3)->toString(), "1");
  EXPECT_EQ((*m3 * *m1)->toString(), "1/2");
  EXPECT_EQ((*m3 * *m2)->toString(), "1");

  EXPECT_THROW(*m1 * tests::TestArithmetic(), std::invalid_argument);
  EXPECT_THROW(tests::TestArithmetic() * *m1, std::invalid_argument);

  Integer a = 2;
  EXPECT_EQ((a *= 3).toString(), "6");
  EXPECT_EQ((Integer(2) * 2).toString(), "4");
  EXPECT_EQ((-2 * Integer(2)).toString(), "-4");
}

TEST(ArithmeticTests, diveTest) {
  ArithmeticPtr m1 = std::make_unique<Integer>(1);
  ArithmeticPtr m2 = std::make_unique<Integer>(5);
  ArithmeticPtr m3 = std::make_unique<Rational>(2);
  ArithmeticPtr m4 = std::make_unique<Rational>(1, 2);

  EXPECT_EQ((*m1 / *m2)->toString(), "1/5");

  EXPECT_EQ((*m1 / *m1)->toString(), "1");
  EXPECT_EQ((*m3 / *m3)->toString(), "1");
  EXPECT_EQ((*m1 / *m3)->toString(), "1/2");
  EXPECT_EQ((*m3 / *m1)->toString(), "2");

  EXPECT_EQ((*m1 / *m4)->toString(), "2");
  EXPECT_EQ((*m3 / *m4)->toString(), "4");
  EXPECT_EQ((*m4 / *m1)->toString(), "1/2");
  EXPECT_EQ((*m4 / *m3)->toString(), "1/4");

  EXPECT_THROW(*m1 / tests::TestArithmetic(), std::invalid_argument);
  EXPECT_THROW(tests::TestArithmetic() / *m1, std::invalid_argument);

  Integer a = 4;
  EXPECT_EQ((a /= 2).toString(), "2");
  EXPECT_EQ((Integer(4) / 2).toString(), "2");
  EXPECT_EQ((-2 / Integer(2)).toString(), "-1");
}

TEST(ArithmeticTests, convertTest) {
  ArithmeticPtr m1 = std::make_unique<Integer>(1);
  EXPECT_EQ((+*m1)->toString(), "1");

  EXPECT_EQ((+Integer(1)).toString(), "1");
}

TEST(ArithmeticTests, negateTest) {
  ArithmeticPtr m1 = std::make_unique<Integer>(1);
  EXPECT_EQ((-*m1)->toString(), "-1");

  EXPECT_EQ((-Integer(1)).toString(), "-1");
}
