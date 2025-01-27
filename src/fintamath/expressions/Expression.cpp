#include "fintamath/expressions/Expression.hpp"

#include <algorithm>
#include <stdexcept>

#include "fintamath/constants/Constant.hpp"
#include "fintamath/functions/ConcreteFunction.hpp"
#include "fintamath/functions/Operator.hpp"
#include "fintamath/variables/Variable.hpp"

namespace fintamath {
  constexpr int64_t INITIAL_PRECISION = 36;
  constexpr int64_t PRECISION_INCREASER = 9;
  constexpr int64_t NEW_PRECISION = INITIAL_PRECISION + PRECISION_INCREASER;
  constexpr int64_t NEW_ROUND_PRECISION = NEW_PRECISION - 1;

  std::vector<std::string> makeVectOfTokens(const std::string &strExpr);
  Expression makeExpression(const std::vector<std::string> &tokensVect);

  void cutSpaces(std::string &strExpr);

  bool isDigit(char ch);
  bool isLetter(char ch);

  void addMultiply(std::vector<std::string> &tokensVect);
  void addClosingBracket(std::vector<std::string> &tokensVect);
  void addOpenBracket(std::vector<std::string> &tokensVect);
  void addUnaryOperator(std::vector<std::string> &tokensVect);
  void addOperator(std::vector<std::string> &tokensVect, char token);
  void addRational(std::vector<std::string> &tokensVect, const std::string &token, size_t &pos);
  void addFactorial(std::vector<std::string> &tokensVect, const std::string &token, size_t &pos);
  void addConstOrFunction(std::vector<std::string> &tokensVect, const std::string &token, size_t &pos);
  void addBinaryFunctions(std::vector<std::string> &tokensVect);
  void addBinaryFunction(std::vector<std::string> &tokensVect, std::vector<size_t> &placementsVect, size_t num);
  void addValue(std::shared_ptr<Expression::Elem> &elem, const std::string &token);
  void addVariable(std::vector<std::string> &tokensVect, const std::string &token, size_t &pos);

  bool descent(const std::vector<std::string> &tokensVect, const std::shared_ptr<Expression::Elem> &elem, size_t begin,
               size_t end, const std::string &oper1, const std::string_view &oper2);
  bool descent(const std::vector<std::string> &tokensVect, const std::shared_ptr<Expression::Elem> &elem, size_t begin,
               size_t end, const std::string &oper);
  bool descent(const std::vector<std::string> &tokensVect, const std::shared_ptr<Expression::Elem> &elem, size_t begin,
               size_t end);

  void makeExpressionRec(const std::vector<std::string> &tokensVect, std::shared_ptr<Expression::Elem> &elem,
                         size_t first, size_t last);

  Rational toRational(const std::shared_ptr<Expression::Elem> &elem);
  void solveRec(const std::shared_ptr<Expression::Elem> &elem);
  void elemReset(const std::shared_ptr<Expression::Elem> &elem, const Rational &val);

  void toShortForm(std::string &strVal);
  void insertFloatingPoint(std::string &strVal, int64_t precision);
  size_t cutZeros(std::string &strVal);

  std::string toString(const Expression::Elem &elem);
  void clone(const std::shared_ptr<Expression::Elem> &from, std::shared_ptr<Expression::Elem> &to);
  bool equals(const Expression::Elem &lhs, const Expression::Elem &rhs);

  Expression::Expression(const Expression &rhs) noexcept {
    if (rhs.root) {
      fintamath::clone(rhs.root, root);
    }
  }

  Expression::Expression(Expression &&rhs) noexcept : root(std::move(rhs.root)) {
  }

  Expression &Expression::operator=(const Expression &rhs) noexcept {
    if (&rhs != this) {
      if (rhs.root) {
        fintamath::clone(rhs.root, root);
      } else {
        root = nullptr;
      }
    }
    return *this;
  }

  Expression &Expression::operator=(Expression &&rhs) noexcept {
    if (&rhs != this) {
      std::swap(root, rhs.root);
    }
    return *this;
  }

  Expression::Expression(const std::string &str) {
    makeExpression(makeVectOfTokens(str));
  }

  Expression::Expression(const MathObject &obj) {
    root = std::make_shared<Expression::Elem>();
    root->info = obj.clone();
  }

