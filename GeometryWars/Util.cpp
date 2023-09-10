#include "Util.h"
#include <cmath>

namespace Util {
	double sqrt(float x)
	{
		if (x < 0.0) {
			return 0.0;
		}

		if (x == 0.0) {
			return 0.0;
		}

		double guess = x; // Initial guess
		double epsilon = 1e-6; // Tolerance for convergence

		while (true) {
			double next_guess = 0.5 * (guess + x / guess); // Newton-Raphson formula
			if (std::abs(next_guess - guess) < epsilon) {
				return next_guess;
			}
			guess = next_guess;
		}
	}
}