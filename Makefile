# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbrito-g <kbrito-g@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/29 19:29:21 by vinpache          #+#    #+#              #
#    Updated: 2025/11/05 16:30:02 by kbrito-g         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
INC_DIR     = include
SRC_DIR     = src
LIBFT_DIR   = libft

SRC_FILES = main.c                          \
            tokenization/lexer.c            \
            tokenization/token_utils.c      \
            tokenization/redirect_utils.c   \
            parsing/parser.c                \
            parsing/command_utils.c         \
            expansion/expansion.c           \
            expansion/expansion_utils.c     \
            signals/signals.c               \
            builtins/builtin_utils.c        \
            builtins/cd.c                   \
            builtins/echo.c                 \
            builtins/exit.c                 \
            builtins/export.c               \
            builtins/pwd.c                  \
            builtins/unset.c                \
            execution/execute.c             \
            execution/execute_checks.c      \
            execution/path_utils.c          \
            execution/free_utils.c          \
            execution/execute_utils.c       \
            execution/pipeline.c            \
            execution/redirect.c            \
            execution/heredoc.c             \
            env/env.c                       \
            env/env_utils.c                 \
            env/envp_utils.c

SRCS = $(addprefix $(SRC_DIR)/, $(filter-out main.c, $(SRC_FILES))) ./main.c
OBJS = $(SRCS:.c=.o)

LIBFT = $(LIBFT_DIR)/libft.a
LIBS  = -lreadline -lncurses

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "🔗 Vinculando tudo para criar $(NAME)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -I $(INC_DIR) -o $(NAME)
	@echo "✅ Compilado com sucesso: $(NAME)"

$(LIBFT):
	@echo "🧩 Compilando libft"
	@make -C $(LIBFT_DIR) --no-print-directory

%.o: %.c
	@$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

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