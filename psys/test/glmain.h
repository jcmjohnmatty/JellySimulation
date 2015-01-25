#ifndef __GLMAIN_H_
#define __GLMAIN_H_ /* __GLMAIN_H_ */

/** The time step. */
#define DT 0.02f

void
glut_setup (void);

void
gl_setup (void);

void
my_setup (void);

void
my_cleanup (void);

void
my_display (void);

void
my_reshape (int w, int h);

void
my_keyboard (unsigned char key, int x, int y);

void
my_timeout (int id);

#endif /* __GLMAIN_H_ */
