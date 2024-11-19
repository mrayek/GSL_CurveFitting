#ifndef CURVE_FITTING_SOLVER_HPP
#define CURVE_FITTING_SOLVER_HPP

#include <vector>
#include <span>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include <gsl/gsl_multifit_nlin.h>

#include "FittingCurve.hpp"

class CurveFittingSolver
{
    public:
        using const_span = const std::span<const double>;

        CurveFittingSolver(
            const gsl_multifit_fdfsolver_type* T = gsl_multifit_fdfsolver_lmsder,
            std::size_t maxIter = 100,
            double xTol = 1e-08,
            double gTol = 1e-08
        );

        CurveFittingSolver(CurveFittingSolver&);

        CurveFittingSolver(CurveFittingSolver&&) noexcept;

        ~CurveFittingSolver();

        CurveFittingSolver& operator=(CurveFittingSolver&);

        CurveFittingSolver& operator=(CurveFittingSolver&&) noexcept;

        void
        initialize(const_span x, const_span y, const_span w, FittingCurve& fc);

        void
        run();

        void getCoefficients(double* coeffs) const;

    private:
        struct CurveFittingData
        {
            const_span x;
            const_span y;
            const_span w;
            FittingCurve& fc;

            CurveFittingData(const_span x, const_span y, const_span w,
                FittingCurve& fc) : x(x), y(y), w(w), fc(fc)
            {
                if (x.size() != y.size() || x.size() != w.size())
                {
                    const std::string msg {
                        "Error, arguments size mismtach\n"
                        "\tx: " + std::to_string(x.size()) + ",\n"
                        "\ty: " + std::to_string(y.size()) + ",\n"
                        "\tw: " + std::to_string(w.size()) + ",\n"
                    };

                    throw std::invalid_argument(msg);
                }
            }

            inline std::size_t size() const { return x.size(); }
        };

        const gsl_multifit_fdfsolver_type* solverType;
        gsl_multifit_fdfsolver*            solver;
        gsl_vector_view                    unknowns;
        gsl_multifit_function_fdf          f;

        const std::size_t maxIter;
        const double      xTol;
        const double      gTol;

        std::unique_ptr<CurveFittingData> fcd;
        
        void
        set();

        static int
        model_f(const gsl_vector* unknowns, void* data, gsl_vector* ov);

        static int
        model_df(const gsl_vector* unknowns, void* data, gsl_matrix* om);

        static int
        model_fdf(const gsl_vector* unknowns, void* data, gsl_vector* f,
            gsl_matrix* j);
};


#endif