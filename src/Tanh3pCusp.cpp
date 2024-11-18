#include "Tanh3pCusp.hpp"

Tanh3pCusp::Tanh3pCusp(const std::vector<value_type> &coeffs, value_type c)
    : FittingCurve(coeffs), c(c) { }

void
Tanh3pCusp::setCoefficients(const std::vector<value_type> &coeffs, value_type c)
{
    this->c = c;
    FittingCurve::setCoefficients(coeffs);
}


FittingCurve::value_type
Tanh3pCusp::value(value_type x, const_pointer coeffs) const
{
    const auto a0 { coeffs[0] };
    const auto a1 { coeffs[1] };
    const auto a2 { coeffs[2] };

    return a0 + a1 * (1. - std::tanh((c - x) / (a2 * c)));
}

void
Tanh3pCusp::derivatives(value_type x, const_pointer coeffs)
{
    const auto a1 { coeffs[1] };
    const auto a2 { coeffs[2] };

    const auto dtmp      { a2 * c };
    const auto arg       { (c - x) / dtmp };
    const auto sech2_arg { std::pow(1. / std::cosh(arg), 2) };
    
    derivativeValues[0] = 1.;
    derivativeValues[1] = 1. - std::tanh(arg);
    derivativeValues[2] = a1 * (c - x) * sech2_arg / (a2 * dtmp);
} 