#ifndef __INDEXUTIL_H_
#define __INDEXUTIL_H_ /* __INDEXUTIL_H_ */

/*
 * These functions, together, will allow indexing into an array of vertices
 * allowing for wrapping back around to the beginning of the array (getting
 * all of the adjacent vertices).
 */

/**
 * @brief Get the row index into an array of vertices, supporting wrapping back
 *        around to the beginning and other useful features like that.
 * @param i The current index.
 * @param rs The number of radial slices.
 * @param k The number of the vertex to get, starting with the vertex directly
 *          to the right of the current as being k == 0, then (rotating
 *          counterclockwise), the next vertex would have k == 1, and so on.
 */
int
get_facex (int i, int rs, int k);

/**
 * @brief Get the column index into an array of vertices, supporting wrapping
 *        back around to the beginning and other useful features like that.
 * @param j The current index.
 * @param vs The number of vertical slices.
 * @param k The number of the vertex to get, starting with the vertex directly
 *          to the right of the current as being k == 0, then (rotating
 *          counterclockwise), the next vertex would have k == 1, and so on.
 */
int
get_facey (int j, int vs, int k);

/**
 * @brief Get the row index into an array of vertices, supporting wrapping back
 *        around to the beginning and other useful features like that (for
 *        triangles).
 * @param i The current index.
 * @param rs The number of radial slices.
 * @param k The number of the vertex to get, starting with the vertex directly
 *          to the right of the current as being k == 0, then (rotating
 *          counterclockwise), the next vertex would have k == 1, and so on.
 */
int
get_facex8 (int i, int rs, int k);

/**
 * @brief Get the column index into an array of vertices, supporting wrapping
 *        back around to the beginning and other useful features like that (for
 *        triangles).
 * @param j The current index.
 * @param vs The number of vertical slices.
 * @param k The number of the vertex to get, starting with the vertex directly
 *          to the right of the current as being k == 0, then (rotating
 *          counterclockwise), the next vertex would have k == 1, and so on.
 */
int
get_facey8 (int j, int vs, int k);

#endif /* __INDEXUTIL_H_ */
