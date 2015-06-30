#pragma once

#include <cmath>
#include <map>

using namespace std;

double max(double a, double b);

double min(double a, double b);

float Norm_cdf(const double& z);

double Norm_pdf(double &z);

struct PricingParam
{
    double spot_price,strike_price;
    float free_rate,yield_rate,time_to_maturity,volatility;
    int option_type,value_method,multiplier;
	map<string,string> other_param;
};

double bls_vanila_option(PricingParam &param);

double bls_vanila_delta(PricingParam &param);

double bls_vanila_gamma(PricingParam &param);

double bls_vanila_theta(PricingParam &param);

double bls_vanila_vega(PricingParam &param);
