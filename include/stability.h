#ifndef STABILITY_H
#define STABILITY_H

#include "aircraft.h"

double tail_volume_coeff(const Aircraft *a);
double neutral_point_x(const Aircraft *a);
double static_margin(const Aircraft *a);

#endif