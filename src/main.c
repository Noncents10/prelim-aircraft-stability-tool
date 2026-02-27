#include "aircraft.h"
#include "io.h"

int main(void)
{
    Aircraft a;

    read_aircraft_from_cli(&a);
    print_results(&a);

    return 0;
}
