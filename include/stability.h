#ifndef STABILITY_H
#define STABILITY_H

#include "aircraft.h"

/* Return 1 if aircraft fields are valid for the stability model */
int    stability_inputs_valid(const Aircraft *a);

double tail_volume_coeff(const Aircraft *a);
double tail_effectiveness_K(const Aircraft *a);
double neutral_point_x(const Aircraft *a);
double static_margin(const Aircraft *a);

#endif