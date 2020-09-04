# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/24 00:18:28 by dbaffier          #+#    #+#              #
#    Updated: 2020/08/24 20:14:13 by dbaffier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MATT = Matt_daemon

CFLAGS += -Wall -Werror -Wextra -std=c++11 -pthread

INC_DIR = inc/
INCS = -I$(INC_DIR)

OBJS_DIR = objs/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

SRCS_DIR = src/
SRCS =	main.cpp					\
		Daemon.cpp

all: $(OBJS_DIR) $(MATT)

$(OBJS_DIR):
	@mkdir -p $@

$(MATT): $(OBJS)
	g++ $^ -o $@

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	g++ $(CFLAGS) -o $@ -c $< $(INCS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(MATT)

re: fclean all


.PHONY: fclean all clean re
