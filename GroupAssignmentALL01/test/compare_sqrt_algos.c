#include <stdio.h>
#include "math.h"
#include <time.h>
#include <stdlib.h>
#include <stdint.h> // uint32_t

float Q_rsqrt( float number )
{
  const float x2 = number * 0.5F;
  const float threehalfs = 1.5F;

  union {
    float f;
    uint32_t i;
  } conv  = { .f = number };
  conv.i  = 0x5f3759df - ( conv.i >> 1 );
  conv.f  *= threehalfs - ( x2 * conv.f * conv.f );
  return conv.f;
}

int main(int argc, char** argv){
  if (argc != 3){
    printf("Usage : #points, percentange_precision(no percentage symbol)\n");
    exit(0);
  }
  int n_points = atoi(argv[1]);
  float precision = atof(argv[2]);
  float quake = 0;
  float m_sqrt = 0;
  srand(time(NULL));
  int r;
  float upper_bound;
  float lower_bound;

  for (int i = 0; i < n_points; i++){
    r = rand();

#ifdef DEBUG
printf("Calculating %d\n",r);
#endif

    quake = 1/Q_rsqrt(r);
    m_sqrt = sqrt(r);

#ifdef DEBUG
  printf("quake = %f, sqrt = %f\n",quake,m_sqrt);
#endif

    lower_bound = m_sqrt - (m_sqrt * precision / 100);
    upper_bound = m_sqrt + (m_sqrt * precision / 100);
    
#ifdef DEBUG
  printf("lower = %f, upper = %f\n",lower_bound, upper_bound);
#endif
    int warn_flag = 0;
    if(quake > upper_bound || quake < lower_bound){
      printf("Precision Warning : quake = %f, sqrt = %f\n",quake, m_sqrt);
      warn_flag++;

    }

    printf("Done with precision interval of %f percent and %d precision warnings\n", precision, warn_flag);
    return 0;
  }
}
