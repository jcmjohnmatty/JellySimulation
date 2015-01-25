#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif
#include "glmain.h"
#include "jelly/jelly.h"
#include "textures/blooper_head.h"
#include "textures/sewer_bg.h"
#include "textures/sewer_floor.h"
#include "textures/sewer_pipe.h"
#include "util/linalg.h"
#include "util/transform.h"

struct jelly* blooper;
/* Keep blooper_w / blooper_h == 9.0 / 8.0 */
static float blooper_w = 1.875;
static float blooper_h = 1.6875;

/* Blooper's texture data. */
static unsigned int blooper_texid;
static unsigned char blooper_tex[BLOOPERTEX_WIDTH][BLOOPERTEX_HEIGHT]
                                [BLOOPERTEX_BYTES_PER_PIXEL];

/* The texture data for the sea floor/wall (oops, however it looks kind of nice
   so I left it.  Feel free to change that if you think it would be better as a
   solid color or something else). */
static unsigned int sewerbg_texid;
static unsigned char sewerbg_tex[SEWERBG_WIDTH][SEWERBG_HEIGHT][3];
static unsigned int sewerfloor_texid;
static unsigned char sewerfloor_tex[SEWERFLOOR_WIDTH][SEWERFLOOR_HEIGHT][3];
static unsigned int sewerpipe_texid;
static unsigned char sewerpipe_tex[SEWERPIPE_WIDTH][SEWERPIPE_HEIGHT][3];

struct scene_props sp;

static int window_w = 500;
static int window_h = 500;
static int render_mode;

int
main (int argc, char* argv[])
{
  glutInit (&argc, argv);

  glut_setup ();
  gl_setup ();
  my_setup ();
  glutMainLoop ();

  my_cleanup ();
  return 0;
}

void
glut_setup (void)
{
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowSize (window_w, window_h);
  glutInitWindowPosition (0, 0);
  glutCreateWindow ("Jellyfish");

  /* set up callback functions */
  glutDisplayFunc (my_display);
  glutReshapeFunc (my_reshape);
  glutKeyboardFunc (my_keyboard);

  /* Set the next timer event. */
  glutTimerFunc ((int)(DT*1000), my_timeout, 0);
}

void
gl_setup (void)
{
  /* enable depth handling (z-buffer) */
  glEnable (GL_DEPTH_TEST);

  /* enable auto normalize */
  glEnable (GL_NORMALIZE);

  /* define the background color as hot pink */
  glClearColor (0, 0, 0, 1);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (40, 1.0, 1, 200.0);
  glMatrixMode (GL_MODELVIEW);
  /* Init modelview to identity. */
  glLoadIdentity ();

  /* toggle to smooth shading (instead of flat) */
  glShadeModel (GL_SMOOTH);
}

void
lighting_setup (void)
{
  float eye[] = { -5, 5, 5, 1 };
  float look[] = { 0, 0, 0, 1 };
  float globalAmb[] = { .1, .1, .1, .1 };

  /* Lighting elements */
  float amb[] = { 0.2, 0.2, 0.2, 1.0 };
  float dif[] = { 0.8, 0.8, 0.8, 1.0 };
  float spec[] = { 5.0, 5.0, 5.0, 1.0 };

 
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_SMOOTH);

  /* Reflective propoerites for global ambiant light */
  glLightModelfv (GL_LIGHT_MODEL_TWO_SIDE, globalAmb);

  glLightfv(GL_LIGHT0, GL_POSITION, eye);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, look);

  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
  glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);

 /* Enable lighting */
  glEnable (GL_LIGHTING);
  glEnable (GL_NORMALIZE);
  glEnable (GL_COLOR_MATERIAL);
  glEnable(GL_LIGHT0);
}

