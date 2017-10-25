CC=gcc
IDIR=-I.
CFLAGS=-Wall

EXECUTABLE=runnable
OBJ=fsm.o main.o


all: $(EXECUTABLE)

# here $@ == the target (LHS) and $^ == the dependency (RHS)
$(EXECUTABLE): $(OBJ)
	$(CC) -o $@ $^ $(IDIR)

# for any *.o the dependcy is the .c file.
# the $< is the first dependency (the .c in this case) & $@ is the target (LHS i.e. <some>.o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(IDIR)

# just in case there is a clean file, make sure clean runs
.PHONY: clean

# remove all the .o files and the executable.
clean:
	rm -f *.o $(EXECUTABLE)