#ifndef __JELLY_TENTACLE_H_
#define __JELLY_TENTACLE_H_

struct tentacle 
{
 /** The verticies of the shape. */
  struct vertex*** vertices;

  /** The length of an articulation */
  float length;

  /** The thickness of the line */
  float thickness;
  
  /** The number of articulations */
  int points;

};

struct tentacle*
tentacle_create_tentacle(float length, float thickness, int points);

void
tentacle_delete (struct tentacle* j);

#endif
