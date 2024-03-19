NAME = webserv

CPP = c++

SRC_ROOT = $(wildcard ./*.cpp)
# SRC_REQ_PARSER = $(wildcard ./src/request/*.cpp)
# SRC_RESP_PARSER = $(wildcard ./src/response/*.cpp)
# SRC_SERVER = $(wildcard ./src/server/*.cpp)
# SRC_CONFIG = $(wildcard ./src/config/*.cpp)
# SRC_CGI = $(wildcard ./src/cgi/*.cpp)
# SRC_TYPES = $(wildcard ./src/types/*.cpp)
# ALL_SRC = $(SRC_REQ_PARSER) $(SRC_RESP_PARSER) $(SRC_SERVER) $(SRC_CONFIG) $(SRC_ROOT) $(SRC_CGI) $(SRC_TYPES)
OBJ=$(ALL_SRC:.cpp=.o)

# HPP_REQ_PARSER = $(wildcard ./src/request/*.hpp)
# HPP_RESP_PARSER = $(wildcard ./src/response/*.hpp)
# HPP_SERVER = $(wildcard ./src/server/*.hpp)
# HPP_CONFIG = $(wildcard ./src/config/*.hpp)
# HPP_CGI = $(wildcard ./src/cgi/*.hpp)
# HPP_TYPES = $(wildcard ./src/types/*.hpp)
# ALL_HPP = $(HPP_REQ_PARSER) $(HPP_RESP_PARSER) $(HPP_SERVER) $(HPP_CONFIG) $(HPP_CGI) $(HPP_TYPES)

ALL_SRC = $(wildcard ./src/*/*.cpp) $(SRC_ROOT)
ALL_HPP = $(wildcard ./src/*/*.hpp)

CPPFLAGS = -I./src -g -Wall -Wextra -Werror -std=c++98

RM = rm -rf

all: $(NAME)

$(NAME) : $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ) -o $@

%.o : %.cpp $(ALL_HPP)
	$(CPP) $(CPPFLAGS) -c $< -o $@

fclean: clean
	$(RM) $(NAME)

clean:
	$(RM) $(OBJ)

re:	fclean all

push:
	git add .
	git commit -m "changes"
	git push

.PHONY: all clean fclean re
