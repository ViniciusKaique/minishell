# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/27 17:59:13 by vinpache          #+#    #+#              #
#    Updated: 2025/10/27 18:10:31 by vinpache         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror

INC_DIR     = include
SRC_DIR     = src
BUILTINS_DIR= $(SRC_DIR)/builtins
LIBFT_DIR   = libft

SRC_FILES =  main.c 					\
			 command_utils.c 			\
			 redirect_utils.c 			\
			 envp_utils.c 				\
			 token_utils.c 				\
			 free_utils.c 				\
			 path_utils.c 				\
			 execute.c 					\
			 lexer.c 					\
			 parser.c 					\
			 builtins/builtin_utils.c 	\
			 builtins/cd.c 				\
			 builtins/echo.c 			\
			 builtins/env.c 			\
			 builtins/exit.c 			\
			 builtins/export.c 			\
			 builtins/pwd.c 			\
			 builtins/unset.c

SRCS        = $(addprefix $(SRC_DIR)/, $(filter-out main.c, $(SRC_FILES))) main.c

OBJS = $(SRCS:.c=.o)

LIBFT = $(LIBFT_DIR)/libft.a
LIBS  = -lreadline -lncurses

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "🔗 Vinculando tudo para criar $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -I $(INC_DIR) -o $(NAME)
	@echo "✅ Compilado com sucesso: $(NAME)"

$(LIBFT):
	@echo "🧩 Compilando libft..."
	@make -C $(LIBFT_DIR) --no-print-directory

%.o: %.c
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	@$(RM) $(OBJS)
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@echo "🧹 Objetos removidos."

fclean: clean
	@$(RM) $(NAME)
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@echo "🧽 Tudo limpo."

re: fclean all

.PHONY: all clean fclean re
