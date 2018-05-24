NAME		=	ft_nm

CC			=	gcc
FLAGS		=	#-Wall -Wextra -Werror

LIBFT_DIR	=	libft/
LIBFT_LIB	=	$(LIBFT_DIR)libft.a
LIBFT_INC	=	$(LIBFT_DIR)include/

SRC_DIR		=	src/
INC_DIR		=	include/
OBJ_DIR		=	objs/

SRC_BASE	=	main.c

SRCS = $(addprefix $(SRC_DIR), $(SRC_BASE))
OBJS = $(addprefix $(OBJ_DIR), $(SRC_BASE:.c=.o))

all : $(NAME)

$(NAME):		$(LIBFT_LIB) $(OBJ_DIR) $(OBJS)
	@$(CC) $(OBJS) -o $(NAME) \
		-I $(INC_DIR) \
		-I $(LIBFT_INC) $(LIBFT_LIB)
		$(FLAGS)
	@rm -f libft_malloc.so
	@echo "\033[1;34m"$(NAME)" compiled"

$(LIBFT_LIB):
	@make -j -C $(LIBFT_DIR)

$(OBJ_DIR) :
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $(OBJS))

$(OBJ_DIR)%.o :	$(SRC_DIR)%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) -c $< -o $@\
		-I $(INC_DIR)\
		-I $(LIBFT_INC)

clean:
	@make clean -C $(LIBFT_DIR)
	@rm -rf $(OBJ_DIR)
	@echo "\033[0;33mall is clean";

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)
	@echo "\033[3;0;31mall files have been scratched";

re:				fclean all

.PHONY :		fclean clean re

-include $(OBJS:.o=.d)
