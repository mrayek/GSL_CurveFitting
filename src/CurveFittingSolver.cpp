#include "CurveFittingSolver.hpp"

CurveFittingSolver::CurveFittingSolver(
    const gsl_multifit_fdfsolver_type* T,
    std::size_t maxIter,
    double xTol,
    double gTol
) : solverType(T), maxIter(maxIter), xTol(xTol), gTol(gTol) { }

CurveFittingSolver::~CurveFittingSolver()
{
    if (solver) { gsl_multifit_fdfsolver_free(solver); }
}

void
CurveFittingSolver::initialize(const_span x, const_span y, const_span w,
    FittingCurve& fc)
{
    fcd = std::make_unique<CurveFittingData>(x, y, w, fc);

    f.f      = model_f;
    f.df     = model_df;
    f.fdf    = model_fdf;
    f.n      = fcd->size();
    f.p      = fcd->fc.numCoefficients();
    f.params = fcd.get();

    solver   = gsl_multifit_fdfsolver_alloc(solverType, f.n, f.p);
    unknowns = gsl_vector_view_array(fcd->fc.coefficients(), f.p);
}

void
CurveFittingSolver::run()
{
    set(); // resetting solver's state

    int status;
    std::size_t iter { 0 };

    /* Main loop */
    do
    {
        status = gsl_multifit_fdfsolver_iterate(solver);

        if (status) { break; }

        status = gsl_multifit_test_delta(
            solver->dx,
            solver->x,
            xTol,
            gTol
        );
    }
    while (status == GSL_CONTINUE && ++iter < maxIter);
}

void
CurveFittingSolver::getCoefficients(double* coeffs) const
{
    for (std::size_t i = 0; i < f.p; i++)
    {
        coeffs[i] = gsl_vector_get(solver->x, i);
    }
}

void
CurveFittingSolver::set()
{
    gsl_multifit_fdfsolver_set(solver, &f, &unknowns.vector);
}

int
CurveFittingSolver::model_f(const gsl_vector* unknowns, void* data,
    gsl_vector* ov)
{
    const CurveFittingData* d = static_cast<CurveFittingData*>(data);

    for (int i = 0; i < d->size(); i++)
    {
        const double value {
            d->w[i] * (d->fc.evaluateAt(d->x[i], unknowns->data) - d->y[i]) 
        };

        gsl_vector_set(ov, i, value);
    }

    return GSL_SUCCESS;
}

int
CurveFittingSolver::model_df(const gsl_vector* unknowns, void* data,
    gsl_matrix* om)
{
    const CurveFittingData* d = static_cast<CurveFittingData*>(data);

    for (std::size_t i = 0; i < d->size(); i++)
    {
        /* Compute partial derivatives */
        d->fc.computeDerivativesAt(d->x[i], unknowns->data);

        for (std::size_t j = 0; j < d->fc.numCoefficients(); j++)
        {
            const double value { d->w[i] * d->fc.getDerivative(j) };

            gsl_matrix_set(om, i, j, value);
        }
    }

    return GSL_SUCCESS;
}

int
CurveFittingSolver::model_fdf(const gsl_vector* unknowns, void* data,
    gsl_vector* f, gsl_matrix* j)
{
    model_f(unknowns, data, f);
    model_df(unknowns, data, j);

    return GSL_SUCCESS;
}