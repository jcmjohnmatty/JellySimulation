#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../scene.h"
#include "../util/linalg.h"
#include "indexutil.h"
#include "jelly_animate.h"
#include "jelly_body.h"
#include "jelly_tentacle.h"
#include "vertex.h"

static void
tentacle_animate_step (struct jelly* j, struct scene_props* sp, float dt);

/**
 * @brief Compute the net spring force on a vertex in a jellyfish.
 * @param j The jellyfish to compute the spring force of a vertex in.
 * @param vxi The row that the vertex is in.
 * @param vyi The column that the vertex is in.
 * @return The net spring force on the specified vertex.
 */
static float*
compute_net_springf (const struct jelly * j, const int vxi, const int vyi)
{
  int k;
  float* disp;
  /* Scaled displacement. */
  float* sdisp;
  float* net_springf = NULL;
  /* Temporaries. */
  float* tmp0 = NULL;
  float* tmp1 = NULL;
  struct vertex* curr;
  struct vertex* vadj;

  /* Get current vertex. */
  curr = j->vertices[vxi][vyi];

  /* We initally have no force. */
  net_springf = calloc (4, sizeof (float));
  net_springf[3] = 1;

  /* There are eight vertices adjacent to the current vertex since trimesh. */
  for (k = 0; k < 8; k++)
    {
      /* Get the adjacent vertex. */
      vadj = j->vertices[get_facex8 (vxi, j->rs, k)][get_facey8 (vyi, j->vs,
                                                                 k)];

      /* Direction vector from curr to vadj. */
      disp = vector_sub (curr->x, vadj->x, 4);
      sdisp = vector_scale (disp, j->l_r, 4);

      /* Calculate force and add to the net force. */
      tmp0 = vector_sub (disp, sdisp, 4);
      free (disp);
      free (sdisp);
      tmp1 = vector_scale (tmp0, j->lambda / j->l_r, 4);
      free (tmp0);
      tmp0 = vector_add (net_springf, tmp1, 4);
      free (net_springf);
      net_springf = tmp0;
    }

  return net_springf;
}

/**
 * @brief Compute the force from the water pressure on a vertex in the
 *        jellyfish.
 * @param j The jellyfish to compute the force from water pressure on a vertex
 *        in.
 * @param thrust The magnitude of the thrust that the jellyfish is producing.
 * @param vxi The row that the vertex is in.
 * @param vyi The column that the vertex is in.
 * @return The calculated force vector for that vertex.
 */
float*
compute_net_pressf (const struct jelly * j, const float thrust,
                    const int vxi, const int vyi)
{
  int k;
  float area = 0.0f;
  float* normal;
  float* tmpv1;
  float* tmpv2;
  float* tmp = NULL;
  float* tmppressf = NULL;
  float* pressf = NULL;
  struct vertex* v0;
  struct vertex* v1;
  struct vertex* v2;

  pressf = calloc (4, sizeof (float));

  /* Get the start vertex and that vertex's normal vector. */
  v0 = j->vertices[vxi][vyi];
  normal = j->norms[vxi][vyi];

  for (k = 0; k < 8; k++)
    {
      /* First get the next two verticies. */
      v1 = j->vertices[get_facex8 (vxi, j->rs, k)][get_facey8 (vyi, j->vs, k)];
      v2 = j->vertices[get_facex8 (vxi, j->rs, k + 1)][get_facey8 (vyi, j->vs,
                                                               k + 1)];

      tmpv1 = vector_sub (v1->x, v0->x, 4);
      tmpv2 = vector_sub (v2->x, v0->x, 4);

      /* Get the area. */
      tmp = vector_cross (tmpv1, tmpv2, 3);
      area += 0.5 * vector_magnitude (tmp, 4);

      free (tmp);
      free (tmpv1);
      free (tmpv2);
    }

  /* Finally add this part to the net force. */
  tmppressf = pressf;
  /* This should really be the sum of all the areas on the mesh instead of
     j->avg_area, however j->avg_area is much faster and still very close to
     correct. */
  tmp = vector_scale (normal, 1/3. * thrust * area / j->avg_area, 4);
  pressf = vector_add (tmppressf, tmp, 4);
  free (tmp);
  free (tmppressf);

  return pressf;
}

