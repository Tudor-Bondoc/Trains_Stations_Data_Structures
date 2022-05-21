EXEC = graf_costuri

build: $(EXEC).c
	gcc $(EXEC).c -o $(EXEC).out -std=gnu99

run:
	./$(EXEC).out

clean:
	rm -f $(EXEC).out