.PHONY: help run build build-cross
help :
	@echo "Use this make file to execute the following targets: "
	@echo "* run: run against the test case"
	@echo "* build: Builds the project and produces binary in bin/ directory."
	@echo "* build-cross: Cross-builds the project and produces binary in bin/ directory."

APP_NAME := sexpr
BINDIR := ./bin

DEFS:=-D_XOPEN_SOURCE
CFLAGS:=-std=gnu11 -ggdb -O0
build:
	@cc main.c $(CFLAGS) $(DEFS) -o $(BINDIR)/$(APP_NAME)
run: build
	@$(BINDIR)/$(APP_NAME) ./test/example.sexp
clean:
	@rm -f $(BINDIR)/$(APP_NAME)


