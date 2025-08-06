#include "../../include/minirt.h"

int	get_num_processors(void)
{
	long	num_cores;

	num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (num_cores < 1)
		return (1);
	return ((int)num_cores);
}