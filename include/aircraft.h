#ifndef AIRCRAFT_H
#define AIRCRAFT_H

typedef struct Aircraft {
    /* Geometry */
    double S_w;      /* wing area [m^2] */
    double c_bar;    /* mean aerodynamic chord [m] */
    double x_ac_w;   /* wing-body aerodynamic centre from datum [m] */

    double S_t;      /* tail area [m^2] */
    double l_t;      /* tail arm [m] */

    /* Aerodynamics (per rad) */
    double a_w;      /* wing lift curve slope [1/rad] */
    double a_t;      /* tail lift curve slope [1/rad] */
    double eta_t;    /* tail efficiency [-] */
    double deps_da;  /* downwash gradient dε/dα [-] */

    /* CG */
    double x_cg;     /* centre of gravity from datum [m] */
} Aircraft;

#endif