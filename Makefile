TEST_DIR  ?= tests
BUILD_DIR ?= build
CC        ?= cc
CFLAGS    ?= -I. -Wall -Wextra -g

SINGLE_SRCS := $(wildcard $(TEST_DIR)/*.c)
SINGLE_BINS := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%,$(SINGLE_SRCS))

SUBDIRS := $(wildcard $(TEST_DIR)/*/)

.PHONY: all test clean $(SUBDIRS)

all: $(SINGLE_BINS) $(SUBDIRS)

$(BUILD_DIR)/%: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(SUBDIRS): | $(BUILD_DIR)
	@if [ -f "$@/Makefile" ] || [ -f "$@/makefile" ]; then \
		echo "==> make in $@"; \
		$(MAKE) -C "$@"; \
	else \
		name=$$(basename "$@"); \
		echo "==> compile $@ -> $(BUILD_DIR)/$$name"; \
		$(CC) $(CFLAGS) "$@"/*.c -o "$(BUILD_DIR)/$$name"; \
	fi

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: $(SINGLE_BINS) $(SUBDIRS)
	@for b in $(SINGLE_BINS); do echo "==> run $$b"; "$$b" || exit 1; done

clean:
	rm -rf $(BUILD_DIR)
	@for d in $(SUBDIRS); do \
		if [ -f "$$d/Makefile" ] || [ -f "$$d/makefile" ]; then $(MAKE) -C "$$d" clean || true; fi; \
	done
