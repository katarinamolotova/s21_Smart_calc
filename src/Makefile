WWW = -std=c++17 -Wall -Werror -Wextra -lstdc++
LINUX=-lsubunit -lrt -lpthread -lm
LFLAGS=-fprofile-arcs -ftest-coverage
LIBS=-lgtest -lgmock -pthread
OS=$(shell uname -s)

all: clean test

rebuild: clean all 

test: clean s21_smartcalc_oop.a
ifeq ($(OS), Darwin)
	gcc $(WWW) tests/test.cpp s21_smartcalc_oop.a -o test $(LIBS)
else
	gcc $(WWW) tests/test.cpp s21_smartcalc_oop.a -o test $(LIBS) $(LINUX)
endif
	./test

gcov_report: clean_test
ifeq ($(OS), Darwin)
	gcc $(WWW) --coverage tests/test.cpp controller/s21_*.cc model/s21_*cc -o test $(LIBS)
else
	gcc $(WWW) --coverage tests/test.cpp controller/s21_*.cc model/s21_*cc -o test $(LIBS) $(LINUX)
endif
	./test
	lcov -t "test" -o test.info --no-external -c -d .
	genhtml -o report test.info
	open report/index.html
	rm -rf test.* s21_*.* test s21_*

s21_smartcalc_oop.a: build_s21_smartcalc_oop

dvi:
	open dvi.html

dist:
	tar -cf ../build/dist.tar ../build/SmartCalc_v2.app


install:
	cd view && qmake && make && make clean && rm Makefile && cd ../ && mv view/SmartCalc_v2.app ../build

uninstall:
	rm -rf ../build/SmartCalc_v2.app

build_s21_smartcalc_oop:
	gcc -c controller/s21_*.cc model/s21_*cc
	# gcc -c controller/s21_*.cc model/s21_*cc tests/test.cpp
	ar rcs s21_smartcalc_oop.a s21_*.o
	ranlib s21_smartcalc_oop.a

clean_obj:
	rm -rf *.o

clean_lib: 
	rm -rf *.a

clean_test:
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	rm -rf test
	rm -rf report

clean: clean_lib clean_test clean_obj
	# rm -rf s21_test