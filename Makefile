redblack : FLAGS = -O3 -g3 -Wall -c -fmessage-length=0
d: FLAGS = -O0 -g -Wall -c -fmessage-length=0

OBJ += src/main.o
 
%.o: %.c
	@echo Building: "$<"
	@gcc  $(INCLUDE) $(FLAGS) -o "$@" "$<"

redblack: $(OBJ)
	@gcc -o "src/redblack" $(OBJ)
	@echo RedBlack Example Created.

d: $(OBJ)
	@gcc -o "src/redblack" $(OBJ)
	@echo RedBlack Debug Example Created.

clean:
	@rm -rf $(OBJ)
	@rm -rf src/redblack 
	@echo Cleanup Finished.
 
all: redblack
