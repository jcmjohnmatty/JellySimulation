#ifndef __VERTEX_H_
#define __VERTEX_H_ /* __VERTEX_H_ */

/**
 * @brief Stores vertex data.
 */
struct vertex
{
  /** The mass of this vertex. */
  float m;
  /** The acceleration vector. */
  float* a;
  /** The velocity vector. */
  float* v;
  /** The position vector. */
  float* x;
  /** The color of this vertex. */
  float* rgb;
};

/**
 * @brief Init a vertex.
 * @param v The vertex to init.
 */
void
vertex_init (struct vertex* v);

/**
 * @brief Clone a vertex.
 * @param v The vertex to clone.
 * @return A deep copy of v.
 */
struct vertex*
vertex_clone (struct vertex* v);

/**
 * @brief Delete a vertex.
 * @param v The vertex to delete.
 */
void
vertex_delete (struct vertex* v);

#endif /* __VERTEX_H_ */
