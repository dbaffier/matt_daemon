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

BEN_AFK = Ben_Afk

CFLAGS += -Wall -Werror -Wextra -std=c++11 -pthread

INC_DIR = inc/
INCS = -I$(INC_DIR)

OBJS_DIR = objs/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

OBJS_DIR_BEN = objs_ben/
OBJS_BEN = $(addprefix $(OBJS_DIR_BEN), $(SRCS_BEN:.cpp=.o))

SRCS_DIR = src/
SRCS =	main.cpp					\
		Daemon.cpp					\
		Tintin_reporter.cpp

BEN_DIR = Ben/
SRCS_BEN =	Ben_Afk.cpp				\
			main.cpp

all: $(OBJS_DIR) $(OBJS_DIR_BEN) $(MATT) $(BEN_AFK)

$(OBJS_DIR):
	@mkdir -p $@

$(OBJS_DIR_BEN):
	@mkdir -p $@

$(MATT): $(OBJS)
	g++ $^ -o $@

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	g++ $(CFLAGS) -o $@ -c $< $(INCS)

$(BEN_AFK): $(OBJS_BEN)
	g++ $^ -o $@

$(OBJS_DIR_BEN)%.o: $(BEN_DIR)%.cpp
	g++ $(CFLAGS) -o $@ -c $< $(INCS)

clean:
	rm -rf $(OBJS_DIR)
	rm -rf $(OBJS_DIR_BEN)

fclean: clean
	rm -f $(MATT)
	rm -f $(BEN_AFK)

re: fclean all


.PHONY: fclean all clean re
