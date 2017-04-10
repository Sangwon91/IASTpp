#pragma once

class root_finder
    {
public:
    typedef arma::vec vec;
    typedef arma::mat mat;
    typedef std::function<double(const vec&)> func_t;

    void add_eqn(func_t equation);
    void reset_eqn();
    void set_initial_guess(const vec& initial_guess);

    vec solve();

    size_t get_iterations();
private:
    std::vector<func_t> m_equations {};
    vec m_variables {};
    size_t m_iterations {};
    };
