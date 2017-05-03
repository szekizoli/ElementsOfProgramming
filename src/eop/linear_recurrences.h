#pragma once

#include <utility>

#include "type_functions.h"
#include "eop.h"

#define Numeric(...)

namespace eop {

  // This library is eligible for linear recurrences of order 2

  template<typename N>
    requires(Numeric(N))
  using Coefficients = std::pair<N, N>;

  template<typename N>
    requires(Numeric(N))
  using Starting_elements = std::pair<N, N>;

  template<typename N>
    requires(Numeric(N))
  using Matrix = std::pair<std::pair<N, N>, std::pair<N, N>>;

  template<typename N>
    requires(Numeric(N))
  eop::Coefficients<N> make_coefficients(N n_0, N n_1) {
    return std::make_pair(n_0, n_1);
  }

  template<typename N>
    requires(Numeric(N))
  eop::Matrix<N> make_matrix(const Coefficients<N>& coeff) {
    return std::make_pair(
               coeff,
               std::make_pair(N(1), N(0)));
  }

  template<typename N>
  requires(Numeric(N))
    eop::Matrix<N> make_matrix(N n_0_0, N n_0_1, N n_1_0, N n_1_1) {
    return std::make_pair(
      std::make_pair(n_0_0, n_0_1),
      std::make_pair(n_1_0, n_1_1));
  }

  template<typename N>
    requires(Numeric(N))
  eop::Matrix<N> matrix_2_2_multiply(const Matrix<N>& m_0, const Matrix<N>& m_1) {
    auto v_0_0 = m_0.first.first  * m_1.first.first  + m_0.first.second  * m_1.second.first;
    auto v_1_0 = m_0.second.first * m_1.first.first  + m_0.second.second * m_1.second.first;
    auto v_0_1 = m_0.first.first  * m_1.first.second + m_0.first.second  * m_1.second.second;
    auto v_1_1 = m_0.second.first * m_1.first.second + m_0.second.second * m_1.second.second;
    return make_matrix(v_0_0, v_0_1, v_1_0, v_1_1);
  }

  template<typename N, typename I>
    requires(Numeric(N) & Integer(I))
  N calculate(const eop::Coefficients<N>& coeff, const eop::Starting_elements<N> se, I n) {
    if (zero(n)) return se.second;
    if (one(n))  return se.first;
    auto m = make_matrix(coeff);
    auto result = power(m,
          n-1,
          matrix_2_2_multiply<N>);
    return result.first.first * se.first + result.first.second * se.second;
  }

} // namespace eop
