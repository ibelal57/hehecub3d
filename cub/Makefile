NAME = cub3d
.DEFAULT_GOAL := all

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I$(MLX_DIR)
LDFLAGS = -lm
SWIFTC = swiftc

MLX_DIR = minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx.dylib

USE_MLX ?= 1

# If a prebuilt MinilibX library exists in $(MLX_DIR), skip rebuilding it.
NEED_MLX_BUILD := $(shell if [ -f $(MLX_DIR)/libmlx.dylib ] || [ -f $(MLX_DIR)/libmlx.a ] || [ -f $(MLX_DIR)/libmlx_Linux.a ]; then echo 0; else echo 1; fi)

SRC = src/main.c \
	src/app_init.c \
	src/app_cleanup.c \
	src/error.c \
	src/file_read.c \
	src/string_utils.c \
	src/parser.c \
	src/parser_color.c \
	src/parser_map.c \
	src/render.c \
	src/raycast.c \
	src/input.c

ifeq ($(USE_MLX),0)
SRC += src/mlx_stub.c
endif

OBJ = $(SRC:.c=.o)

ifeq ($(USE_MLX),1)
CFLAGS += -DCUB3D_USE_MLX=1
LDFLAGS += -L$(MLX_DIR) -lmlx -lz -Wl,-rpath,@executable_path/$(MLX_DIR)

ifeq ($(NEED_MLX_BUILD),0)
# If a prebuilt MLX library exists (e.g. minilibx-linux/libmlx.a), link X11 libs.
LDFLAGS += -lX11 -lXext
endif

mlx:
ifeq ($(NEED_MLX_BUILD),1)
mlx:
	cd $(MLX_DIR) && \
	rm -f mlx_image.o mlx_image.swiftmodule mlx_window.o mlx_window.swiftmodule \
	mlx_init.o mlx_init.swiftmodule interface.o mlx_xpm.o mlx_png.o \
	mlx_string_put.o libmlx.dylib && \
	$(SWIFTC) -suppress-warnings -I. -parse-as-library -c mlx_image.swift -o mlx_image.o \
	-emit-module -emit-module-path mlx_image.swiftmodule -module-name mlx_image && \
	$(SWIFTC) -suppress-warnings -I. -parse-as-library -c mlx_window.swift -o mlx_window.o \
	-emit-module -emit-module-path mlx_window.swiftmodule -module-name mlx_window && \
	$(SWIFTC) -suppress-warnings -I. -parse-as-library -c mlx_init.swift -o mlx_init.o \
	-emit-module -emit-module-path mlx_init.swiftmodule -module-name mlx_init && \
	$(SWIFTC) -suppress-warnings -I. -parse-as-library -c interface.swift -o interface.o \
	-module-name interface && \
	cc -Wno-deprecated-non-prototype -I. -c mlx_xpm.c -o mlx_xpm.o && \
	cc -Wno-deprecated-non-prototype -I. -c mlx_png.c -o mlx_png.o && \
	cc -Wno-deprecated-non-prototype -I. -c mlx_string_put.c -o mlx_string_put.o && \
	$(SWIFTC) -emit-library -o libmlx.dylib interface.o mlx_init.o mlx_window.o \
	mlx_image.o mlx_xpm.o mlx_png.o mlx_string_put.o -lz && \
	install_name_tool -id @rpath/libmlx.dylib libmlx.dylib
endif
all: mlx $(NAME)
else
CFLAGS += -DCUB3D_USE_MLX=0
endif

all: $(NAME)

$(NAME): $(OBJ)

ifeq ($(USE_MLX),1)
ifeq ($(NEED_MLX_BUILD),1)
$(NAME): mlx
endif
endif
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

clean:
	rm -f $(OBJ)
	rm -f $(MLX_DIR)/mlx_image.o $(MLX_DIR)/mlx_image.swiftmodule $(MLX_DIR)/mlx_window.o \
	$(MLX_DIR)/mlx_window.swiftmodule $(MLX_DIR)/mlx_init.o $(MLX_DIR)/mlx_init.swiftmodule \
	$(MLX_DIR)/interface.o $(MLX_DIR)/mlx_xpm.o $(MLX_DIR)/mlx_png.o \
	$(MLX_DIR)/mlx_string_put.o $(MLX_DIR)/libmlx.dylib

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re