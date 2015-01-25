#ifndef __TRANSFORM_H_
#define __TRANSFORM_H_ /* __TRANSFORM_H_ */

/** Express the three different types of axes. */
enum
  {
    X_AXIS,
    Y_AXIS,
    Z_AXIS
  };

/** Express type of a transformation. */
enum
  {
    ROTATION,
    SCALE,
    TRANSLATION
  };

/**
 * @brief This is a transformation.
 */
struct transformation
{
  int type;
  float** matrix;
};

/* Functions!!! */

/**
 * @brief Returns a rotation matrix based off of given info.
 * @param theta The rotation angle.
 * @param ax An integer representing the axis.
 *        0 ==> X_AXIS
 *        1 ==> Y_AXIS
 *        2 ==> Z_AXIS
 * @return The rotation matrix.
 */
float**
rotation_axis (float theta, int ax);

/**
 * @brief Returns a rotation matrix based off of given info.
 * @param theta The rotation angle.
 * @param u The vector to rotate about.
 * @return The rotation matrix.
 */
float**
rotation_arbitrary (float theta, float u[3]);

/**
 * @brief Returns a scale matrix based off of given info.
 * @param ss The scale factor in each of the three directions.
 *        ss[0] ==> scale in x
 *        ss[1] ==> scale in y
 *        ss[2] ==> scale in z
 * @return The scale matrix.
 */
float**
scale (float ss[4]);

/**
 * @brief Returns a translation matrix based off of given info.
 * @param ds The displacement vector.
 *        ds[0] ==> dx
 *        ds[1] ==> dy
 *        ds[2] ==> dz
 * @return The translation matrix.
 */
float**
translation (float ds[4]);

#endif /* __TRANSFORM_H_ */