  void Expression::makeExpression(const std::vector<std::string> &tokensVect) {
    if (tokensVect.empty()) {
      throw std::invalid_argument("Expression invalid input");
    }
    makeExpressionRec(tokensVect, root, 0, tokensVect.size() - 1);
  }

  std::string Expression::toString() const {
    if (!root) {
      return {};
    }
    return fintamath::toString(*root);
  }

  std::string Expression::solve() {
    solveRec(root);
    auto resVal = toRational(root);
    auto valStr = resVal.toString(INITIAL_PRECISION);
    toShortForm(valStr);
    return valStr;
  }

  bool Expression::equals(const Expression &rhs) const {
    return fintamath::equals(*root, *rhs.root);
  }

  std::vector<std::string> makeVectOfTokens(const std::string &strExpr) {
    std::string tmpStrExpr = strExpr;
    cutSpaces(tmpStrExpr);
    std::vector<std::string> tokensVect;

    size_t i = 0;
    while (i < tmpStrExpr.size()) {
      if (tmpStrExpr[i] == ')') {
        addClosingBracket(tokensVect);
      } else if (tmpStrExpr[i] == '(') {
        addOpenBracket(tokensVect);
      } else if (types::isOperator(std::string(1, tmpStrExpr[i]))) {
        addOperator(tokensVect, tmpStrExpr[i]);
      } else if (tmpStrExpr[i] == '!') {
        addFactorial(tokensVect, tmpStrExpr, i);
      } else if (isDigit(tmpStrExpr[i])) {
        addRational(tokensVect, tmpStrExpr, i);
      } else {
        try {
          addConstOrFunction(tokensVect, tmpStrExpr, i);
        } catch (const std::invalid_argument &) {
          addVariable(tokensVect, tmpStrExpr, i);
        }
      }
      i++;
    }

    addBinaryFunctions(tokensVect);
    reverse(tokensVect.begin(), tokensVect.end());

    return tokensVect;
  }

  void cutSpaces(std::string &strExpr) {
    while (!strExpr.empty()) {
      if (strExpr.front() != ' ') {
        break;
      }
      strExpr.erase(strExpr.begin());
    }

    int64_t i = 0;
    while (i < int64_t(strExpr.size())) {
      if (strExpr[size_t(i)] == ' ') {
        strExpr.erase(size_t(i), 1);
        i--;
      }
      i++;
    }
  }

  bool isDigit(char ch) {
    return (ch >= '0' && ch <= '9');
  }

  bool isLetter(char ch) {
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
  }

  void addMultiply(std::vector<std::string> &tokensVect) {
    if (tokensVect.empty()) {
      return;
    }
    if (tokensVect.back() != "," && tokensVect.back() != "(" && !types::isOperator(tokensVect.back()) &&
        !types::isFunction(tokensVect.back())) {
      tokensVect.emplace(tokensVect.end(), "*");
    }
  }

  void addClosingBracket(std::vector<std::string> &tokensVect) {
    tokensVect.emplace_back(")");
  }

  void addOpenBracket(std::vector<std::string> &tokensVect) {
    addMultiply(tokensVect);
    tokensVect.emplace_back("(");
  }

  void addUnaryOperator(std::vector<std::string> &tokensVect) {
    if (tokensVect.back() == "+") {
      tokensVect.pop_back();
    } else if (tokensVect.back() == "-") {
      tokensVect.pop_back();
      tokensVect.emplace_back("-1");
      tokensVect.emplace_back("*");
    }
  }

  void addOperator(std::vector<std::string> &tokensVect, char token) {
    tokensVect.emplace_back(1, token);
    if (tokensVect.size() == 1) {
      addUnaryOperator(tokensVect);
      return;
    }
    if (*(tokensVect.end() - 2) == "(") {
      addUnaryOperator(tokensVect);
    }
  }

  void addRational(std::vector<std::string> &tokensVect, const std::string &token, size_t &pos) {
    addMultiply(tokensVect);

    std::string strVal;
    while (pos < token.size()) {
      strVal += token[pos];
      pos++;
      if (!isDigit(token[pos]) && token[pos] != '.') {
        break;
      }
    }
    if (pos != 0) {
      pos--;
    }

    tokensVect.push_back(strVal);
  }

