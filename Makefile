
lsall:sc1

sorter: sc1.c
	gcc -O0 -Wall -Werror -std=c99 -g sc1.c -o sc1 -lm       

clean:
	rm -rf sc1
