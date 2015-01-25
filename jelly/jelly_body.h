#ifndef __JELLY_BODY_H_
#define __JELLY_BODY_H_ /* __JELLY_BODY_H_ */

#include "vertex.h"
#include "jelly_tentacle.h"

/**
 * @brief Store jellyfish's body data.  
 */
struct jelly
{
  /** The verticies of the shape. */
  struct vertex*** vertices;
  /** The normals of the shape. */
  float*** norms;
  /** The texture coordinates of the shape. */
  float*** texcoords;
  /** The Cumulative transformation matrix. */
  float** ctm;
  /** The radius of the unbrella. */
  float r;
  /** The height of the ubrella. */
  float h;
  /** The number of radial slices of the shape. */
  int rs;
  /** The number of vertical slices of the shape. */
  int vs;

  /* NOW PROPERTIES FOR SIMULATION */

  /** The average cross sectional area. */
  float avg_area;
  /** The shear of the curve (chosen to be 2/3). */
  float eps;
  /** The period. */
  float omega;
  /** The spring tension. */
  float lambda;
  /** The dampening coefficent. */
  float b;
  /** The resting length. */
  float l_r;
  /** The rest radius. */
  float r_r;
  /* Alpha. */
  float alpha;
  /* Beta. */
  float beta;
  
  /** Tentacles */
  struct tentacle **tentacle_objs;
  /**  The number of tentacles. The first rs are attached to the bottom at 
  the edges and the last one is the center one. Perhaps we should break
  that one off into a separate variable. But for now, they're all in
  the same array. */
  int tentacles;
};

/* FUNCTIONS */

/* CREATING SHAPES */

/**
 * @Brief Create a jellyfish.
 * @param r The radius of the jellyfish's umbrella.
 * @param h The radius of the jellyfish's umbrella.
 * @param rs The radial slices.
 * @param vs The vertical slices.
 * @return The newly created jellyfish.
 */
struct jelly*
jelly_create_jelly (float r, float h, int rs, int vs);

/* APPLY TRANSFORMATIONS */

/**
 * @brief Apply a transformation to the jellyfish.
 * @param j The jellyfish to transform.
 * @param trans The transformation matrix to apply to all of j's vertricies.
 */
void
jelly_apply_transformation (struct jelly* j, float** trans);

/* DELETE SHAPES */

void
jelly_reset (struct jelly* j);

void
jelly_delete (struct jelly* j);

/* CALCULATING NORMALS AND TEXTURE COORDINATES */

/**
 * @brief Compute the normals of the jellyfish and store them in the jellyfish.
 * @param j The jellyfish to compute the normals for.
 */
void
jelly_compute_normals (struct jelly* j);

/**
 * @brief Compute the texture coordinates for the jellyfish and store them in
 *        the jellyfish.
 * @param j The jellyfish to compute the texture coordinates for.
 */
void
jelly_compute_texcoords (struct jelly* j);

/* DRAWING. */

/**
 * @brief Draw a jellyfish.
 * @param j The jellyfish to draw.
 * @param render_mode The mode to render in.
 * @param texid The texture id to use for the body.
 */
void
jelly_draw (struct jelly* j, int render_mode, int texid);

#endif /* __JELLY_BODY_H_ */
