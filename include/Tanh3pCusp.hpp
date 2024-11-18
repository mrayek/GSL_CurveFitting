#ifndef TANH3PCUSP_HPP
#define TANH3PCUSP_HPP

#include "FittingCurve.hpp"

class Tanh3pCusp : public FittingCurve
{
    public:
        using FittingCurve::FittingCurve;

        Tanh3pCusp(const std::vector<value_type> &coeffs, value_type c);

        void
        setCoefficients(const std::vector<value_type> &coeffs, value_type c);

    private:
        value_type c;

        value_type
        value(value_type x, const_pointer coeffs) const override;

        void
        derivatives(value_type x, const_pointer coeffs) override;
};

#endif