# **************************************************************************** #
#                                   MAKEFILE                                   #
# **************************************************************************** #

# --- Configuração ---
NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror

# --- Diretórios ---
INC_DIR     = include
SRC_DIR     = src
LIBFT_DIR   = libft

# --- Pastas do projeto ---
SRC_DIRS = \
	validation \
	tokenization \
	parsing \
	execution \
	builtins \
	signals \
	expansion

# --- Arquivos Fonte ---
SRC_FILES =  main.c \
	tokenization/lexer.c \
	tokenization/token_utils.c \
	tokenization/redirect_utils.c \
	parsing/parser.c \
	parsing/command_utils.c \
	execution/execute.c \
	execution/envp_utils.c \
	execution/path_utils.c \
	execution/free_utils.c \
	builtins/builtin_utils.c \
	builtins/cd.c \
	builtins/echo.c \
	builtins/env.c \
	builtins/exit.c \
	builtins/export.c \
	builtins/pwd.c \
	builtins/unset.c

# --- Geração dos caminhos completos ---
SRCS = $(addprefix $(SRC_DIR)/, $(filter-out main.c, $(SRC_FILES))) ./main.c
OBJS = $(SRCS:.c=.o)

# --- Libs ---
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
