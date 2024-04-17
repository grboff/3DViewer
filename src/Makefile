CC = gcc
#--------------------------------------------------------------------------------------------
CFLAGS = -std=c11 -Wall -Werror -Wextra
#--------------------------------------------------------------------------------------------
FUNCS = c_files/s21_view.c
#--------------------------------------------------------------------------------------------
TEST = viewer
TEST_FILES = test_files/test_viewer.c $(FUNCS)
TEST_FLAGS = -lcheck
#--------------------------------------------------------------------------------------------
REPORT = gcov_report
GCOV_REPORT = gcov/$(REPORT)
GCOV_INFO = $(GCOV_REPORT).info
GCOV_FILES = *.gcno *.gcda gcov

GCOV_FLAGS  = --coverage $(TEST_FLAGS)
#--------------------------------------------------------------------------------------------
FILES = c_files/*.c c_files/*.h
#--------------------------------------------------------------------------------------------
APP = 3DViewer.app

OS := $(shell uname)
ifeq ($(OS), Linux)
	TEST_FLAGS += -lm -lsubunit
	GCOV_FLAGS += -lrt -lsubunit -lpthread
	APP = 3DViewer
endif
#--------------------------------------------------------------------------------------------
.PHONY: all test clean rebuild $(REPORT) check leaks install uninstall dist
#--------------------------------------------------------------------------------------------

all: install

test:
	clear
	@echo "\n\n=============RANNING TESTS=============="
	$(CC) $(CFLAGS) $(TEST_FILES) $(TEST_FLAGS) -o $(TEST)
	./$(TEST)
	@rm $(TEST)
	@echo "========================================"

clean:
	@echo "\n\n================CLEANING================"
	rm -rf build
	rm -rf gcov *.gcda *.gcno report
	rm -rf $(TEST)
	rm -rf .clang-format
	rm -rf a.out c_files/a.out RESULT.txt
	rm -rf 3DViewer/3DViewer.app 3DViewer/Makefile 3DViewer/.qmake.stash
	@echo "========================================"

rebuild: clean all

$(REPORT): clean
	@echo "\n\n==============GCOV COVERAGE=============="
	mkdir -p gcov
	$(CC) $(TEST_FILES) $(GCOV_FLAGS) -o $(GCOV_REPORT)
	@echo "\n-COMPILATION-"
	./$(GCOV_REPORT)
	@echo "\n-LCOV-"
	mv $(GCOV_FILES)
	lcov -t "$(REPORT)" -o $(GCOV_INFO) -c -d gcov
	@echo "\n-GENHTML-"
	genhtml -o report $(GCOV_INFO) $(GCOV_INFO)
	@echo "\n-OPEN RESULT-"
	open report/index.html
	@echo "========================================"

check:
	clear
	@cp ../materials/linters/.clang-format .
	@echo "\n======= CHECK GOOGLE CODE STYLE ======="
	clang-format -n $(FILES) 3DViewer/*.cpp 3DViewer/*.h
	@echo "\n=========== CHECK CPPCHECK ============"
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config $(FUNCS) c_files/*.h
	@rm .clang-format

leaks:
	@echo "\n\n===============LEAKS TEST==============="
ifeq ($(OS) ,Darwin)
	$(CC) $(CFLAGS) $(TEST_FILES) $(TEST_FLAGS) -o $(TEST)
	leaks -atExit -- ./$(TEST)
else
ifeq ($(OS) ,Linux)
	$(CC) $(CFLAGS) -g $(TEST_FILES) -o $(TEST) -lcheck -lpthread -lm -D_GNU_SOURCE -lrt -lsubunit
	CK_FORK=no valgrind \
	--leak-check=full \
	--trace-children=yes \
	--track-fds=yes \
	--track-origins=yes \
	--leak-check=full \
	--show-leak-kinds=all \
	--verbose \
	./$(TEST)
endif
endif
	@rm $(TEST)
	@echo "========================================"

install:
	@rm -rf build
	mkdir build
	cd 3DViewer/ && qmake6 && make && make clean && cp -r $(APP) ../build

uninstall:
	rm -rf build

dist:
	@if [ ! -d build ] ; then echo "creating build" ; make install; fi
	@if [ -d build ] ; then tar -zcvf archive.tar build; else echo "build not exists, error!"; exit 1; fi

dvi:
	open ./page/readme.html