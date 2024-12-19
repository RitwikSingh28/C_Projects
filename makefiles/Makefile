# hello:
# 	echo "Hello World!"
# 	echo "This line will print if the file hello doesn't exist"

# blah: blah.c
# 	gcc blah.c -o blah
#
# targets: dependencies
# 	command
# 	command
# 	command

# blah: blah.o
# 	gcc blah.o -o blah
# 
# blah.o: blah.c
# 	gcc -c blah.c -o blah.o
# 
# blah.c:
# 	echo "int main(void) { return 0; }" > blah.c

all: some_file

files=file1 file2

some_file: $(files)
	echo "Look at the variable: " $(files)
	touch some_file

file1:
	touch file1
file2:
	touch file2

print: $(wildcard *.c)
	ls -la $?

hey: one two
	echo $@		# Outputs "hey", since this is the target name
	echo $?		# Outputs all prerequisites newer than the target
	echo $^		# Outputs all prerequisites
	echo $<		# Outputs the first prerequisite
	touch hey

one:
	touch one

two:
	touch two

clean:
	rm -f some_file file1 file2 one two *.o hey
