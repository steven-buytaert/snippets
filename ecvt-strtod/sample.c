#include <stdio.h>

#include <ecvt-strtod.h>

int main(int argc, char * argv[]) {

  ECvt_t ECvt = {
    .f64   = 1.380651569e-23,     // Boltzmann constant (https://phys.org/news/2013-07-accurate-boltzmann-constant.html)
    .width = 64,
    .ndig  = 17,
  }; 

  printf("%.*e printf\n", ECvt.ndig - 1, ECvt.f64);
  
  sbtecvt(& ECvt);
  
  printf("%c.%se%d sbtecvt\n", ECvt.digits[0], & ECvt.digits[1], ECvt.decpt - 1);

  ECvt.f32 = 3.14159265f;
  ECvt.width = 32;

  sbtecvt(& ECvt);
  
  printf("%c.%s sbtecvt\n", ECvt.digits[0], & ECvt.digits[1]);

  return 0;

}
