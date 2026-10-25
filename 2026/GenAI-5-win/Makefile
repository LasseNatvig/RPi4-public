# Makefile - Build configuration - Example code for TDT4258 - 2026 edition
# 

CC = clang
CDEBUGFLAGS = -std=c17 -fdiagnostics-absolute-paths -Wall -g -target x86_64-windows-msvc
CFLAGS = -std=c17 -O2 -fdiagnostics-absolute-paths -Wall -target x86_64-windows-msvc
LDLIBS =

all: debug.exe release.exe test_all.exe

debug.exe: main.c src/quadratic.c
	$(CC) $(CDEBUGFLAGS) -g -I. -Isrc -o debug.exe main.c src/quadratic.c $(LDLIBS)

release.exe: main.c src/quadratic.c
	$(CC) $(CFLAGS) -I. -Isrc -o release.exe main.c src/quadratic.c $(LDLIBS)

# Build all tests together (runs all test suites)
test_all.exe: tests/all_tests.c tests/test_quadratic.c unity/unity.c src/quadratic.c
	$(CC) $(CDEBUGFLAGS) -I. -Isrc -Iunity -Itests -g -o test_all.exe tests/all_tests.c tests/test_quadratic.c unity/unity.c src/quadratic.c $(LDLIBS)

# Run all tests
run-tests: test_all.exe
	.\test_all.exe

clean:
	-del *.exe
	-del *.o
	-del *.obj
	-del *.ilk
	-del *.pdb

