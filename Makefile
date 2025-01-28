main: main.o setup.o translator.o
	gcc -o VMtranslator main.o setup.o translator.o
	@echo "\e[32mcompilation ended"

setup.o: setup.c setup.h
	gcc -c setup.c

translator.o: translator.c translator.h
	gcc -c translator.c

#@start the command without printing output on terminal
# -f to force remove
clean:
	@rm -f *.o *.asm VMtranslator
	@echo "\e[32mtemporary file removed"
