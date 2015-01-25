#include <stdlib.h>
#include <GL/glut.h>
#include "psys.h"

struct particle*
pnew (void)
{
  struct particle* p;

  p = malloc (sizeof (struct particle));
  /* Default mass of 1. */
  p->m = 1;

  /* 3D. */

  /* Allocate memory for each of the arrays. */
  p->f = calloc (4, sizeof (float));
  p->f[3] = 1;
  p->v = calloc (4, sizeof (float));
  p->v[3] = 1;
  p->x = calloc (4, sizeof (float));
  p->x[3] = 1;

  return p;
}

struct particlesys*
psys_new (int n, struct particle* (*pnew) (void))
{
  int i;
  struct particlesys* psys;

  psys = malloc (sizeof (struct particlesys));

  psys->particles = malloc (sizeof (struct particle*) * n);
  for (i = 0; i < n; i++)
    psys->particles[i] = pnew ();

  psys->n = n;
  psys->t = 0;

  return psys;
}

void
pfree (struct particle* p)
{
  free (p->f);
  free (p->v);
  free (p->x);

  free (p);
}

void
psys_free (struct particlesys* psys, void (*pfree) (struct particle* p))
{
  int i;

  /* Free each of the particles in psys using the given function. */
  for (i = 0; i < psys->n; i++)
    pfree (psys->particles[i]);

  /* Free the memory allocated for the array of particles. */
  free (psys->particles);

  /* Free the actual particle system. */
  free (psys);
}

void
psys_timeout (struct particlesys* psys, void (*pupdate) (struct particle* p))
{
  int i;

  /* Pretty easy, just update each of the particles in the particle system
     using the given function. */
  for (i = 0; i < psys->n; i++)
    pupdate (psys->particles[i]);
}

void
psys_draw (struct particlesys* psys)
{
  int i;

  glDisable (GL_TEXTURE_2D);

  /* For now just have everything be grey. */
  glColor4f (0.875, 0.875, 0.875, -1);
  /* For now just use 1.25 as the point size. */
  glPointSize (2);

  /* Draw every particle. */
  glBegin (GL_POINTS);
  /* For each particle. */
  for (i = 0; i < psys->n; i++)
    {
      glVertex4fv (psys->particles[i]->x);
    }
  glEnd ();

  glEnable (GL_TEXTURE_2D);
}
