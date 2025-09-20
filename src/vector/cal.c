#include "../../include/minirt.h"

// This is a simplified factorial function for the example
double	ft_factorial(int n)
{
	if (n == 0)
		return (1);
	return (n * ft_factorial(n - 1));
}

// Simplified Taylor series implementation for sine
double	ft_sin(double x)
{
	double	result;
	double	term;
	int		i;

	result = 0.0;
	i = 0;
	while (i < 10)
	{
		term = ft_pow(-1, i) * ft_pow(x, 2 * i + 1) / ft_factorial(2 * i + 1);
		result += term;
		i++;
	}
	return (result);
}

// Approximates the cosine of x using a Taylor series.
double	ft_cos(double x)
{
	double	result;
	int		i;
	int		sign;

	result = 0.0;
	i = 0;
	while (i < 10)
	{
		if (i % 2 == 0)
			sign = 1;
		else
			sign = -1;
		result += sign * ft_pow(x, 2 * i) / ft_factorial(2 * i);
		i++;
	}
	return (result);
}

// Calculates the tangent of x using ft_sin and ft_cos.
double	ft_tan(double x)
{
	double cos_val;

	cos_val = ft_cos(x);
	if (cos_val == 0.0)
		return (INFINITY);
	return (ft_sin(x) / cos_val);
}