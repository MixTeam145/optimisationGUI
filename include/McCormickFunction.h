#pragma once
#include "Function.h"

/**
* The McCormick function class (2-dimentional)
*/
class McCormickFunction : public Function
{
public:
	McCormickFunction();

	~McCormickFunction();

	/**
	* Calculates the value of the function at a given point
	*
	* \param x Point
	*/
	double eval(const VectorXd& x) const override;

	/**
	* Calculates the gradient of the function at a given point
	*
	* \param x Point
	*/
	VectorXd grad(const VectorXd& x) const override;

	/**
	* Prints the function to given output stream
	* 
	* \param os Output stream
	*/
	void print_function(std::ostream& os) const override;
};

