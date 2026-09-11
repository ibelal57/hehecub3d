#ifndef MLX_COMPAT_H
# define MLX_COMPAT_H

# if defined(CUB3D_USE_MLX) && CUB3D_USE_MLX == 0
#  include "mlx_stub.h"
# elif __has_include("mlx.h")
#  include "mlx.h"
# else
#  include "mlx_stub.h"
# endif

# if defined(__APPLE__) && !(defined(CUB3D_USE_MLX) && CUB3D_USE_MLX == 0)
static inline int	mlx_destroy_display(void *mlx_ptr)
{
	(void)mlx_ptr;
	return (0);
}
# endif

#endif