#include "Exp3pCusp.hpp"

Exp3pCusp::Exp3pCusp(const std::vector<value_type> &coeffs, value_type c)
    : FittingCurve(coeffs), c(c) { }

void
Exp3pCusp::setCoefficients(const std::vector<value_type> &coeffs, value_type c)
{
    this->c = c;
    FittingCurve::setCoefficients(coeffs);
}


FittingCurve::value_type
Exp3pCusp::value(value_type x, const_pointer coeffs) const
{
    const auto a0 { coeffs[0] };
    const auto a1 { coeffs[1] };
    const auto a2 { coeffs[2] };

    return a0 + a1 * std::exp(a2 * c / x);
}

void
Exp3pCusp::derivatives(value_type x, const_pointer coeffs)
{
    auto a0 { coeffs[0] };
    auto a1 { coeffs[1] };
    auto a2 { coeffs[2] };

    auto dtmp { c / x };
    auto dexp { std::exp(a2 * dtmp) };

    derivativeValues[0] = 1.;
    derivativeValues[1] = dexp;
    derivativeValues[2] = a1 * dtmp * dexp;
} 