#include <stdio.h>
#include "aircraft.h"
#include "stability.h"

static double prompt_double(const char *label, const char *units)
{
    double x = 0.0;
    for(;;) {
        if (units && units[0] != '\0') {
            printf("%s [%s]: ", label, units);
        } else {
            printf("%s: ", label);
        }

        if (scanf("%lf", &x) == 1) {
            return x;
        }

        /* Clear invalid input*/
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        printf("Invalid number. Try again.\n");
    }
}

int main(void)
{
    Aircraft a;

    printf("Preliminary Aircraft Stability Tool\n");
    printf("-----------------------------------\n");
    printf("Enter values in SI units.\n\n");

    a.S_w    = prompt_double("Wing area S_w", "m^2");
    a.c_bar  = prompt_double("Mean aerodynamic chord c_bar", "m");
    a.x_ac_w = prompt_double("Wing-body AC location x_ac_w (from datum)", "m");

    a.S_t    = prompt_double("Tail area S_t", "m^2");
    a.l_t    = prompt_double("Tail arm l_t (from CG to tail AC, +aft)", "m");

    a.a_w    = prompt_double("Wing lift curve slope a_w", "1/rad");
    a.a_t    = prompt_double("Tail lift curve slope a_t", "1/rad");
    a.eta_t  = prompt_double("Tail efficiency eta_t", "No Units");
    a.deps_da= prompt_double("Downwash gradient dε/dα", "No Units");

    a.x_cg   = prompt_double("CG location x_cg (from datum)", "m");

    const double xnp = neutral_point_x(&a);
    const double sm  = static_margin(&a);

    printf("\nResults\n");
    printf("-------\n");
    printf("Neutral Point x_np: %.6f m\n", xnp);
    printf("Static Margin SM  : %.2f %% MAC\n", sm * 100.0);

    return 0;
}