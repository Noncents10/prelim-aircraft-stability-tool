/*
stability.c — Conceptual longitudinal static stability

This module computes:
- Tail volume coefficient V_H
- Neutral point x_np (longitudinal)
- Static margin SM

Coordinate convention:
- All x-locations are measured from a chosen datum (e.g., nose) in metres.

Model assumptions (state these in README too):
- Linear aerodynamics (small angles, constant lift-curve slopes)
- Subsonic flow
- Wing-body aerodynamic centre x_ac_w is provided as an input (often ~0.25 MAC)
- Horizontal tail provides stabilising contribution reduced by:
  - tail efficiency eta_t
  - downwash gradient dε/dα
- No fuselage pitching-moment term included explicitly
- H-tail treated as a horizontal tail for longitudinal stability (vertical fins ignored here)

Key equations:
1) Tail volume coefficient
   V_H = (S_t * l_t) / (S_w * c_bar)

2) Neutral point (conceptual blending model)
   x_np = x_ac_w + [ K / (a_w + K) ] * V_H * c_bar
   where K = eta_t * a_t * (1 - dε/dα)

3) Static margin
   SM = (x_np - x_cg) / c_bar
*/

#include <math.h>
#include "stability.h"

/* Tiny helper: avoid division by ~0 */
static int nearly_zero(double x)
{
    return fabs(x) < 1e-12;
}

int stability_inputs_valid(const Aircraft *a)
{
    if (!a) return 0;

    /* Geometry must be positive */
    if (a->S_w <= 0.0) return 0;
    if (a->S_t <= 0.0) return 0;
    if (a->c_bar <= 0.0) return 0;

    /* Tail arm can be zero/negative in theory, but for a conventional tail it should be > 0 */
    if (a->l_t <= 0.0) return 0;

    /* Lift curve slopes should be positive */
    if (a->a_w <= 0.0) return 0;
    if (a->a_t <= 0.0) return 0;

    /* Efficiency typically 0..1 (allow slightly >1 in case someone uses a correction) */
    if (a->eta_t <= 0.0) return 0;

    /* dε/dα typically 0..0.6; allow 0..1 to avoid rejecting unusual cases */
    if (a->deps_da < 0.0 || a->deps_da >= 1.0) return 0;

    return 1;
}

double tail_volume_coeff(const Aircraft *a)
{
    /* V_H = (S_t * l_t) / (S_w * c_bar) */
    return (a->S_t * a->l_t) / (a->S_w * a->c_bar);
}

double tail_effectiveness_K(const Aircraft *a)
{
    /* K = eta_t * a_t * (1 - dε/dα) */
    return a->eta_t * a->a_t * (1.0 - a->deps_da);
}

double neutral_point_x(const Aircraft *a)
{
    /*
      x_np = x_ac_w + [ K / (a_w + K) ] * V_H * c_bar

      Interpretation:
      - x_ac_w is where the wing-body “acts”
      - tail shifts the neutral point aft depending on:
        - tail volume V_H (area * arm)
        - tail effectiveness K relative to wing slope a_w
    */
    const double Vh = tail_volume_coeff(a);
    const double K  = tail_effectiveness_K(a);

    if (nearly_zero(a->a_w + K)) {
        /* Degenerate case: avoid divide by zero */
        return a->x_ac_w;
    }

    return a->x_ac_w + (K / (a->a_w + K)) * Vh * a->c_bar;
}

double static_margin(const Aircraft *a)
{
    /* SM = (x_np - x_cg) / c_bar */
    const double xnp = neutral_point_x(a);
    return (xnp - a->x_cg) / a->c_bar;
}