void
jelly_animate_step (struct jelly* j, struct scene_props* sp, float dt)
{
  int i;
  int k;
  int dim;
  float thrust;
  float tmpa;
  float tmpv;
  float tmpx;
  float r_t;
  float scale;
  float sin_phi;
  float cos_theta;
  float sin_theta;
  float* spring_force;
  float* wpressf;
  struct vertex* tmp;
  float offset;

  /* Increment the total time. */
  sp->t += dt;

  /* We have to scale the acceleration. */
  scale = 0.00000625;

  /* Calculate the thrust force. */
  thrust = sp->water_rho / j->avg_area * pow (((j->omega + j->eps * j->omega
                                                * cos (j->omega * sp->t))
                                               * cos (j->omega * sp->t +
                                                      j->eps
                                                      * sin (j->omega * sp->t)))
                                              / dt, 2);
  thrust *= 100 * sin (j->omega * sp->t);

  /* Calculate the aperture radius. */
  r_t = 75 * scale * (j->alpha * pow (sin (j->omega * sp->t), 3)
                      + j->beta * sin (j->omega * sp->t));

  /* For each vertex, update the acceleration, velocity, and position for each
     dimension (x, y, z). */
  for (i = 0; i < j->rs; i++)
    {
      /* Calculate the angle. */
      sin_phi = sin (M_PI * i / (2 * j->rs));

      for (k = 0; k < j->vs; k++)
        {
          /* Get the current vertex. */
          tmp = j->vertices[i][k];

          /* Calculate the angle. */
          sin_theta = sin (2 * M_PI * k / j->vs);
          cos_theta = cos (2 * M_PI * k / j->vs);

          /* Calculate the net spring force on this vertex. */
          spring_force = compute_net_springf (j, i, k);
          assert (spring_force != NULL);

          /* Calculate the force caused by the water pressure on this vertex. */
          wpressf = compute_net_pressf (j, scale * thrust, i, k);
          assert (wpressf != NULL);

          /* For each dimension. */
          for (dim = 0; dim < 3; dim++)
            {
              /* Update acceleration. */
              tmpa = tmp->a[dim];

              tmpv = tmp->v[dim];

              /* Spring force from adjacent springs. */
              tmp->a[dim] = scale * ((dim == 1 ? (thrust
                                                  - 625 * pow (M_PI, 12)
                                                  * pow (r_t, 24)
                                                  * pow (tmpv, 2)): 0)
                                     + 9000 * spring_force[dim]
                                     + wpressf[dim]) / tmp->m;

              /* Update velocity. */
              tmp->v[dim] = j->b * (tmpv + tmpa * dt);

              /* Update position. */
              tmpx = tmp->x[dim];
              offset = tmpv * dt + tmpa * pow (dt, 2) / 2;
              tmp->x[dim] = tmpx + offset;
              
              /* Let's update the jellyfish tentacles by the same amount as the widest 
              row of points in the aperture. I.e., the last row. */
              /*if(i==(j->rs-1) && k==0 && dim==1)
                {
                  for(t=0;t<j->tentacles;t++)
                    {
                      struct tentacle *tentacle = j->tentacle_objs[t];
                      for(p=0;p<tentacle->points;p++)
                        {
                          tentacle->vertices[p][0]->x[dim] += offset;
                        }
                    }
                }*/

              /* Increment by the aperture radius change. */
              switch (dim)
                {
                case 0:
                  /* Change the x. */
                  tmp->x[dim] -= r_t * pow (i, 2) / pow (j->rs, 2) * sin_phi * cos_theta;
                  break;

                case 1:
                  break;

                case 2:
                  /* Change the z. */
                  tmp->x[dim] -= r_t * pow (i, 2) / pow (j->rs, 2) * sin_phi * sin_theta;
                  break;

                default:
                  break;
                }
            }

          /* Done with this spring force and force from water pressure. */
          free (spring_force);
          free (wpressf);
        }
    }
    
    tentacle_animate_step(j, sp, dt);

  /* This concludes the global movement of the jellyfish body. */
}


static void 
relax(struct tentacle *tentacle, int maxiters);

static void 
vertlet(struct vertex *v, float dt, float accel, float scale);

static
void constrain_length(struct vertex *from, struct vertex *to, float len);


static void 
constrain_angle(struct vertex *from, struct vertex *to, float angle);

