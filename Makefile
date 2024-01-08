NAME = webserv

SRC_ROOT = $(wildcard ./*.cpp)
SRC_REQ_PARSER = $(wildcard ./src/app/request/*.cpp)
SRC_RESP_PARSER = $(wildcard ./src/app/response/*.cpp)
SRC_SERVER = $(wildcard ./src/app/server/*.cpp)
SRC_CONFIG = $(wildcard ./src/app/config/*.cpp)
ALL_SRC = $(SRC_REQ_PARSER) $(SRC_RESP_PARSER) $(SRC_SERVER) $(SRC_CONFIG) $(SRC_ROOT)
OBJ=$(ALL_SRC:.cpp=.o)

CPPFLAGS = -I./src/app #-Wall -Wextra -Werror -std=c++98

RM = rm -rf

all: $(NAME)

$(NAME) : $(OBJ)
	c++ $(CPPFLAGS) $(OBJ) -o $@

fclean:
	$(RM) $(OBJ) $(NAME)

clean:
	$(RM) $(NAME)

re:	fclean all

push:
	git add .
	git commit -m "changes"
	git push

.PHONY: all clean fclean re