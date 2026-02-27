#include <stdio.h>
#include "io.h"
#include "stability.h"

static double prompt_double(const char *label, const char *units)
{
    double x = 0.0;

    for (;;) {
        printf("%s [%s]: ", label, units);

        if (scanf("%lf", &x) == 1)
            return x;

        while (getchar() != '\n');
        printf("Invalid input. Try again.\n");
    }
}

void read_aircraft_from_cli(Aircraft *a)
{
    printf("Enter aircraft parameters (SI units)\n\n");

    a->S_w    = prompt_double("Wing area S_w", "m^2");
    a->c_bar  = prompt_double("Mean aerodynamic chord c_bar", "m");
    a->x_ac_w = prompt_double("Wing-body AC location", "m");

    a->S_t    = prompt_double("Tail area S_t", "m^2");
    a->l_t    = prompt_double("Tail arm l_t", "m");

    a->a_w    = prompt_double("Wing lift curve slope a_w", "1/rad");
    a->a_t    = prompt_double("Tail lift curve slope a_t", "1/rad");
    a->eta_t  = prompt_double("Tail efficiency eta_t", "-");
    a->deps_da= prompt_double("Downwash gradient dε/dα", "-");

    a->x_cg   = prompt_double("CG location x_cg", "m");

}

void print_results(const Aircraft *a)
{
    double xnp = neutral_point_x(a);
    double sm  = static_margin(a);

    printf("\nResults\n");
    printf("-------\n");
    printf("Neutral Point: %.6f m\n", xnp);
    printf("Static Margin: %.2f %% MAC\n", sm * 100.0);
}