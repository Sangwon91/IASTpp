#pragma once

//#define ROOT_FINDER_DEBUG

#ifdef  ROOT_FINDER_DEBUG
    #define DEBUG(x) std::cout << "[DEBUG] " << \
                        "File: " << __FILE__ << ", " << \
                        "Line: " << __LINE__ << ", " << \
                        #x << " = " << (x) << std::endl;
#else
    #define DEBUG(x)
#endif

// add system of equations.
void
root_finder::add_eqn(func_t equation)
    {
    m_equations.push_back(equation);
    }

void
root_finder::reset_eqn()
    {
    m_equations.clear();
    }

// set initial guess.
void
root_finder::set_initial_guess(const vec& initial_guess)
    {
    m_variables = initial_guess;
    }

root_finder::vec
root_finder::solve()
    {
    // define constans.
    const size_t MAX_ITERS {100000};
    const double TOLF   {1.e-7};
    const double TOLX   {1.e-9};
    const double TOLMIN {1.e-8};

    DEBUG("check D.O.F")
    // check degrees of freedom.
    if (m_variables.n_elem > m_equations.size())
        {
        throw std::invalid_argument
            {"error: dim of var > dim of eqns in root_finder::solve()"};
        }
    else if (m_variables.n_elem < m_equations.size())
        {
        throw std::invalid_argument
            {"error: dim of var < dim of eqns in root_finder::solve()"};
        }

    const size_t dimension {m_variables.n_elem};
    auto& eqns = m_equations;
    auto& vars = m_variables;
    DEBUG(dimension)
    // define equation vector calculator.
    auto cal_eqn_values = [&eqns, &dimension](const vec& vars, vec& eqn_values)
        {
        DEBUG("begin cal_eqn_values")
        DEBUG(eqn_values.n_elem)
        for (size_t dim {}; dim < dimension; ++dim)
            {
            eqn_values(dim) = eqns[dim](vars);
            DEBUG(eqn_values(dim))
            }
        };

    vec eqn_values (arma::size(vars));
    cal_eqn_values(vars, eqn_values);
    DEBUG(eqn_values)
    auto objective = [](const vec& eqn_values) -> double
        {
        return 0.5 * arma::dot(eqn_values, eqn_values);
        };

    DEBUG(2.0 * objective(eqn_values))
    // define jacobian calculator.
    auto cal_jacobian =
    [&cal_eqn_values](vec& vars, const vec& eqn_values, mat& jacob)
        {
        const double EPS  {1.e-7};
              double temp {};
              double h    {};

        vec near_values {arma::size(eqn_values)};

        for (size_t i {}; i < vars.n_elem; ++i)
            {
            temp = vars(i);
            h = EPS * std::abs(temp);
            if (h == 0.0)
                h = EPS;

            vars(i) = temp + h;
            h = vars(i) - temp;

            cal_eqn_values(vars, near_values);
            vars(i) = temp;
            // calculate i col of jacobian.
            // [dF_i/dx_1 dF_i/dx_2 dF_i/dx_3 ... dF_i/dx_n]^t <- transpose.
            jacob.col(i) = (near_values - eqn_values) / h;
            }
        };

    // make d by d matrix.
    mat jacobian {arma::zeros<mat>(dimension, dimension)};

    DEBUG("Calculate Jacobian")
    cal_jacobian(vars, eqn_values, jacobian);
    vec gradient = arma::trans(jacobian) * eqn_values;

    DEBUG(2.0 * arma::norm(gradient))

    double obj_value = objective(eqn_values);

    bool is_spurious;
    // check if initial guess is root.
    if (arma::abs(eqn_values).max() < 0.01 * TOLF)
        {
        DEBUG(is_spurious)
        is_spurious = false;
        return vars;
        }

    double max_step {std::max(arma::norm(vars), static_cast<double>(dimension))};
    // move only same order of magnitude.
    // max_step *= 100.0;
    // remove maximum limit...
    // It was very very hamful!
    max_step = 1.0e300;
    DEBUG(max_step)

    vec old_vars;
    vec direction;
    double old_obj_value;
    // define line_search algorithm.
    auto line_search =
        [&vars, &old_vars, &obj_value, &old_obj_value,
         &gradient, &direction, &max_step, &objective,
         &cal_eqn_values, &eqn_values, &TOLX, &is_spurious]()
        {
        const double ALPHA {1.e-4};

        is_spurious = false;

        if (arma::norm(direction) > max_step)
            {
            direction *= max_step / arma::norm(direction);
            }

        double slope = arma::dot(gradient, direction);
        if (slope >= 0.0)
            throw std::runtime_error
                {"error: roundoff problem in line search of root_finder::solve()"};

        vec abs_vars = arma::abs(old_vars);
        abs_vars.transform([](double x) {return (x > 1.0 ? x : 1.0);});
        // same as maximum of |dFdx / x|
        double lam_in = TOLX / arma::abs(direction / abs_vars).max();

        double lam  {1.0};
        double lam2 {};
        double obj_value2 {};

        bool is_first_trial {true};

        while (true)
            {
            vars = old_vars + lam * direction;
            cal_eqn_values(vars, eqn_values);
            obj_value = objective(eqn_values);

            double temp_lam {};

            if (lam < lam_in)
                {
                vars = old_vars;
                is_spurious = true;
                return;
                }
            else if (obj_value < old_obj_value + ALPHA * lam * slope)
                {
                // new x sufficiently decrease.
                return;
                }
            else
                {
                if (is_first_trial)
                    {
                    // min of 2nd order interpolaed polynomial.
                    temp_lam = -slope / 2.0 / (obj_value - old_obj_value - slope);
                    is_first_trial = false;
                    }
                else
                    {
                    // min of 3rd order interpolated polynomial.
                    double rhs1 = obj_value  - old_obj_value - lam  * slope;
                    double rhs2 = obj_value2 - old_obj_value - lam2 * slope;

                    double a = (rhs1 / lam / lam - rhs2 / lam2 / lam2) / (lam - lam2);
                    double b = (-lam2 * rhs1 / lam / lam + lam * rhs2 / lam2 / lam2)
                               / (lam - lam2);

                    if (a == 0.0)
                        {
                        temp_lam = -slope / 2.0 / b;
                        }
                    else
                        {
                        // discriminant
                        double disc = b * b - 3.0 * a * slope;
                        if (disc < 0.0)
                            {
                            temp_lam = 0.5 * lam;
                            }
                        else if (b <= 0.0)
                            {
                            temp_lam = (-b + std::sqrt(disc)) / 3.0 / a;
                            }
                        else
                            {
                            temp_lam = -slope / (b + std::sqrt(disc));
                            }
                        // endif
                        if (temp_lam > 0.5 * lam)
                            {
                            // maximum limit: 50 % of previous lambda.
                            temp_lam = 0.5 * lam;
                            }
                        // end if
                        }
                    // endif
                    }
                // endif
                }
            // end if
            if (not std::isfinite(lam))
                {
                // I don't know why it occurs.
                throw std::runtime_error {"error: line search fails in root_finder::solve()"};
                }
            // update values.
            lam2 = lam;
            obj_value2 = obj_value;
            // lower limit: 10 % of previous lambda.
            // lambda is always positive because
            // we are searching on minimizing direction.
            lam = std::max(temp_lam, 0.1 * lam);
            }
        // end while
        };

    auto scale_vec = [](const vec& v, vec& scaled_v)
        {
        scaled_v = v;
        scaled_v.transform([](double x) {return std::max(x, 1.0);});
        };

    DEBUG("start iteration")
    // root finding iteration.
    for (size_t iter {}; iter < MAX_ITERS; ++iter)
        {
        // save iterations.
        m_iterations = iter + 1;
        // calculate gradient from jacobian matrix.
        cal_eqn_values(vars, eqn_values);
        cal_jacobian(vars, eqn_values, jacobian);
        gradient = jacobian.t() * eqn_values;

        old_vars = vars;
        old_obj_value = obj_value;
        // calculate Newton direction.
        bool is_solved = arma::solve(direction, jacobian, -eqn_values);
        if (not is_solved)
            {
            throw std::runtime_error {"error: jacobian is singluar in root_finer::solve()"};
            }
        // move along to the Newtone direction.
        line_search();
        // check convergence.
        if (arma::abs(eqn_values).max() < TOLF)
            {
            DEBUG(iter);
            is_spurious = false;
            break;
            }

        vec scaled_vars;
        scale_vec(arma::abs(vars), scaled_vars);

        if (is_spurious)
            {
            double den {std::max(obj_value, 5.0 * dimension)};

            if ((arma::abs(gradient) / scaled_vars).max() / den < TOLMIN)
                {
                is_spurious = true;
                throw std::runtime_error {"error: spurious solution in root_finder::solve()"};
                }
            else
                {
                is_spurious = false;
                }
            break;
            }

        if ((arma::abs(vars - old_vars) / scaled_vars).max() < TOLX)
            {
            break;
            }

        if (iter + 1 == MAX_ITERS)
            {
            throw std::runtime_error {"error: MAX_ITERS exceeded in root_finder::solve()"};
            }
        }
    DEBUG(vars);
    DEBUG(eqn_values);

    return vars;
    }

size_t
root_finder::get_iterations()
    {
    return m_iterations;
    }

#undef DEBUG
