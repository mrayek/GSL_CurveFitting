#ifndef FITTING_CURVE_HPP
#define FITTING_CURVE_HPP

#include <vector>
#include <cmath>

class FittingCurve
{
    public:
        using value_type      = double;
        using pointer         = double*;
        using const_pointer   = const double*;
        using reference       = double&;
        using const_reference = const double&;
        using size_type       = std::size_t;

        FittingCurve() noexcept;

        explicit FittingCurve(const std::vector<value_type>& coeffs);

        virtual ~FittingCurve() noexcept;

        void
        setCoefficients(const std::vector<value_type>& coeffs);

        value_type
        evaluateAt(value_type x, const_pointer coeffs = nullptr) const;

        void
        computeDerivativesAt(value_type x, const_pointer coeffs = nullptr);

        inline std::vector<value_type>
        getCoefficients() const noexcept
        { return coeffs; }

        inline const_reference
        getCoefficient(size_type index) const
        { return coeffs.at(index); }

        inline value_type
        getDerivative(size_type index) const
        { return derivativeValues.at(index); }

        inline pointer
        coefficients() noexcept
        { return coeffs.data(); }

        inline const_pointer
        coefficients() const noexcept
        { return coeffs.data(); }

        inline size_type
        numCoefficients() const noexcept
        { return coeffs.size(); }

    protected:
        std::vector<value_type> coeffs;
        std::vector<value_type> derivativeValues;

        virtual value_type value(value_type x, const_pointer coeffs) const = 0;
        virtual void derivatives(value_type x, const_pointer coeffs) = 0;
};

#endif