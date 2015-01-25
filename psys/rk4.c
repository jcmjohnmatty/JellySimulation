#include "rk4.h"

float
nderiv (float (*f) (float x, float y), float x, float y, float h)
{
  float k1;
  float k2;
  float k3;
  float k4;
  float y_;

  k1 = f (x, y);
  k2 = f (x + h / 2, y + h * k1 / 2);
  k3 = f (x + h / 2, y + h * k2 / 2);
  k4 = f (x + h, y + h * k3);

  /* Compute the next value of y. */
  y_ = y + h * (0.1666666666666667 * (k1 + k4) + 0.3333333333333333 * (k2
                                                                       + k3));

  return y_;
}
