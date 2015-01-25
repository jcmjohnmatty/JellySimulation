#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif
#include "indexutil.h"
#include "jelly_body.h"
#include "jelly_tentacle.h"
#include "vertex.h"
#include "../util/linalg.h"
#include "../util/uba.h"

static void
jelly_init_phys_props (struct jelly* j)
{
  float e;
  float f;

  e = 1 - pow (j->h / j->r, 2);
  f = 0.7;

  /* Surface area of ellipsoid. */
  j->avg_area = 2 * M_PI * pow (j->r, 2)
    * (1 - ((1 - pow (e, 2)) / e) * atan (e));

  /* Volume shear factor. */
  j->eps = 2.0/3;

  /* Period (f * 2 * pi). */
  j->omega = f * 2 * M_PI;

  /** Spring tension. */
  j->lambda = 1.375;

  /** Dampening coefficent. */
  j->b = 1;

  /** Resting length. */
  j->l_r = 0.75 * j->r;

  /* Aperture radius. */
  j->r_r = 1.125 * j->r;

  /* Constants. */
  j->alpha = 0.3;
  j->beta = 1 - j->alpha;
  j->beta *= 125;
}

struct jelly*
jelly_create_jelly (float r, float h, int rs, int vs)
{
  int i;
  int j;
  int p;
  float theta;
  float sin_theta;
  float cos_theta;
  float phi;
  float sin_phi;
  float cos_phi;
  struct vertex** v;
  struct jelly* res;

  res = malloc (sizeof (struct jelly));
  res->r = r;
  res->h = h;
  res->rs = rs;
  res->vs = vs;
  
  /* For now, let's have a tentacle on each endpoint and one in the center */
  res->tentacles=rs+1;
  res->tentacle_objs = malloc(sizeof(struct tentacle *) * res->tentacles);

  res->vertices = malloc (sizeof (struct vertex**) * rs);
  for (i = 0; i < rs; i++)
    res->vertices[i] = malloc (sizeof (struct vertex*) * vs);

  res->ctm = matrix_identity (4, 4);

  v = malloc (sizeof (struct vertex*));

  for (i = 0; i < vs; i++)
    {
      theta = 2 * M_PI * i / vs;
      sin_theta = sin (theta);
      cos_theta = cos (theta);

      for (j = 0; j < rs; j++)
        {
          phi = M_PI * j / (2 * rs);
          sin_phi = sin (phi);
          cos_phi = cos (phi);

          *v = malloc (sizeof (struct vertex));
          vertex_init (*v);
          (*v)->m = 2048 * 1.0 / (rs * vs);
          (*v)->x[0] = r * sin_phi * cos_theta;
          (*v)->x[1] = h * cos_phi;
          (*v)->x[2] = r * sin_phi * sin_theta;
          (*v)->x[3] = 1;
          (*v)->rgb[0] = 0;
          (*v)->rgb[1] = 0;
          (*v)->rgb[2] = 1;

          res->vertices[j][i] = *v;      
        }
    }
    
    
  /* Outer tentacles at each endpoint */
  for (i = 0; i < rs; i++)
    {
      theta = 2 * M_PI * i / rs;
      sin_theta = sin (theta);
      cos_theta = cos (theta);
  
			struct tentacle *tentacle = tentacle_create_tentacle(3.0f, 1.4f, 8);
			res->tentacle_objs[i] = tentacle;
			for(p=0; p<tentacle->points; p++)
			{
				tentacle->vertices[p][0]->x[0] = r * cos_theta;
				tentacle->vertices[p][0]->x[2] = r * sin_theta;
			}
		}
    
  /* Center tentacle is the thickest */
  res->tentacle_objs[rs] = tentacle_create_tentacle(h+5.0f, 8.0f, 2);

  jelly_compute_normals (res);

  jelly_compute_texcoords (res);

  jelly_init_phys_props (res);

  free (v);

  return res;
}

void
jelly_apply_transformation (struct jelly* j, float** trans)
{
  float** tmpctm;

  tmpctm = j->ctm;
  j->ctm = matrix_matrix_multiply (trans, 4, 4, tmpctm, 4, 4);
  matrix_delete (tmpctm, 4, 4);
}

void
jelly_reset (struct jelly* j)
{
  matrix_delete (j->ctm, 4, 4);
}

void
jelly_delete (struct jelly* j)
{
  int i;
  int k;

  for (i = 0; i < j->rs; i++)
    {
      for (k = 0; k < j->vs; k++)
        {
          vertex_delete (j->vertices[i][k]);
          free (j->norms[i][k]);
          free (j->texcoords[i][k]);
        }

      free (j->vertices[i]);
      free (j->norms[i]);
      free (j->texcoords[i]);
    }

  free (j->vertices);
  free (j->norms);
  free (j->texcoords);

  matrix_delete (j->ctm, 4, 4);
}

