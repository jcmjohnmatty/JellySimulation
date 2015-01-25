#include "indexutil.h"

int
get_facex (int i, int rs, int k)
{
  int res;

  switch (k)
    {
    case 0:
      res = i;
      break;
    case 1:
      res = (i + 1) % rs;
      break;
    case 2:
      res = i;
      break;
    case 3:
      res = (i - 1) % rs;
      break;
    case 4:
      res = i;
      break;
    default:
      res = -1;
      break;
    }

  if (k < 0 || k > 4) return -1;
  else
    if (res < 0)
      res += rs;

  return res;
}

int
get_facey (int j, int vs, int k)
{
  int res;

  switch (k)
    {
    case 0:
      if (j != vs - 1) res = (j + 1) % vs;
      else res = j;
      break;
    case 1:
      res = j;
      break;
    case 2:
      if (j != 0) res = (j - 1) % vs;
      else res = 0;
      break;
    case 3:
      res = j;
      break;
    case 4:
      if (j != vs - 1) res = (j + 1) % vs;
      else res = j;
      break;
    default:
      res = -1;
      break;
    }

  if (k < 0 || k > 4) return -1;
  else
    if (res < 0) res += vs;
  return res;
}

/* Added the below two functions in order to more accurately calculate the
   spring force on the jellyfish (in jelly_animate.c). */

int
get_facex8 (int i, int rs, int k)
{
  int res;

  switch (k)
    {
    case 0:
      res = i;
      break;
    case 1:
      res = (i + 1) % rs;
      break;
    case 2:
      res = (i + 1) % rs;
      break;
    case 3:
      res = (i + 1) % rs;
      break;
    case 4:
      res = i;
      break;
    case 5:
      res = i - 1;
      break;
    case 6:
      res = i - 1;
      break;
    case 7:
      res = i - 1;
      break;
    case 8:
      res = i;
      break;
    default:
      res = -1;
      break;
    }

  if (k < 0 || k > 8) return -1;
  else
    if (res < 0)
      res += rs;

  return res;
}

int
get_facey8 (int j, int vs, int k)
{
  int res;

  switch (k)
    {
    case 0:
      res = (j + 1) % vs;
      break;
    case 1:
      res = (j + 1) % vs;
      break;
    case 2:
      res = j;
      break;
    case 3:
      res = j - 1;
      break;
    case 4:
      res = j - 1;
      break;
    case 5:
      res = j - 1;
      break;
    case 6:
      res = j;
      break;
    case 7:
      res = (j + 1) % vs;
      break;
    case 8:
      res = j;
      break;
    default:
      res = -1;
      break;
    }

  if (k < 0 || k > 8) return -1;
  else
    if (res < 0)
      res += vs;

  return res;
}
