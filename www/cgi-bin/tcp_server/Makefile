target1 = server
CC = gcc
all:$(target1)
$(target1):main.o sqlite3.o
	$(CC) $^ -o $@ $(FLAGS) -lpthread -ldl
%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS) 
.PHONY:clean
clean:
	rm $(target1) *.o -rfv	
