NAME = philo

SRCS =	ft_atoi.c \
		functions_1.c \
		functions_2.c \
		data_threads_init.c \
		philo.c \

OBJS = ${SRCS:.c=.o}

HEADER = philo.h

FLAGS	= -Wall -Wextra -Werror

all:	$(NAME)

$(NAME):	$(OBJS) $(HEADER)
	gcc $(FLAGS) $(SRCS) -I. -o $(NAME)

clean:
	rm -f $(OBJS)

fclean:	 clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
