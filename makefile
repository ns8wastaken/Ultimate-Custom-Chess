CPP = g++ src/main.cpp
ARGS = -std=c++20 -g -Wall -pedantic -Wextra
DEPS_RAYLIB = -L C:/raylib/lib -lraylib -lgdi32 -lwinmm

all: compile finish open

compile:
	@mkdir -p bin/assets
	$(CPP) $(ARGS) -Isrc/includes -I C:/raylib/include -o bin/main.exe $(DEPS_RAYLIB)

copy-assets:
	@cp -r src/assets/* bin/assets

finish:
	@echo -e "\033[0;32m== Done ==\e[0m"

open:
	@echo -e "\033[0;94mRunning exe...\e[0m"
	@cd "$(CURDIR)/bin" && ./main.exe
