NAME1			=	ft_nm
NAME2			=	ft_otool

CC				=	gcc
FLAGS			=	-Wall -Wextra -Werror

SRC_DIR_NM		=	src/nm/
SRC_DIR_OTOOL	=	src/otool/

SRC1			=	main.c
SRC2			=	main.c

INCLUDES		= -I include/ -I libft/

SRCS_NM			= $(addprefix $(SRC_DIR_NM), $(SRC1))
SRCS_OTOOL		= $(addprefix $(SRC_DIR_OTOOL), $(SRC2))

all : $(NAME1) $(NAME2)

$(NAME1):
	@make -C libft/
	@$(CC) $(FLAGS) $(SRCS_NM) libft/libft.a $(INCLUDES) -o $(NAME1)
	@echo "\033[1;34m"$(NAME1)" compiled"

$(NAME2):
	@$(CC) $(FLAGS) $(SRCS_OTOOL) libft/libft.a $(INCLUDES) -o $(NAME2)
	@echo "\033[1;34m"$(NAME2)" compiled"

clean:
	@make clean -C libft/
	@echo "\033[0;33mall is clean";

fclean: clean
	@make fclean -C libft/
	@rm -f $(NAME1) $(NAME2)
	@echo "\033[3;0;31mall files have been scratched";

re:				fclean all

.PHONY :		fclean clean re
