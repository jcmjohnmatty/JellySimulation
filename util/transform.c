#include <math.h>
#include <stdlib.h>
#include "transform.h"

float**
rotation_axis (float theta, int ax)
{
  float u[3];

  if (ax == X_AXIS) u[0] = 1;
  else u[0] = 0;
  if (ax == Y_AXIS) u[1] = 1;
  else u[1] = 0;
  if (ax == Z_AXIS) u[2] = 1;
  else u[2] = 0;

  return rotation_arbitrary (theta, u);
}

float**
rotation_arbitrary (float theta, float u[3])
{
  int i;
  float cos_theta = cos (theta);
  float sin_theta = sin (theta);
  float** res = malloc (sizeof (float*) * 4);

  for (i = 0; i < 4; i++)
    {
      res[i] = calloc (4, sizeof (float));

      switch (i)
        {
        case 0:
          res[0][0] = cos_theta + pow (u[0], 2) * (1 - cos_theta);
          res[0][1] = u[0] * u[1] * (1 - cos_theta) - u[2] * sin_theta;
          res[0][2] = u[0] * u[2] * (1 - cos_theta) + u[1] * sin_theta;
          res[0][3] = 0;
          break;
        case 1:
          res[1][0] = u[1] * u[0] * (1 - cos_theta) + u[2] * sin_theta;
          res[1][1] = cos_theta + pow(u[1], 2) * (1 - cos_theta);
          res[1][2] = u[1] * u[2] * (1 - cos_theta) - u[0] * sin_theta;
          res[1][3] = 0;
          break;
        case 2:
          res[2][0] = u[2] * u[0] * (1 - cos_theta) - u[1] * sin_theta;
          res[2][1] = u[2] * u[1] * (1 - cos_theta) + u[0] * sin_theta;
          res[2][2] = cos_theta + pow (u[2], 2) * (1 - cos_theta);
          res[2][3] = 0;
          break;
        case 3:
          res[3][0] = res[3][1] = res[3][2] = 0;
          res[3][3] = 1;
          break;
        default:
          break;
        }
    }

  return res;
}

float**
scale (float ss[3])
{
  int i;
  int j;
  float** res;

  res = malloc (sizeof (float*) * 4);

  for (i = 0; i < 4; i++)
    {
      res[i] = calloc (4, sizeof (float));
      for (j = 0; j < 4; j++)
        {
          if (i == j)
            {
              if (j != 3)
                res[i][j] = ss[i];
              else
                res[i][j] = 1;
            }
        }
    }

  return res;
}

float**
translation (float ds[4])
{
  int i;
  int j;
  float** res = malloc (sizeof (float*) * 4);

  for (i = 0; i < 4; i++)
    {
      res[i] = calloc (4, sizeof (float));
      for (j = 0; j < 4; j++)
        {
          if (j == i)
            res[i][j] = 1;
          else
            if (j == 3)
              res[i][j] = ds[i];
        }
    }

  return res;
}
