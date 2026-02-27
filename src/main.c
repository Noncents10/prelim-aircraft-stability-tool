#include <stdio.h>
#include "aircraft.h"
#include "stability.h"
#include "io.h"
#include "plot.h"

int main(void)
{
    Aircraft a;

    read_aircraft_from_cli(&a);

    
    if (!stability_inputs_valid(&a)) {
        printf("ERROR: invalid aircraft inputs. Check areas, MAC, tail arm, etc.\n");
        return 1;
    }

    print_results(&a);
    write_aircraft_svg("out/aircraft_overview.svg", &a);
    write_wing_detail_svg("out/wing_detail.svg", &a);
    write_tail_detail_svg("out/tail_detail.svg", &a);

    return 0;
}
