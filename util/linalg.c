#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "linalg.h"

float**
matrix_new (int rows, int cols)
{
  int i;
  float** res;

  res = malloc (sizeof (float*) * rows);

  for (i = 0; i < rows; i++)
    res[i] = calloc (cols, sizeof (float));

  return res;
}

float**
matrix_identity (int rows, int cols)
{
  int i;
  float** res;

  res = matrix_new (rows, cols);

  for (i = 0; i < (rows < cols ? rows : cols); i++)
    res[i][i] = 1;

  return res;
}

void
matrix_delete (float** m, int r, int c)
{
  int i;
  for (i = 0; i < r; i++)
    free (m[i]);
  free (m);
}

void
matrix_print (float** m, int r, int c)
{
  int i;
  int j;

  for (i = 0; i < r; i++)
    {
      for (j = 0; j < c; j++)
        {
          printf ("%f ", m[i][j]);
        }
      printf ("\n");
    }
}

float**
matrix_clone (float** m, int r, int c)
{
  int i;
  int j;
  float** res = malloc (sizeof (float*) * r);

  for (i = 0; i < r; i++)
    {
      res[i] = calloc (c, sizeof (float));
      for (j = 0; j < c; j++)
        res[i][j] = m[i][j];
    }

  return res;
}

float**
matrix_matrix_multiply (float** m1, int m1_r, int m1_c, float** m2, int m2_r,
                        int m2_c)
{
  int i;
  int j;
  int k;
  float** res = malloc (sizeof (float*) * m1_r);

  assert (m1_c == m2_r);

  for (i = 0; i < m1_r; i++)
    {
      res[i] = calloc (m2_c, sizeof (float));
      for (j = 0; j < m2_r; j++)
        for (k = 0; k < m2_c; k++)
          res[i][k] += m1[i][j] * m2[j][k];
    }

  return res;
}

float*
matrix_vector_multiply (float** m, int m_r, int m_c, float* v, int v_c)
{
  int i;
  int j;
  float tmp;
  float* res = calloc (m_r, sizeof (float));

  assert (m_c == v_c);

  for (i = 0; i < m_r; i++)
    {
      tmp = 0;
      for (j = 0; j < m_c; j++)
        tmp += m[i][j] * v[j];

      res[i] = tmp;
    }

  return res;
}

float**
matrix_scaler_multiply (float** m, int m_r, int m_c, float n)
{
  int i;
  int j;
  float** res;

  res = malloc (sizeof (float*) * m_r);

  for (i = 0; i < m_r; i++)
    {
      res[i] = calloc (m_c, sizeof (float));
      for (j = 0; j < m_c; j++)
        res[i][j] = n * m[i][j];
    }

  return res;
}

float**
submatrix_excluding (float** m, int m_r, int m_c, int cur_r, int cur_c)
{
  int j;
  int k;
  int tmp_j = 0;
  int tmp_k = 0;
  float** tmp = matrix_new (m_r - 1, m_c - 1);

  /* Create the submatrix. */
  /* Start at row 2 since we skip the first row. */
  for (j = 0; j < m_r; j++)
    {
      if (j != cur_r)
        {
          tmp_k = 0;
          for (k = 0; k < m_c; k++)
            {
              /* If we are not in the current column, assign the current
                 entry to the next entry in the matrix. */
              if (k != cur_c)
                {
                  tmp[tmp_j][tmp_k] = m[j][k];
                  tmp_k++;
                }
            }
          tmp_j++;
        }
    }

  return tmp;
}

float
matrix_det (float** m, int m_r, int m_c)
{
  int i;
  float res=0.0f;
  float** tmp;

  assert (m_r == m_c);
  assert ((m_r > 1) && (m_c > 1));

  if ((m_r == 2) && (m_c == 2))
    res = m[0][0] * m[1][1] - m[0][1] * m[1][0];
  else
    {
      for (i = 0; i < m_c; i++)
        {
          tmp = submatrix_excluding (m, m_r, m_c, 0, i);
          res += pow (-1, i) * m[0][i] * matrix_det (tmp, m_r - 1, m_c - 1);
          matrix_delete (tmp, m_r - 1, m_c - 1);
        }
    }

  return res;
}