  void addFactorial(std::vector<std::string> &tokensVect, const std::string &token, size_t &pos) {
    if (tokensVect.empty()) {
      throw std::invalid_argument("Expression invalid input");
    }

    std::string factorialFunc = "!";
    if (pos != token.size() - 1 && token[pos + 1] == '!') {
      factorialFunc += '!';
      pos++;
    }

    size_t bracketsNum = 0;

    for (size_t i = tokensVect.size() - 1; i > 0; i--) {
      if (tokensVect[i] == ")") {
        bracketsNum++;
      } else if (tokensVect[i] == "(") {
        if (bracketsNum == 0) {
          throw std::invalid_argument("Expression invalid input");
        }
        bracketsNum--;
      }
      if (bracketsNum == 0) {
        if (types::isFunction(tokensVect[i - 1])) {
          tokensVect.insert(tokensVect.begin() + int64_t(i - 1), factorialFunc);
        }
        tokensVect.insert(tokensVect.begin() + int64_t(i), factorialFunc);
        return;
      }
    }

    if (tokensVect.front() == "(") {
      if (bracketsNum == 0) {
        throw std::invalid_argument("Expression invalid input");
      }
      bracketsNum--;
    }
    if (bracketsNum != 0) {
      throw std::invalid_argument("Expression invalid input");
    }

    tokensVect.insert(tokensVect.begin(), factorialFunc);
  }

  void addConstOrFunction(std::vector<std::string> &tokensVect, const std::string &token, size_t &pos) {
    if (token[pos] == '!') {
      addFactorial(tokensVect, token, pos);
      return;
    }
    if (token[pos] == ',') {
      tokensVect.emplace_back(",");
      return;
    }

    std::string literalExpr;
    size_t newPos = pos;
    while (newPos < token.size()) {
      literalExpr += token[newPos];
      newPos++;
      if (!isLetter(token[newPos])) {
        break;
      }
    }
    if (newPos != 0) {
      newPos--;
    }

    if (types::isConstant(literalExpr) || types::isFunction(literalExpr)) {
      pos = newPos;
      addMultiply(tokensVect);
      tokensVect.push_back(literalExpr);
    } else {
      throw std::invalid_argument("Expression invalid input");
    }
  }

  void addBinaryFunctions(std::vector<std::string> &tokensVect) {
    std::vector<size_t> placementsVect;
    size_t i = 0;
    while (i < tokensVect.size()) {
      if (types::isBinaryFunction(tokensVect[i]) &&
          find(placementsVect.begin(), placementsVect.end(), i) == placementsVect.end()) {
        addBinaryFunction(tokensVect, placementsVect, i);
      }
      i++;
    }
  }

  void addBinaryFunction(std::vector<std::string> &tokensVect, std::vector<size_t> &placementsVect, size_t num) {
    std::string token = tokensVect[num];
    tokensVect.erase(tokensVect.begin() + int64_t(num));
    size_t bracketsNum = 1;

    size_t i = num + 1;
    while (i < tokensVect.size()) {
      if (bracketsNum == 0) {
        throw std::invalid_argument("Expression invalid input");
      }

      if (tokensVect[i] == "(") {
        bracketsNum++;
      } else if (tokensVect[i] == ")") {
        bracketsNum--;
      } else if (bracketsNum == 1 && tokensVect[i] == ",") {
        tokensVect.erase(tokensVect.begin() + int64_t(i));
        tokensVect.insert(tokensVect.begin() + int64_t(i), {")", token, "("});

        transform(placementsVect.begin(), placementsVect.end(), placementsVect.begin(),
                  [i](size_t k) { return (k > i) ? k + 1 : k; });
        placementsVect.push_back(i + 1);

        return;
      }

      i++;
    }

    throw std::invalid_argument("Expression invalid input");
  }

  void addVariable(std::vector<std::string> &tokensVect, const std::string &token, size_t &pos) {
    if (types::isVariable(std::string(1, token[pos]))) {
      addMultiply(tokensVect);
      tokensVect.emplace_back(1, token[pos]);
    } else {
      throw std::invalid_argument("Expression invalid input");
    }
  }

