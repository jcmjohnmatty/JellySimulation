/*
 * This file contains some basic structures and functions for simulating a
 * particle system.  Specifically, it contains some functions for creating and
 * freeing particle systems, as well as some basic functions to create, free,
 * and update particles.  These functions are not meant to cover every possible
 * case, but rather to form a basis for users to create particle systems on.
 */

#ifndef __PSYS_H_
#define __PSYS_H_ /* __PSYS_H_ */

/**
 * @brief A particle.
 */
struct particle
{
  /**
   * The number of dimensions the particle is moving in (should probably be 3,
   * however left as variable for modularity.
   */
  int ndims;
  /** The particle's mass. */
  float m;
  /** The force accumulator. */
  float* f;
  /** The velocity. */
  float* v;
  /** The position of the particle. */
  float* x;

  /* PROPERTIES TO MAKE PRETTY PARTICLES... */
};

/**
 * @brief A particle system.
 */
struct particlesys
{
  /** An array of pointers to each particle. */
  struct particle** particles;
  /** The number of particles in the above array. */
  int n;
  /** The clock for this particle system. */
  float t;
};

/**
 * @brief A basic function to create a new particle moving in R3.
 * @return a pointer to the newly created particle (centered on (0,0,0), not
 *         moving).
 */
struct particle*
pnew (void);

/**
 * @brief Create a new particle system.
 * @param n The number of particles in the new particle system.
 * @param pnew A function describing how to create a new particle.
 * @return A pointer to the newly created particle system.
 */
struct particlesys*
psys_new (int n, struct particle* (*pnew) (void));

/**
 * @brief Free the memory allocated for a particle (allocated using the standard
 *        pfree).
 * @param p The particle who's memory to free.
 */
void
pfree (struct particle* p);

/**
 * @brief Free a particle system.
 * @param psys A pointer to the particle system to free.
 * @param pfree A function describing how to free the memory allocated for a
 *        particle (takes a pointer to a particle as an argument).
 */
void
psys_free (struct particlesys* psys, void (*pfree) (struct particle* p));

/**
 * @brief What to do when a timeout occurs for a particle system.
 * @param psys A pointer to the particle system.
 * @param pupdate A function describing how a particle should be updated (takes
 *        a pointer to a particle as an argument).
 */
void
psys_timeout (struct particlesys* psys, void (*pupdate) (struct particle* p));

/**
 * @brief Draw a particle system.
 * @param psys The particle system to draw.
 */
void
psys_draw (struct particlesys* psys);

#endif /* __PSYS_H_ */
