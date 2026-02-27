/*
Conceptual longitudinal stability model.

Assumptions:
- Linear aerodynamics
- Subsonic flow
- Wing-body aerodynamic centre provided as input
- No fuselage pitching moment term included
- Tail effectiveness reduced by downwash

Neutral point model:
x_np = x_ac_w + [ K / (a_w + K) ] * V_H * c_bar
where:
V_H = (S_t * l_t) / (S_w * c_bar)
K = eta_t * a_t * (1 - dε/dα)

Static margin:
SM = (x_np - x_cg) / c_bar
*/

#include "stability.h"

double tail_volume_coeff(const Aircraft *a)
{
    return (a->S_t * a->l_t) / (a->S_w * a->c_bar);
}

double neutral_point_x(const Aircraft *a)
{
    double Vh = tail_volume_coeff(a);
    double K  = a->eta_t * a->a_t * (1.0 - a->deps_da);

    return a->x_ac_w + (K / (a->a_w + K)) * Vh * a->c_bar;
}

double static_margin(const Aircraft *a)
{
    double xnp = neutral_point_x(a);
    return (xnp - a->x_cg) / a->c_bar;
}