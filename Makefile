##
## EPITECH PROJECT, 2021
## The Plzza
## File description:
## Makefile
##

CC			=	g++

MAIN		=	$(addprefix $(SRC_D), main.cpp)
OBJ_M		=	$(MAIN:.cpp=.o)

SRC			=	$(addprefix $(SRC_D), $(SRC_F))
OBJ			=	$(SRC:.cpp=.o)
SRC_D		=	src/
SRC_F		=	utils.cpp \
				Reception.cpp \
				Pizza.cpp

SRC_UT		=	$(addprefix $(SRC_UT_D), $(SRC_UT_F))
OBJ_UT		=	$(SRC_UT:.cpp=.o)
SRC_UT_D	=	tests/
SRC_UT_F	=	testReception.cpp

INC			=	-I./inc

CXXFLAGS	=	$(STD) $(INC) $(WFLAGS) $(COVFLAGS)

STD			=	-std=c++17

WFLAGS		=	-W -Wall -Wextra -Werror

DBFLAGS		=	-g -g3 -ggdb

COVFLAGS 	=   --coverage -g -O0

LDFLAGS_UT  =	-lgtest -lgtest_main -lpthread

NAME		=	plazza

NAME_UT		=	unit_tests

all: $(NAME)

$(NAME): $(OBJ_M) $(OBJ)
	$(CC) $(CXXFLAGS) -o $(NAME) $(OBJ_M) $(OBJ)

tests_run: clean $(OBJ) $(OBJ_UT)
	$(CC) $(CXXFLAGS) -o $(NAME_UT) $(OBJ) $(OBJ_UT) $(LDFLAGS_UT)
	./$(NAME_UT)

coverage:
	gcovr -ps --exclude $(MAIN) --exclude tests/

clean:
	rm -f $(OBJ) $(OBJ_M) $(OBJ_UT) *.o *.gc*

fclean: clean
	rm -f $(NAME) $(NAME_UT)

re: fclean all