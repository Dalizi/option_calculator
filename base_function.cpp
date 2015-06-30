#include "base_function.h"

double max(double a, double b)
{
	if (a>b)
		return a;
	else
		return b;
}

double min(double a, double b)
{
	if (a<b)
		return a;
	else
		return b;
}

float Norm_cdf(const double& z) { 
	if (z > 6.0) { return 1.0; }; // this guards against overflow 
	if (z < -6.0) { return 0.0; }; 
	double b1 = 0.31938153; 
	double b2 = -0.356563782; 
	double b3 = 1.781477937; 
	double b4 = -1.821255978; 
	double b5 = 1.330274429; 
	double p = 0.2316419; 
	double c2 = 0.3989423; 
	double a=fabs(z); 
	double t = 1.0/(1.0+a*p); 
	double b = c2*exp((-z)*(z/2.0)); 
	double n = ((((b5*t+b4)*t+b3)*t+b2)*t+b1)*t; 
	n = 1.0-b*n; 
	if ( z < 0.0 ) n = 1.0 - n; 
	return n; 
}; 

double Norm_pdf(double &z)
{
	double c2 = 0.3989422804*exp((-z)*(z / 2.0));
	return c2;
}
//Black-Scholes Formula for Vanila European Option
double bls_vanila_option(PricingParam &param)
{
    if (param.time_to_maturity > 0.0005)
	{
        double time_sqrt = sqrt(param.time_to_maturity);
        double d1 = (log(param.spot_price / param.strike_price) + (param.free_rate - param.yield_rate)*param.time_to_maturity) / (param.volatility*time_sqrt) + 0.5*param.volatility*time_sqrt;
        double d2 = d1 - (param.volatility*time_sqrt);

        if (param.option_type == 1)
		{
            return param.spot_price*Norm_cdf(d1)*exp(-param.yield_rate*param.time_to_maturity) - param.strike_price*exp(-param.free_rate*param.time_to_maturity)*Norm_cdf(d2);
		}
        else if (param.option_type == 2)
		{
            return -param.spot_price*Norm_cdf(-d1)*exp(-param.yield_rate*param.time_to_maturity) + param.strike_price*exp(-param.free_rate*param.time_to_maturity)*Norm_cdf(-d2);
		}
	}
	else
	{
        if (param.option_type == 1)
		{
            return max(param.spot_price - param.strike_price,0);
		}
        else if (param.option_type == 2)
		{
            return max(0,-param.spot_price + param.strike_price);
		}
	}

}

//Black-Scholes Formula for Vanila European Option Delta
double bls_vanila_delta(PricingParam &param)
{
	double result = 0;
    if (param.time_to_maturity > 0.0005)
	{
        double time_sqrt = sqrt(param.time_to_maturity);
        double d1 = (log(param.spot_price / param.strike_price) + (param.free_rate - param.yield_rate)*param.time_to_maturity) / (param.volatility*time_sqrt) + 0.5*param.volatility*time_sqrt;

        if (param.option_type == 1)
		{
            result = Norm_cdf(d1)*exp(-param.yield_rate*param.time_to_maturity);
		}
        else if (param.option_type == 2)
		{
            result = -Norm_cdf(-d1)*exp(-param.yield_rate*param.time_to_maturity);
		}
	}
	else
	{
        if (param.option_type == 1)
		{
            result = param.spot_price > param.strike_price? 1:0;
		}
        else if (param.option_type == 2)
		{
            result = param.spot_price > param.strike_price ? 0 : -1;
		}
	}
	return result;
}

double bls_vanila_gamma(PricingParam &param)
{
	double result = 0;
    if (param.time_to_maturity > 0.0005)
	{
        double time_sqrt = sqrt(param.time_to_maturity);
        double d1 = (log(param.spot_price / param.strike_price) + (param.free_rate - param.yield_rate)*param.time_to_maturity) / (param.volatility*time_sqrt) + 0.5*param.volatility*time_sqrt;


        result = Norm_pdf(d1) / (param.spot_price*param.volatility*time_sqrt);
	}
	else
	{
		result = 0;
	}
	return result;
}

double bls_vanila_theta(PricingParam &param)
{
	double result = 0;
    if (param.time_to_maturity > 0.0005)
	{
        double time_sqrt = sqrt(param.time_to_maturity);
        double d1 = (log(param.spot_price / param.strike_price) + (param.free_rate - param.yield_rate)*param.time_to_maturity) / (param.volatility*time_sqrt) + 0.5*param.volatility*time_sqrt;
        double d2 = d1 - (param.volatility*time_sqrt);


        if (param.option_type == 1)
		{
            result = -param.spot_price*Norm_cdf(d1)* param.volatility / (2 * time_sqrt) - param.free_rate*param.strike_price*exp(-param.free_rate*param.time_to_maturity)*Norm_cdf(d2);
		}
        else if (param.option_type == 2)
		{
            result = -param.spot_price*Norm_cdf(d1)* param.volatility / (2 * time_sqrt) + param.free_rate*param.strike_price*exp(-param.free_rate*param.time_to_maturity)*Norm_cdf(-d2);
		}
	}
	else
	{
		result = 0;
	}
	return result/256;
}

double bls_vanila_vega(PricingParam &param)
{
	double result = 0;
    if (param.time_to_maturity > 0.0005)
	{
        double time_sqrt = sqrt(param.time_to_maturity);
        double d1 = (log(param.spot_price / param.strike_price) + (param.free_rate - param.yield_rate)*param.time_to_maturity) / (param.volatility*time_sqrt) + 0.5*param.volatility*time_sqrt;
        result = Norm_pdf(d1) *(param.spot_price*time_sqrt)/100;
	}
	else
	{
		result = 0;
	}
	return result;
}
