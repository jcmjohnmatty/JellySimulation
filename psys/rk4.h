#ifndef __RK4_H_
#define __RK4_H_ /* __RH4_H_ */

/**
 * @brief Compute the (approximate) derivitave of the given function at the
 *        given point using RK4.
 * @param f The function to compute the numerical derivitave of at a point
 *        (something of the form y' = f(x, y).
 * @param x The x-coordinate of the function.
 * @param y The previous value of y.
 * @param h The step size to use.
 * @return The next value of y.
 */
float
nderiv (float (*f) (float x, float y), float x, float y, float h);

#endif /* __RK4_H_ */
