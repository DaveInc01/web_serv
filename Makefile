NAME = webserv

SRC_ROOT = $(wildcard ./*.cpp)
SRC_REQ_PARSER = $(wildcard ./src/request/*.cpp)
SRC_RESP_PARSER = $(wildcard ./src/response/*.cpp)
SRC_SERVER = $(wildcard ./src/server/*.cpp)
SRC_CONFIG = $(wildcard ./src/config/*.cpp)
SRC_CGI = $(wildcard ./src/cgi/*.cpp)
ALL_SRC = $(SRC_REQ_PARSER) $(SRC_RESP_PARSER) $(SRC_SERVER) $(SRC_CONFIG) $(SRC_ROOT) $(SRC_CGI) 
OBJ=$(ALL_SRC:.cpp=.o)

CPPFLAGS = -I./src -g -std=c++11  #-Wall -Wextra -Werror -std=c++98

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