static void
tentacle_animate_step (struct jelly* j, struct scene_props* sp, float dt)
{
  float deltaY = 0.0f;
  int i;
  int p;
  
  /* This moves the outer edge tentacles */
  for (i = 0; i < j->rs; i++)
    {
      struct tentacle *tentacle = j->tentacle_objs[i];
    
      /* Save a copy of the Y displacement to move the tentacles with the body */
      deltaY = j->vertices[j->vs-1][i]->x[1] - tentacle->vertices[0][0]->x[1];
    
      memcpy(tentacle->vertices[0][0]->a, tentacle->vertices[0][0]->x, sizeof(float)*4);
       /* Make sure the first point of the tentacle matches the end of the cap */
      memcpy(tentacle->vertices[0][0]->x, j->vertices[j->vs-1][i]->x, sizeof(float)*4);

      for(p=1; p<tentacle->points; p++)
        {
          float old_x = tentacle->vertices[p][0]->x[0];
          float old_y = tentacle->vertices[p][0]->x[1];
          float old_z = tentacle->vertices[p][0]->x[2];

          tentacle->vertices[p][0]->x[1] += deltaY;
          vertlet(tentacle->vertices[p][0], dt, 0.0f, 0.0f);

          /* Abuse the vector to hold old position data rather than acceleration */
          tentacle->vertices[p][0]->a[0] = old_x;
          tentacle->vertices[p][0]->a[1] = old_y;
          tentacle->vertices[p][0]->a[2] = old_z;
      }

      relax(tentacle, 100);
    }

  struct tentacle *tentacle = j->tentacle_objs[j->rs];

  deltaY = j->vertices[0][0]->x[1]-tentacle->vertices[0][0]->x[1];
  tentacle->vertices[0][0]->x[1] = j->vertices[0][0]->x[1] - 0.01f;

  /* Move the inner tentacle */
  for(p=1; p<tentacle->points; p++)
    {
      tentacle->vertices[p][0]->x[1] = tentacle->vertices[p-1][0]->x[1]-tentacle->length;
    }
}


static void 
vertlet(struct vertex *v, float dt, float accel, float friction)
{
   v->x[0] = (2.0f - friction) * v->x[0] - (1.0f - friction) * v->a[0] + accel * dt * dt; 
   //We omit the Y term due to having it be dragged purely upward by the cap
   v->x[2] = (2.0f - friction) * v->x[2] - (1.0f - friction) * v->a[2] + accel * dt * dt ; 
}

static void 
relax(struct tentacle *tentacle, int maxiters)
{
  int i;
  int p;

  /* Apply relaxation to propagate the constraints forwards and backwards along the tentacle */
  for (i = 0; i < maxiters; i++)
    {
      for(p=1; p<tentacle->points; p++)
        {
          constrain_length(tentacle->vertices[p-1][0], tentacle->vertices[p][0], tentacle->length);
          //constrain_angle(tentacle->vertices[p-1][0], tentacle->vertices[p][0], M_PI/8.0f);
        }
/*
      for(p=tentacle->points-1; p>0; p--)
        {
          constrain_length(tentacle->vertices[p][0], tentacle->vertices[p-1][0],  tentacle->length);
          //constrain_angle(tentacle->vertices[p][0], tentacle->vertices[p-1][0], M_PI/8.0f);
        }
*/
    }
}

static void 
constrain_length(struct vertex *from, struct vertex *to, float len)
{
  float dx = to->x[0] - from->x[0];
  float dy = to->x[1] - from->x[1];
  float dz = to->x[2] - from->x[2];
  float dst = sqrtf(dx*dx + dy*dy + dz*dz);

  if (dst > len && dst != 0) 
    {
      to->x[0] -= (dst - len) * (dx / dst) * 0.5;
      //Won't adjust the Y term for the constraints.
      to->x[2] -= (dst - len) * (dz / dst) * 0.5;
    }
  else if (dst < len && dst != 0) 
    {
      to->x[0] += (dst - len) * (dx / dst) * 0.5;
      //Won't adjust the Y term for the constraints.
      to->x[2] += (dst - len) * (dz / dst) * 0.5;
    }
}

static void 
constrain_angle(struct vertex *from, struct vertex *to, float angle)
{
  float *vec = vector_sub(to->x, from->x, 3);
  float eY[] = {0, -1, 0};
  
  float dot = vector_dot(vec, eY, 3);
  float len = vector_magnitude(vec, 3);
  
  float a = acosf(dot/len);

  if (fabs(a) > angle) 
    {
      to->x[0] += -to->x[0] * 0.1f ;
      to->x[2] += -to->x[2] * 0.1f ;
    }
    
  free(vec);
}
