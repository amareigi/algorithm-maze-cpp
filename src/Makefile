TARGET=Maze
FLAGS=-Wall -Werror -Wextra -std=c++17
CC=g++ $(FLAGS)
FILE=Maze
MODEL=model/*
CONTROLLER=controller/*
FILETEST=tests/*
FILEREPORT=s21_test_report
FILETESTO=s21_test
DIRREPORT=report
COVFLAG=-fprofile-arcs -ftest-coverage 
ASAN=-g -fsanitize=address
FILTER="/usr/include/*" "l1/*"

OS = $(shell uname)

ifeq ($(OS), Darwin)
	TESTFLAGS=-pthread -lcheck -lgtest -lstdc++
	OPEN_CMD=open
else
	TESTFLAGS=-pthread -lcheck_pic -lrt -lsubunit -lgtest -lstdc++
	OPEN_CMD=xdg-open
endif

all: clean gcov_report install

install:
	mkdir ./build
	mkdir ./app
	cd ./build && qmake ../view/ && make && cp view ../app/$(TARGET).app
	
uninstall:
	rm -rf ./app ./build

dist:
	mkdir dist
	mkdir dist/src
	cp -r view/ files/ tests/ model/ controller/ Makefile *.h  dist/src/
	cd ./dist && tar cvzf $(TARGET)_v2.0.tgz *
	rm -rf dist/src/

dvi:
	$(OPEN_CMD) $(TARGET).html

test: 
	$(CC) $(FILETEST).cpp $(MODEL).cpp $(CONTROLLER).cpp -o $(FILETESTO) $(TESTFLAGS)
	./$(FILETESTO)

asan: 
	$(CC) $(ASAN) $(FILETEST).cpp $(MODEL).cpp $(CONTROLLER).cpp -o $(FILETESTO) $(TESTFLAGS)
	./$(FILETESTO)

test_val: 
	$(CC) $(FILETEST).cpp $(MODEL).cpp $(CONTROLLER).cpp -o $(FILETESTO) $(TESTFLAGS)
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes -s ./$(FILETESTO)

gcov_report:
	$(CC) $(COVFLAG) $(FILETEST).cpp $(MODEL).cpp $(CONTROLLER).cpp -o $(FILEREPORT) $(TESTFLAGS)
	./$(FILEREPORT)
	mkdir $(DIRREPORT)
	gcovr --html-details --exclude $(FILETEST) -o $(DIRREPORT)/report.html
	rm -rf *.o *.a *.gcno *.gcda *.info *.log $(FILETESTO) $(FILEREPORT)
	$(OPEN_CMD) $(DIRREPORT)/report.html

rep: gcov_report

clean:
	rm -rf *.o *.a *.gcno *.gcda *.info *.log *.txt $(DIRREPORT) $(FILETESTO) \
	$(FILEREPORT) ./build* ./dist ./app *.out

#style
cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem *.cpp *.hpp *.h \
	$(MODEL).cpp $(MODEL).hpp $(FILETEST).cpp $(CONTROLLER).cpp $(CONTROLLER).hpp
