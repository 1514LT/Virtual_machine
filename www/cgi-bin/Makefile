target1=./login/login.cgi
target2=./singup/singup.cgi
target3=./find/find.cgi
target4=./write_off/write_off.cgi
FLAGS = -Wall
CC = gcc
all:$(target1) $(target2) $(target3) $(target4) 
$(target1):./login/login.o sqlite3.o
	$(CC) $^ -o $@ $(FLAGS) -lpthread -ldl
$(target2):./singup/singup.o sqlite3.o
	$(CC) $^ -o $@ $(FLAGS) -lpthread -ldl
$(target3):./find/find.o sqlite3.o
	$(CC) $^ -o $@ $(FLAGS) -lpthread -ldl
$(target4):./write_off/write_off.o sqlite3.o
	$(CC) $^ -o $@ $(FLAGS) -lpthread -ldl
%.o:%.c
	$(CC) -c $^ -o $@ $(FLAGS)
.PHONY:clean
clean:
	rm example ./login/*.o -rfv	
	rm example ./singup/*.o -rfv
	rm example ./find/*.o -rfv
	rm example ./write_off/*.o -rfv