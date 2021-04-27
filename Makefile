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
SRC_F		=	utils.cpp

SRC_UT		=	$(addprefix $(SRC_UT_D), $(SRC_UT_F))
OBJ_UT		=	$(SRC_UT:.cpp=.o)
SRC_UT_D	=	tests/
SRC_UT_F	=

INC			=	-I./inc

STD			=	-std=c++17

CXXFLAGS	=	$(STD) -W -Wall -Wextra -Werror $(INC)

LDFLAGS_UT  =	-lgtest -lgtest_main -lpthread

DBFLAGS		=	-g -g3 -ggdb

NAME		=	plazza

NAME_UT		=	unit_tests

all: $(NAME)

$(NAME): $(OBJ_M) $(OBJ)
	$(CC) $(CXXFLAGS) -o $(NAME) $(OBJ_M) $(OBJ)

tests_run: clean $(OBJ) $(OBJ_UT)
	$(CC) $(CXXFLAGS) -o $(NAME_UT) $(OBJ) $(OBJ_UT) $(LDFLAGS_UT)
	./$(NAME_UT)

coverage:
	gcovr -s --exclude tests/

clean:
	rm -f $(OBJ) $(OBJ_M) $(OBJ_UT) *.o *.gc*

fclean: clean
	rm -f $(NAME) $(NAME_UT)

re: fclean all