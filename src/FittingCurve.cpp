#include "FittingCurve.hpp"

FittingCurve::FittingCurve() noexcept = default;

FittingCurve::FittingCurve(const std::vector<value_type>& coeffs)
    : coeffs(coeffs), derivativeValues(coeffs.size(), 0.) { }

FittingCurve::~FittingCurve() noexcept = default;

void
FittingCurve::setCoefficients(const std::vector<value_type>& coeffs)
{
    this->coeffs = coeffs;
    derivativeValues.resize(coeffs.size(), 0.);
}

FittingCurve::value_type
FittingCurve::evaluateAt(value_type x, const_pointer coeffs) const
{
    const_pointer p = (coeffs == nullptr)
        ? this->coeffs.data()
        : coeffs;

    return value(x, p);
}

void
FittingCurve::computeDerivativesAt(value_type x, const_pointer coeffs)
{
    const_pointer p = coeffs == nullptr
        ? this->coeffs.data()
        : coeffs;

    derivatives(x, p);
}