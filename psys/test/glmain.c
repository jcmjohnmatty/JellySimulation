#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "glmain.h"
#include "../psys.h"

static int n = 0;
static int window_w = 500;
static int window_h = 500;

/** A particle system. */
static struct particlesys* psys;

int
main (int argc, char* argv[])
{
  glutInit (&argc, argv);

  my_setup ();
  glut_setup ();
  gl_setup ();

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
  glutCreateWindow ("Particle System Test");

  /* set up callback functions */
  glutDisplayFunc (my_display);
  glutReshapeFunc (my_reshape);
  glutKeyboardFunc (my_keyboard);

  /* Set the next timer event. */
  glutTimerFunc (DT, my_timeout, 0);
}

void
gl_setup (void)
{
  /* enable depth handling (z-buffer) */
  glEnable (GL_DEPTH_TEST);

  /* enable auto normalize */
  glEnable (GL_NORMALIZE);

  /* define the background color as black. */
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
my_setup (void)
{
  /* Create a new particle system. */
  psys = psys_new (100, pnew);
}

void
my_cleanup (void)
{
  /* Free the memory allocated for our particle system. */
  psys_free (psys, pfree);
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
my_display (void)
{
  /* Clear all pixels, reset depth */
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity ();

  gluLookAt (0.0, 5.0, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  /* Draw stuff. */
  psys_draw (psys);

  /* This buffer is ready */
  glutSwapBuffers ();

  /* Display the changes. */
  glutPostRedisplay ();
}

/**
 * @brief Update a particle on each timeout.
 * @param p The particle to update.
 */
void
pupdate (struct particle* p)
{
  /* For now, just to make sure everything works, translate down. */
  p->x[0] = n - 50;
  p->x[1] -= 0.0625;
  p->x[2] = n - 50;

  n++;

  if (n >= 99) n = 0;
}

void
my_timeout (int id)
{
  /* Update the partle system. */
  psys_timeout (psys, pupdate);

  /* Display the change. */
  my_display ();

  /* Set the next timer event. */
  glutTimerFunc (DT, my_timeout, 0);
}