  void addValue(std::shared_ptr<Expression::Elem> &elem, const std::string &token) {
    if (types::isConstant(token)) {
      elem->info = std::make_shared<Constant>(token);
    } else if (types::isVariable(token)) {
      elem->info = std::make_shared<Variable>(token);
    } else {
      try {
        elem->info = std::make_shared<Rational>(token);
      } catch (const std::invalid_argument &) {
        throw std::invalid_argument("Expression invalid input");
      }
    }
  }

  bool descent(const std::vector<std::string> &tokensVect, const std::shared_ptr<Expression::Elem> &elem, size_t begin,
               size_t end, const std::string &oper1, const std::string_view &oper2) {
    size_t bracketsNum = 0;

    for (size_t i = begin; i <= end; i++) {
      if (tokensVect[i] == ")") {
        bracketsNum++;
      } else if (tokensVect[i] == "(") {
        if (bracketsNum == 0) {
          throw std::invalid_argument("Expression invalid input");
        }
        bracketsNum--;
      }

      if (bracketsNum == 0 && (tokensVect[i] == oper1 || tokensVect[i] == oper2)) {
        if (types::isBinaryFunction(oper1)) {
          elem->info = std::make_shared<ConcreteFunction>(tokensVect[i]);
        } else {
          elem->info = std::make_shared<Operator>(tokensVect[i]);
        }
        makeExpressionRec(tokensVect, elem->right, i + 1, end);
        makeExpressionRec(tokensVect, elem->left, begin, i - 1);
        return true;
      }
    }

    return false;
  }

  bool descent(const std::vector<std::string> &tokensVect, const std::shared_ptr<Expression::Elem> &elem, size_t begin,
               size_t end, const std::string &oper) {
    return descent(tokensVect, elem, begin, end, oper, "");
  }

  bool descent(const std::vector<std::string> &tokensVect, const std::shared_ptr<Expression::Elem> &elem, size_t begin,
               size_t end) {
    if (types::isFunction(tokensVect[end])) {
      elem->info = std::make_shared<ConcreteFunction>(tokensVect[end]);
      makeExpressionRec(tokensVect, elem->right, begin, end - 1);
      return true;
    }
    return false;
  }

  void makeExpressionRec(const std::vector<std::string> &tokensVect, std::shared_ptr<Expression::Elem> &elem,
                         size_t first, size_t last) {
    if (first > last) {
      throw std::invalid_argument("Expression invalid input");
    }
    if (first == SIZE_MAX || last == SIZE_MAX) {
      throw std::invalid_argument("Expression invalid input");
    }

    if (elem == nullptr) {
      elem = std::make_shared<Expression::Elem>();
    }

    if (first == last) {
      addValue(elem, tokensVect[first]);
      return;
    }

    if (descent(tokensVect, elem, first, last, "+", "-")) {
      return;
    }
    if (descent(tokensVect, elem, first, last, "*", "/")) {
      return;
    }
    if (descent(tokensVect, elem, first, last, "^")) {
      return;
    }
    if (descent(tokensVect, elem, first, last, "log")) {
      return;
    }
    if (descent(tokensVect, elem, first, last)) {
      return;
    }
    if (tokensVect[first] == ")" && tokensVect[last] == "(") {
      makeExpressionRec(tokensVect, elem, first + 1, last - 1);
    }
  }

  Rational toRational(const std::shared_ptr<Expression::Elem> &elem) {
    if (elem->info == nullptr) {
      throw std::invalid_argument("Solver invalid input");
    }

    if (elem->info->is<Constant>()) {
      return Constant(elem->info->toString()).toRational(NEW_PRECISION);
    }

    try {
      auto res = std::dynamic_pointer_cast<Rational>(elem->info);
      if (!res) {
        throw std::invalid_argument("Solver invalid input");
      }
      return *res;
    } catch (const std::invalid_argument &) {
      throw std::invalid_argument("Solver invalid input");
    }
  }

