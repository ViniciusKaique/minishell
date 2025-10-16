# **************************************************************************** #
#                                   MAKEFILE                                   #
# **************************************************************************** #

# --- Variáveis do Projeto ---
NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g # Flags de compilação padrão [cite: 236]

# --- Diretórios ---
INC_DIR     = include
SRC_DIR     = src
LIBFT_DIR   = libft

# --- Arquivos Fonte (.c) ---
# Lista todos os arquivos .c que compõem o projeto.
SRC_FILES   = main.c \
              parser.c \
              executor.c \
              builtins.c \
              env_manage.c

# Adiciona o prefixo de diretório (src/) aos arquivos, exceto para main.c
SRCS        = $(addprefix $(SRC_DIR)/, $(filter-out main.c, $(SRC_FILES))) main.c

# --- Arquivos Objeto (.o) ---
# Gera a lista de arquivos .o a partir da lista de .c
OBJS        = $(SRCS:.c=.o)

# --- Bibliotecas ---
LIBFT       = $(LIBFT_DIR)/libft.a
# A biblioteca readline é necessária para o prompt interativo.
# -lncurses é uma dependência comum da readline.
LIBS        = -lreadline -lncurses

# --- Comandos ---
RM          = rm -f

# **************************************************************************** #
#                                   REGRAS                                     #
# **************************************************************************** #

# Regra padrão: compila o projeto inteiro.
all: $(NAME)

# Regra para compilar o executável final.
# Depende de todos os arquivos objeto (.o) e da libft.a.
$(NAME): $(OBJS) $(LIBFT)
	@echo "🔗 Vinculando tudo para criar $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -I $(INC_DIR) -o $(NAME)
	@echo "✅ Compilado com sucesso: $(NAME)"

# Regra para compilar a biblioteca libft.
# Executa o Makefile dentro do diretório da libft.
$(LIBFT):
	@echo "🧩 Compilando libft..."
	@make -C $(LIBFT_DIR) --no-print-directory

# Regra genérica para compilar arquivos .c em .o
# O compilador é informado para procurar headers no diretório de include.
%.o: %.c
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

# Regra para limpar os arquivos objeto.
clean:
	@$(RM) $(OBJS)
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@echo "🧹 Objetos do Minishell removidos."

# Regra para uma limpeza completa (objetos + executável).
fclean: clean
	@$(RM) $(NAME)
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@echo "🧽 Tudo limpo."

# Regra para recompilar tudo do zero.
re: fclean all

# Regra de bônus (atualmente vazia, mas exigida pelo enunciado) [cite: 239]
bonus: all

# Indica que estas regras não correspondem a nomes de arquivos.
.PHONY: all clean fclean re bonus