void
jelly_compute_normals (struct jelly* j)
{
  int i;
  int k;
  int l;
  float* vec1;
  float* vec2;
  float** tmpnorm = NULL;
  float* tmp1;
  float* tmp2;
  struct vertex* v0;
  struct vertex* v1;
  struct vertex* v2;

  tmpnorm = calloc (1,sizeof (float**));

  j->norms = malloc (sizeof (float**) * j->rs);

  for (i = 0; i < j->rs; i++)
    {
      j->norms[i] = malloc (sizeof (float*) * j->vs);

      for (k = 0; k < j->vs; k++)
        {
          if (*tmpnorm == NULL)
            *tmpnorm = calloc (4, sizeof (float));

          /* There are four faces adjacent to every vertex since quad mesh. */
          v0 = j->vertices[i][k];
          for (l = 0; l < 4; l++)
            {
              v1 = j->vertices[get_facex (i, j->rs, l)][get_facey (k, j->vs,
                                                                    l)];
              v2 = j->vertices[get_facex (i, j->rs, l + 1)][get_facey (k,
                                                                        j->vs,
                                                                        l + 1)];

              /* Create the two vectors to take the cross product of. */
              vec1 = vector_sub (v1->x, v0->x, 4);
              vec2 = vector_sub (v2->x, v0->x, 4);

              /* Take the cross product to get the surface normal. */
              tmp1 = vector_cross (vec1, vec2, 3);

              /* Add this surface normal to the sum. */
              tmp2 = vector_add (tmp1, *tmpnorm, 4);
              /* Now that the surface normal has been added to the list, free
                 it. */
              free (tmp1);

              /* So we can free the memmory from the old temp normal... */
              tmp1 = *tmpnorm;
              /* Set the temp normal. */
              *tmpnorm = tmp2;
              free (tmp1);

              /* Free the memmory from the temporaries. */
              free (vec1);
              free (vec2);
            }

          /* Add to normals. */
          j->norms[i][k] = *tmpnorm;

          /* Reassign tmpnorm to NULL for the next round. */
          *tmpnorm = NULL;
        }
    }

  free (tmpnorm);
}

void
jelly_compute_texcoords (struct jelly* j)
{
  int i;
  int k;

  j->texcoords = malloc (sizeof (float**) * j->rs);

  for (i = 0; i < j->rs; i++)
    {
      j->texcoords[i] = malloc (sizeof (float*) * j->vs);
      for (k = 0; k < j->vs; k++)
        {
          j->texcoords[i][k] = malloc (sizeof (float) * 2);
          j->texcoords[i][k][0] = (k + 1.0 + 35) / j->rs;
          j->texcoords[i][k][1] = 1 - (float) (i - 10) / j->vs;
        }
    }
}

void
draw_quad (struct vertex* v1, struct vertex* v2, struct vertex* v3,
           struct vertex* v4, float* n1, float* n2, float* n3, float* n4,
           float* t1, float* t2, float* t3, float* t4, int render_mode)
{
  glBegin (render_mode);
  {
    glColor3fv (v1->rgb);
    glNormal3fv (n1);
    glTexCoord2fv (t1);
    glVertex4fv (v1->x);

    glColor3fv (v2->rgb);
    glNormal3fv (n2);
    glTexCoord2fv (t2);
    glVertex4fv (v2->x);

    glColor3fv (v3->rgb);
    glNormal3fv (n3);
    glTexCoord2fv (t3);
    glVertex4fv (v3->x);

    glColor3fv (v4->rgb);
    glNormal3fv (n4);
    glTexCoord2fv (t4);
    glVertex4fv (v4->x);
  }
  glEnd ();
}

void
draw_tentacles(struct jelly* j)
{
  int i;
  int p;
  struct vertex *temp;
  struct tentacle *tentacle;

  glDisable (GL_TEXTURE_2D);
  for(i=0; i<j->tentacles; i++)
    {
      tentacle = j->tentacle_objs[i];

	  glLineWidth(tentacle->thickness);
	  glBegin(GL_LINE_STRIP); 
	  {
		for(p=0; p<tentacle->points;p++)
		  {
		    temp = vertex_clone (tentacle->vertices[p][0]);
			temp->x = matrix_vector_multiply (j->ctm, 4, 4, temp->x, 4);
			
			glColor3fv (tentacle->vertices[p][0]->rgb);
			glVertex4fv (temp->x);
			free(temp);
		  }
	  }
	  glEnd ();
    }
  glEnable (GL_TEXTURE_2D);
}

void
jelly_draw (struct jelly* j, int render_mode, int texid)
{
  int i;
  int k;
  int l;
  float* tx;
  float* norms[4];
  float* tns[4];
  float* texcoords[4];
  struct vertex* tvs[4];
  struct vertex* vs[4];

  /* Begin using the given texture. */
  glEnable (GL_TEXTURE_2D);
  glBindTexture (GL_TEXTURE_2D, texid);

  for (i = 0; i < j->rs; i++)
    {
      for (k = 0; k < j->vs; k++)
        {
          vs[0] = j->vertices[i][k];
          vs[1] = j->vertices[i][(k + 1) % j->vs];
          vs[2] = j->vertices[(i + 1) % j->rs][(k + 1) % j->vs];
          vs[3] = j->vertices[(i + 1) % j->rs][k];

          norms[0] = j->norms[i][k];
          norms[1] = j->norms[i][(k + 1) % j->vs];
          norms[2] = j->norms[(i + 1) % j->rs][(k + 1) % j->vs];
          norms[3] = j->norms[(i + 1) % j->rs][k];

          texcoords[0] = j->texcoords[i][k];
          texcoords[1] = j->texcoords[i][(k + 1) % j->vs];
          texcoords[2] = j->texcoords[(i + 1) % j->rs][(k + 1) % j->vs];
          texcoords[3] = j->texcoords[(i + 1) % j->rs][k];

          for (l = 0; l < 4; l++)
            {
              tvs[l] = vertex_clone (vs[l]);
              tx = tvs[l]->x;
              tvs[l]->x = matrix_vector_multiply (j->ctm, 4, 4, tx, 4);
              free (tx);
              tx = vector_clone (norms[l], 4);
              tns[l] = matrix_vector_multiply (j->ctm, 4, 4, tx, 4);
              free (tx);
            }

          draw_quad (tvs[0], tvs[1], tvs[2], tvs[3], tns[0], tns[1], tns[2],
                     tns[3], texcoords[0], texcoords[1], texcoords[2],
                     texcoords[3], render_mode);

          for (l = 0; l < 4; l++)
            {
              vertex_delete (tvs[l]);
              free (tns[l]);
            }
        }
    }
    
    draw_tentacles(j);
}
