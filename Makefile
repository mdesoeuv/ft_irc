
NAME			:= ircserv

CXXFLAGS		:= -Wall -Werror -Wextra -g3 -std=c++98

DEBUG			:= -fsanitize=address

OPTI			:= -O3

INC				:=	ft_irc.hpp \
					Client.hpp \
					Server.hpp

INC_DIR			:= inc/

INC_FILES		:= $(addprefix $(INC_DIR), $(INC))

SRCS			:=	main.cpp \
					Client.cpp \
					Server.cpp

SRCS_DIR		:= srcs/

SRCS_FILES		:= $(addprefix $(SRCS_DIR), $(SRCS))

OBJS			:= $(SRCS:.cpp=.o)

OBJS_DIR		:= objs/

OBJS_FILES		:= $(addprefix $(OBJS_DIR), $(OBJS))

all:			$(NAME)

$(NAME):		$(OBJS_FILES)
				c++ $(CXXFLAGS) $(DEBUG) $(SRCS_FILES) -o $(NAME)

$(OBJS_DIR)%.o:	$(SRCS_DIR)%.cpp $(INC_FILES)
				mkdir -p $(OBJS_DIR)
				c++ $(CXXFLAGS) $(DEBUG) -c $< -o $@ -I$(INC_DIR)

clean:
				rm -rf $(OBJS_DIR)

fclean:			clean
				rm -f $(NAME)

re:				fclean all

.PHONY:			all clean fclean re 



