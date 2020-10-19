all: clean predictors

predictors: predictors.o 
	gcc -o predictors predictors.c

clean:
	rm -rf predictors predictors.o
