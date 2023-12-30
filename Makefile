SRC_DIR := src
LIBFT_DIR := libft
OBJ_DIR := obj

SRC = 	07_exec.c \
		08_exec2.c \
		00_main.c \
		utils.c \
		02_token.c \
		09_exec_utils.c \
		cd_pwd.c \
		export.c \
		unset.c \
		env.c \
		echo.c \
		11_here_doc.c \
		expand.c \
		12_redirection.c \
		10_builtins.c \
		signal.c \
		exit.c \
		01_lexer.c \
		04_syntax.c \
		free.c \
		05_parsing.c \
		06_parsing2.c \
		03_token_list.c \
		error.c \
		set_env.c \
		tmp_put.c

SRC        := $(SRC:%=$(SRC_DIR)/%)
OBJ        := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

INCLUDES_LIB += -I $(LIB_FOLDER)/include/


NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror -ggdb

RM = rm -f


MAKEFLAGS   += --no-print-directory

DIR_DUP = mkdir -p $(@D)

all: $(NAME)

$(NAME): $(OBJ)
	make -C ./libft
	$(CC) $(OBJ) -lreadline -L./libft -l:libft.a -o $(NAME)
	$(info CREATED $(NAME))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(DIR_DUP)
	$(CC) $(CFLAGS) -c -o $@ $<
	$(info CREATED $@)

clean:
		$(RM) -r $(OBJ_DIR)
		@$(MAKE) -C ./libft clean

fclean:	clean
		$(RM) $(NAME)
		@$(MAKE) -C ./libft fclean

re:		fclean all

.PHONY: all clean fclean re
.SILENT:
