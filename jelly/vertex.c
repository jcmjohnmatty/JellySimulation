#include <stdlib.h>
#include "vertex.h"

void
vertex_init (struct vertex* v)
{
  v->a = calloc (3, sizeof (float));
  v->v = calloc (3, sizeof (float));
  v->x = calloc (4, sizeof (float));
  v->x[3] = 1;
  v->rgb = calloc (3, sizeof (float));
}

struct vertex*
vertex_clone (struct vertex* v)
{
  int i;
  struct vertex* v_;

  v_ = malloc (sizeof (struct vertex));
  vertex_init (v_);

  for (i = 0; i < 4; i++)
    v_->a[i] = v->a[i];

  for (i = 0; i < 4; i++)
    v_->v[i] = v->v[i];

  for (i = 0; i < 4; i++)
    v_->x[i] = v->x[i];

  for (i = 0; i < 3; i++)
    v_->rgb[i] = v->rgb[i];

  return v_;
}

void
vertex_delete (struct vertex* v)
{
  free (v->a);
  free (v->v);
  free (v->x);
  free (v->rgb);
}