float**
matrix_cof (float** m, int m_r, int m_c)
{
  int i;
  int j;
  float** tmp;
  float** res = matrix_new (m_r, m_c);

  for (i = 0; i < m_r; i++)
    {
      for (j = 0; j < m_c; j++)
        {
          tmp = submatrix_excluding (m, m_r, m_c, i, j);
          res[i][j] = pow (-1, i + j) * matrix_det (tmp, m_r - 1, m_c - 1);
          matrix_delete (tmp, m_r - 1, m_c - 1);
        }
    }

  return res;
}

float**
matrix_transpose (float** m, int m_r, int m_c)
{
  int i;
  int j;
  float** transpose = matrix_new (m_c, m_r);

  for (i = 0; i < m_r; i++)
    for (j = 0; j < m_c; j++)
      transpose[j][i] = m[i][j];

  return transpose;
}

float**
matrix_adj (float** m, int m_r, int m_c)
{
  float** cof;
  float** adj;

  cof = matrix_cof (m, m_r, m_c);
  adj = matrix_transpose (cof, m_r, m_c);
  matrix_delete (cof, m_r, m_c);

  return adj;
}

float**
matrix_inverse (float** m, int m_r, int m_c)
{
  float** inv;
  float** adj;

  adj = matrix_adj (m, m_r, m_c);
  inv = matrix_scaler_multiply (adj, m_r, m_c, 1 / matrix_det (m, m_r, m_c));

  matrix_delete (adj, m_r, m_c);

  return inv;
}

void
vector_print (float* v, int len)
{
  int i;

  for (i = 0; i < len; i++)
    printf ("%f\n", v[i]);
}

float*
vector_clone (float* v, int len)
{
  int i;
  float* res;

  res = malloc (sizeof (float) * len);
  for (i = 0; i < len; i++)
    res[i] = v[i];

  return res;
}

float*
vector_cross (float* v1, float* v2, int w)
{
  float* new_values;

  assert (w == 3);

  new_values = calloc (w + 1, sizeof (float));
  new_values[0] = v1[1] * v2[2] - v1[2] * v2[1];
  new_values[1] = v1[2] * v2[0] - v1[0] * v2[2];
  new_values[2] = v1[0] * v2[1] - v1[1] * v2[0];
  new_values[3] = 1;
  return new_values;
}

float
vector_dot (float* v1, float* v2, int w)
{
  int i;
  float res = 0;

  for (i = 0; i < w - 1; i++)
    res += v1[i] * v2[i];
  return res;
}

float*
vector_add (float* v1, float* v2, int w)
{
  int i;
  float* new_vals;

  new_vals = calloc (w, sizeof (float));
  for (i = 0; i < w - 1; i++)
    new_vals[i] = v1[i] + v2[i];
  new_vals[w - 1] = 1;
  return new_vals;
}

float*
vector_sub (float* v1, float* v2, int w)
{
  int i;
  float* new_vals;

  new_vals = calloc (w, sizeof (float));
  for (i = 0; i < w - 1; i++)
    new_vals[i] = v1[i] - v2[i];
  new_vals[w - 1] = 1;
  return new_vals;
}

float*
vector_scale (float* v1, float k, int w)
{
  int i;
  float* new_vals;

  new_vals = calloc (w, sizeof (float));
  for (i = 0; i < w - 1; i++)
    new_vals[i] = k * v1[i];
  new_vals[w - 1] = 1;
  return new_vals;
}

float
vector_magnitude (float* v, int w)
{
  int i;
  float mag = 0.0f;

  for (i = 0; i < w - 1; i++)
    mag += pow (v[i], 2);

  return sqrt (mag);
}

void
vector_normalize (float* v, int w)
{
  int i;
  float mag = vector_magnitude (v, w);

  for (i = 0; i < w - 1; i++)
    v[i] /= mag;
}

int
vector_equal (float* v1, float* v2, int w)
{
  int i;
  int is_equal = 1;

  for (i = 0; i < w; i++)
    is_equal = is_equal && (v1[i] == v2[i]);

  return is_equal;
}
