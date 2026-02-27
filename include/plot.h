#ifndef PLOT_H
#define PLOT_H

#include "aircraft.h"

int write_aircraft_svg(const char *path, const Aircraft *a);
int write_wing_detail_svg(const char *path, const Aircraft *a);
int write_tail_detail_svg(const char *path, const Aircraft *a);

#endif