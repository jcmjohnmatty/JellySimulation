#ifndef __LINALG_H_
#define __LINALG_H_ /* __LINALG_H_ */

/*
 * FOR ALL FUNCTIONS IN THIS FILE THE USER IS RESPOSIBLE FOR free()ing THE
 * MEMORY POINTED TO THAT IS RETURNED FROM THESE FUNCTIONS (matrix_delete(*)).
 */

/*
 * ALL OF THESE FUNCTIONS EXPECT HOMOGENEOUS COORDINATES WHERE IT MATTERS.
 */

/**
 * @brief Create a new matrix with all entries set to zero.
 * @param rows The number of rows.
 * @param cols The number of cols.
 * @return The newly created matrix that is rowsxcols filled with zeroes.
 */
float**
matrix_new (int rows, int cols);

/**
 * @brief Create an identity matrix.
 * @param rows The number of rows.
 * @param cols The number of cols.
 * @return The newly created identity matrix that is rowsxcols.
 */
float**
matrix_identity (int rows, int cols);

/**
 * @brief Delete a matrix.
 * @param m The matrix to delete.
 * @param r The number of rows in m.
 * @param c The number of cols in m.
 */
void
matrix_delete (float** m, int r, int c);

/**
 * @brief Print a matrix.
 * @param m The matrix to print.
 * @param r The number of rows in m.
 * @param c The number of cols in m.
 */
void
matrix_print (float** m, int r, int c);

/**
 * @brief Clone a matrix.
 * @param m The matrix to clone.
 * @param r The number of rows in m.
 * @param c The number of cols in m.
 * @return A clone of m.
 */
float**
matrix_clone (float** m, int r, int c);

/**
 * @brief Multiply two matrices together.
 * @param m1 The first matrix.
 * @param m1_r The number of rows in m1.
 * @param m1_c The number of cols in m1.
 * @param m2 The second matrix.
 * @param m2_r The number of rows in m2.
 * @param m2_c The number of cols in m2.
 * @return A new matrix that is the result of the multiplication.
 */
float**
matrix_matrix_multiply (float** m1, int m1_r, int m1_c, float** m2, int m2_r,
                        int m2_c);

/**
 * @brief Multiply a matrix and a vector.
 * @param m The matrix.
 * @param m_r The number of rows in m.
 * @param m_c The number of cols in m.
 * @param v The vector.
 * @param v_c The number of columns in v.
 * @return A new vector that is the result of the multiplication.
 */
float*
matrix_vector_multiply (float** m, int m_r, int m_c, float* v, int v_c);

/**
 * @brief Multiply a matrix and a real number (this is possibly spelled horribly
 *        wrong, sorry).
 * @param m The matrix.
 * @param m_r The number of rows in m.
 * @param m_c The number of cols in m.
 * @param n The number.
 * @return A new matrix that is the result of the multiplication.
 */
float**
matrix_scaler_multiply (float** m, int m_r, int m_c, float n);

/**
 * @brief Calculate the determinant of a matrix.
 * @param m The matrix to calculate the determinant of.
 * @param m_r The number of rows in m.
 * @param m_c The number of cols in m.
 * @return The determinant of m.
 */
float
matrix_det (float** m, int m_r, int m_c);

/**
 * @brief Calculate the cofactor matrix of a matrix.
 * @param m The matrix to calculate the cofactor matrix of.
 * @param m_r The number of rows in m.
 * @param m_c The number of cols in m.
 * @return The cofactor matrix of m.
 */
float**
matrix_cof (float** m, int m_r, int m_c);

/**
 * @brief Calculate the transpose of a matrix.
 * @param m The matrix to calculate the transpose of.
 * @param m_r The number of rows in m.
 * @param m_c The number of cols in m.
 * @return The transpose of m.
 */
float**
matrix_transpose (float** m, int m_r, int m_c);

/**
 * @brief Calculate the adjoint of a matrix.
 * @param m The matrix to calculate the adjoint of.
 * @param m_r The number of rows in m.
 * @param m_c The number of cols in m.
 * @return The adjoint of m.
 */
float**
matrix_adj (float** m, int m_r, int m_c);

/**
 * @brief Find the inverse of a matrix.
 * @param m The matrix.
 * @param m_r The number of rows in m.
 * @param m_c The number of columns in m.
 * @return The inverse of m.
 */
float**
matrix_inverse (float** m, int m_r, int m_c);

/**
 * @brief Print a vector out.
 * @param v The vector to print.
 * @parm len The length of v.
 */
void
vector_print (float* v, int len);

/**
 * @brief Clone a vector.
 * @param v The vector to clone.
 * @param len The length of v.
 * @return A freshly malloc()'d clone of v.
 */
float*
vector_clone (float* v, int len);

/**
 * @brief Take the cross product of two vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @param w The number of cols in v1 and v2.
 * @return The result.
 */
float*
vector_cross (float* v1, float* v2, int w);

/**
 * @brief Take the dot product of two vectors.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @param w The number of cols in v1 and v2.
 * @return The result of the multiplication.
 */
float
vector_dot (float* v1, float* v2, int w);

/**
 * @brief Add 2 vector.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @param 2 The number of cols in v1 and v2.
 * @return The result.
 */
float*
vector_add (float* v1, float* v2, int w);
/**
 * @brief Subtract 2 vector.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @param w The number of cols in v1 and v2.
 * @return The result (v1 - v2).
 */
float*
vector_sub (float* v1, float* v2, int w);

/**
 * @breif Scale a vector.
 * @param v1 The vector to scale.
 * @param k The scale factor.
 * @param w The number of cols in v1.
 * @return The result.
 */
float*
vector_scale (float* v1, float k, int w);

/**
 * @brief Calculate the magnitude of a vector (ignoring last coordinate).
 * @param v The vector to calculate the magnitude of.
 * @param w The number of columns in v.
 * @return The magnitude of v.
 */
float
vector_magnitude (float* v, int w);

/**
 * @brief Normalize a vector in-place (ignoring the last coordinate).
 * @param v The vector to normalize.
 * @param w The number of columns in v.
 */
void
vector_normalize (float* v, int w);

/**
 * @breif Check if 2 vectors are equal.
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @param w The number of cols in v1.
 * @return v1 == v2.
 */
int
vector_equal (float* v1, float* v2, int w);

#endif /* __LINALG_H_ */