void
texture_setup (void)
{
  int i;
  int j;

  /* Load the blooper texture (thanks, GIMP)! */
  BLOOPERTEX_RUN_LENGTH_DECODE(blooper_tex, bloopertex_data,
                               sizeof (bloopertex_data),
                               BLOOPERTEX_BYTES_PER_PIXEL);

  /* Load the water caustic for the bottom plane. */
  for (i = 0; i < SEWERBG_WIDTH; i++)
    {
      for (j = 0; j < SEWERBG_HEIGHT; j++)
        {
          HEADER_PIXEL(sewerbg_data, sewerbg_tex[i][j]);
        }
    }
  for (i = 0; i < SEWERFLOOR_WIDTH; i++)
    {
      for (j = 0; j < SEWERFLOOR_HEIGHT; j++)
        {
          HEADER_PIXEL(sewerfloor_data, sewerfloor_tex[i][j]);
        }
    }
  for (i = 0; i < SEWERPIPE_WIDTH; i++)
    {
      for (j = 0; j < SEWERPIPE_HEIGHT; j++)
        {
          HEADER_PIXEL(sewerpipe_data, sewerpipe_tex[i][j]);
        }
    }

  /* Generate and bind the texture using OpenGL. */
  glGenTextures (1, &blooper_texid);
  glGenTextures (1, &sewerbg_texid);
  glGenTextures (1, &sewerfloor_texid);
  glGenTextures (1, &sewerpipe_texid);

  /* Enable texturing. */
  glEnable (GL_TEXTURE_2D);

  /* Specify properties for blooper's texture. */
  glBindTexture (GL_TEXTURE_2D, blooper_texid);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, BLOOPERTEX_WIDTH, BLOOPERTEX_HEIGHT,
                0, GL_RGBA, GL_UNSIGNED_BYTE, blooper_tex);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  /* Now specify properties for the sewer textures. */
  glBindTexture (GL_TEXTURE_2D, sewerbg_texid);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, SEWERBG_WIDTH, SEWERBG_HEIGHT, 0,
                GL_RGB, GL_UNSIGNED_BYTE, sewerbg_tex);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture (GL_TEXTURE_2D, sewerfloor_texid);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, SEWERFLOOR_WIDTH, SEWERFLOOR_HEIGHT,
                0, GL_RGB, GL_UNSIGNED_BYTE, sewerfloor_tex);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture (GL_TEXTURE_2D, sewerpipe_texid);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, SEWERPIPE_WIDTH, SEWERPIPE_HEIGHT,
                0, GL_RGB, GL_UNSIGNED_BYTE, sewerpipe_tex);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  /* How do we map it? */
  glTexEnvi (GL_TEXTURE_ENV,
             GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void
scene_setup (void)
{
  sp.t = 0;
  sp.water_rho = 1;

  /* Setup lighting. */
  lighting_setup ();

  /* Setup textures. */
  texture_setup ();
}

void
jelly_setup (void)
{
  float ds[4];
  float** tra;
  float** rot;

  /* BLOOP! */
  blooper = jelly_create_jelly (blooper_w, blooper_h, 30, 30);

  rot = rotation_axis (-45, Z_AXIS);
  jelly_apply_transformation (blooper, rot);
  matrix_delete (rot, 4, 4);

  ds[0] = -5;
  ds[1] = -5;
  ds[2] = 0;
  ds[3] = 1;
  tra = translation (ds);
  jelly_apply_transformation (blooper, tra);
  matrix_delete (tra, 4, 4);
}

void
my_setup (void)
{
  /* Setup the jellyfish. */
  jelly_setup ();

  /* initialize global shape defaults and mode for drawing */
  render_mode = GL_POLYGON;

  /* Steup the scene. */
  scene_setup ();
}

void
jelly_cleanup (void)
{
  jelly_delete (blooper);
}

void
my_cleanup (void)
{
  jelly_cleanup ();
}

void
my_reshape (int w, int h)
{
  glViewport (0, 0, w, h);
  window_w = w;
  window_h = h;
}

void
my_keyboard (unsigned char key, int x, int y)
{
  switch (key)
    {
    case 'Q':
    case 'q':
      exit (0);

    default:
      break;
    }

  glutPostRedisplay ();
}

void
draw_jelly (void)
{
  /* Draw the jellyfish. */
  jelly_draw (blooper, render_mode, blooper_texid);
}

void
draw_background (void)
{
  float isqrt3 = 0.57735026919;

  glEnable (GL_TEXTURE_2D);
  //glBindTexture (GL_TEXTURE_2D, sewerpipe_texid);
  glBegin (GL_QUADS);
  {
    /* Draw the pipe. */
    glColor3f (0, 0, 0);
    glNormal3f (isqrt3, isqrt3, -1 * isqrt3);
    glTexCoord2f (1, 1);
    glVertex4f (window_w, 0.875 * window_h, -100, 1);
    glColor3f (0, 0, 1);
    glNormal3f (-1 * isqrt3, isqrt3, -1 * isqrt3);
    glTexCoord2f (0, 1);
    glVertex4f (0, 0.875 * window_h, -100, 1);
    glColor3f (0, 0, 0);
    glNormal3f (-1 * isqrt3, -1 * isqrt3, -1 * isqrt3);
    glTexCoord2f (0, 0);
    glVertex4f (0, 0.75 * window_h, -100, 1);
    glColor3f (0, 0, 1);
    glNormal3f (isqrt3, -1 * isqrt3, -1 * isqrt3);
    glTexCoord2f (1, 0);
    glVertex4f (window_w, 0.75 * window_h, -100, 1);
  }
  glEnd ();

  glEnable (GL_TEXTURE_2D);
  glBindTexture (GL_TEXTURE_2D, sewerbg_texid);
  glBegin (GL_QUADS);
  {
    /* Draw the back plane. */
    glColor3f (0, 0, 0);
    glNormal3f (isqrt3, isqrt3, -1 * isqrt3);
    glTexCoord2f (220, 207);
    glVertex4f (1000, 1000, -100, 1);
    glColor3f (0, 0, 1);
    glNormal3f (-1 * isqrt3, isqrt3, -1 * isqrt3);
    glTexCoord2f (0, 207);
    glVertex4f (-1000, 1000, -100, 1);
    glColor3f (0, 0, 0);
    glNormal3f (-1 * isqrt3, -1 * isqrt3, -1 * isqrt3);
    glTexCoord2f (0, 0);
    glVertex4f (-1000, -1000, -100, 1);
    glColor3f (0, 0, 1);
    glNormal3f (isqrt3, -1 * isqrt3, -1 * isqrt3);
    glTexCoord2f (220, 0);
    glVertex4f (1000, -1000, -100, 1);
  }
  glEnd ();
  
  glEnable (GL_TEXTURE_2D);
  glBindTexture (GL_TEXTURE_2D, sewerfloor_texid);
  glBegin (GL_QUADS);
  {
    /* Draw bottom plane. */
    glColor3f (0, 0, 0);
    glNormal3f (isqrt3, -1 * isqrt3, -1 * isqrt3);
    glTexCoord2f (64, 64);
    glVertex4f (1000, -15, 1000, 1);
    glColor3f (0, 0, 1);
    glNormal3f (isqrt3, -1 * isqrt3, isqrt3);
    glTexCoord2f (64, 0);
    glVertex4f (1000, -15, -1000, 1);
    glColor3f (0, 0, 0);
    glNormal3f (-1 * isqrt3, -1 * isqrt3, isqrt3);
    glTexCoord2f (0, 0);
    glVertex4f (-1000, -15, -1000, 1);
    glColor3f (0, 0, 1);
    glNormal3f (-1 * isqrt3, -1 * isqrt3, -1 * isqrt3);
    glTexCoord2f (0, 64);
    glVertex4f (-1000, -15, 1000, 1);
  }
  glEnd ();
}

void
my_display (void)
{
  /* clear all pixels, reset depth */
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity ();

  gluLookAt (0.0, 5.0, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  /* draw the objects */
  draw_background ();
  draw_jelly ();

  /* this buffer is ready */
  glutSwapBuffers ();

  /* Display the change. */
  glutPostRedisplay ();
}

void
my_timeout (int id)
{
  /* Animate the jellyfish. */
  jelly_animate_step (blooper, &sp, DT);

  /* Display the change. */
  my_display ();

  /* Set the next timer event. */
  glutTimerFunc ((int)(DT * 1000), my_timeout, 0);
}
