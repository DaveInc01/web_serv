NAME = webserv

SRC_ROOT = $(wildcard ./*.cpp)
SRC_PARSER = $(wildcard ./src/app/parser/*.cpp)
ALL_SRC = $(SRC_PARSER) $(SRC_ROOT)
OBJ=$(ALL_SRC:.cpp=.o)

CPPFLAGS = #-Wall -Wextra -Werror -std=c++98

RM = rm -rf

all: $(NAME)

$(NAME) : $(OBJ)
	c++ $(CPPFLAGS) $(OBJ) -o $@

fclean:
	$(RM) $(OBJ) $(NAME)

clean:
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re