  void solveRec(const std::shared_ptr<Expression::Elem> &elem) {
    if (elem->info == nullptr) {
      throw std::invalid_argument("Solver invalid input");
    }

    if (elem->right != nullptr) {
      if (elem->right->info == nullptr) {
        throw std::invalid_argument("Solver invalid input");
      }
      if (elem->right->info->is<Operator>() || elem->right->info->is<ConcreteFunction>()) {
        solveRec(elem->right);
      }
    }

    if (elem->left != nullptr) {
      if (elem->left->info == nullptr) {
        throw std::invalid_argument("Solver invalid input");
      }
      if (elem->left->info->is<Operator>() || elem->left->info->is<ConcreteFunction>()) {
        solveRec(elem->left);
      }
    }

    if (elem->info->is<Operator>()) {
      Operator oper(elem->info->toString());
      Rational val(oper.solve(toRational(elem->right), toRational(elem->left), NEW_PRECISION).toString(NEW_PRECISION));
      elemReset(elem, val);
      return;
    }

    if (elem->info->is<ConcreteFunction>()) {
      ConcreteFunction func(elem->info->toString());
      Rational val;
      if (types::isBinaryFunction(func.toString())) {
        val = Rational(
            func.solve(toRational(elem->right), toRational(elem->left), NEW_PRECISION).toString(NEW_ROUND_PRECISION));
      } else {
        val = Rational(func.solve(toRational(elem->right), NEW_PRECISION).toString(NEW_ROUND_PRECISION));
      }
      elemReset(elem, val);
      return;
    }
  }

  void elemReset(const std::shared_ptr<Expression::Elem> &elem, const Rational &val) {
    elem->info = std::make_shared<Rational>(val);
    elem->right.reset();
    elem->left.reset();
  }

  void toShortForm(std::string &strVal) {
    bool isNegative = (*strVal.begin() == '-');
    if (isNegative) {
      strVal.erase(0, 1);
    }

    if (strVal == "0") {
      return;
    }

    if (*strVal.begin() == '0') {
      strVal.erase(strVal.begin() + 1);
      size_t order = cutZeros(strVal);
      insertFloatingPoint(strVal, INITIAL_PRECISION);
      strVal += "*10^(-";
      strVal += std::to_string(order) + ')';
    } else {
      int64_t order = distance(begin(strVal), find(begin(strVal), end(strVal), '.'));
      if (size_t(order) != strVal.size()) {
        strVal.erase(size_t(order), 1);
      }
      if (strVal.size() > INITIAL_PRECISION + 1) {
        strVal.erase(INITIAL_PRECISION + 2);
      }

      insertFloatingPoint(strVal, INITIAL_PRECISION);

      if (order > 1) {
        strVal += "*10^";
        strVal += std::to_string(order - 1);
      }
    }

    if (isNegative) {
      strVal.insert(strVal.begin(), '-');
    }
  }

  void insertFloatingPoint(std::string &strVal, int64_t precision) {
    strVal.insert(strVal.begin() + 1, '.');
    strVal += '0';
    strVal = Rational(strVal).toString(precision);
  }

  size_t cutZeros(std::string &strVal) {
    size_t order = 0;
    while (*strVal.begin() == '0') {
      strVal.erase(strVal.begin());
      order++;
    }
    return order;
  }

  std::string toString(const Expression::Elem &elem) {
    std::string res;
    if (elem.right) {
      res += "(" + fintamath::toString(*elem.right) + ")";
    }
    res += elem.info->toString();
    if (elem.left) {
      res += "(" + fintamath::toString(*elem.left) + ")";
    }
    return res;
  }

  void clone(const std::shared_ptr<Expression::Elem> &from, std::shared_ptr<Expression::Elem> &to) {
    if (!to) {
      to = std::make_shared<Expression::Elem>();
    }
    if (from->right) {
      clone(from->right, to->right);
    }
    if (from->left) {
      clone(from->left, to->left);
    }
    to->info = from->info->clone();
  }

  bool equals(const Expression::Elem &lhs, const Expression::Elem &rhs) {
    if (*lhs.info != *rhs.info) {
      return false;
    }
    if (lhs.right) {
      if (rhs.right) {
        return equals(*lhs.right, *rhs.right);
      }
      return false;
    }
    if (rhs.right) {
      return false;
    }
    if (lhs.left) {
      if (rhs.left) {
        return equals(*lhs.left, *rhs.left);
      }
      return false;
    }
    if (rhs.left) {
      return false;
    }
    return true;
  }
}
