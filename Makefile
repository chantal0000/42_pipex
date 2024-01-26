# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chbuerge <chbuerge@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/06 11:14:10 by chbuerge          #+#    #+#              #
#    Updated: 2024/01/24 14:40:09 by chbuerge         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# VARIABLES
# defines name of the output library file
NAME = pipex
# defines name of the lobrary file of libft project
LIBFT_NAME = libft/libft.a

# which c compiler to use -> cc
CC = cc
# which flags to use for warnings and treating warnings
#CFLAGS = -g -Wall -Wextra -Werror
#-fsanitize=address
# where to locate libft
LIBFT_DIR = ./libft

# list of the source files srequired to build the project
SRCS =		main.c \
			pipex_utils.c

# defines the object files that will be generated from the cource files
OBJS = ${SRCS:.c=.o}

# BUILD TARGETS
all: ${NAME}

$(NAME): $(LIBFT_NAME) $(OBJS)
	cc $(CFLAGS) $(OBJS) $(LIBFT_NAME) -o $(NAME)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

$(LIBFT_NAME):
	cd libft && make
# Remove object files and executables
clean:
	@rm -f ${OBJS}
	@cd $(LIBFT_DIR) && make clean

# Remove object files, executables, and the libft library
fclean:	clean
	@rm -f ${NAME}
	@cd $(LIBFT_DIR) && make fclean

# Rebuild the project from scratch
re:	fclean all



.PHONY: all clean fclean re
