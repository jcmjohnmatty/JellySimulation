#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif
#include "indexutil.h"
#include "jelly_tentacle.h"
#include "vertex.h"
#include "../util/linalg.h"
#include "../util/uba.h"

#define DEBUG

struct tentacle*
tentacle_create_tentacle(float length, float thickness, int points)
{
  struct tentacle *res;
  int i;
  struct vertex **v;
  
  res = malloc (sizeof (struct tentacle));
  res->length = length/points;
  res->thickness = thickness;
  res->points = points;

  res->vertices = malloc (sizeof (struct vertex**) * points);
  for (i = 0; i < points; i++)
    res->vertices[i] = malloc (sizeof (struct vertex*));

  v = malloc (sizeof (struct vertex*));

  for (i = 0; i < points; i++)
    {
	  *v = malloc (sizeof (struct vertex));
	  vertex_init (*v);
	  //(*v)->m = 2048 * 1.0 / (rs * vs);
	  (*v)->x[0] = 0;
	  (*v)->x[1] = -res->length * i;
	  (*v)->x[2] = 0;
	  (*v)->x[3] = 1;
	  
#ifdef DEBUG
      /* Slightly modified again. */
      (*v)->rgb[0] = (1.5 * (float) 56 / 255);//0xE6 / 0XFF);
      (*v)->rgb[1] = (1.5 * (float) 74 / 255);//0X67 / 0XFF);
      (*v)->rgb[2] = (1.5 * (float) 124 / 255);//0xAF / 0XFF);
      //(*v)->rgb[0] = i%2;
	  //(*v)->rgb[1] = 1-i%2;
	  //(*v)->rgb[2] = 0;
#else	  
	  (*v)->rgb[0] = 1;
	  (*v)->rgb[1] = 0;
	  (*v)->rgb[2] = 0;
#endif
	  res->vertices[i][0] = *v;
    }
    
  free (v);

  return res;
}

void
tentacle_delete (struct tentacle* j)
{
  int i;

  for (i = 0; i < j->points; i++)
    {
    
      vertex_delete (j->vertices[i][0]);
      free (j->vertices[i]);
    }

  free (j->vertices);
}
