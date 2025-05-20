CFLAGS?=-Wall -Werror -Wpedantic -march=native -O3 -D DEBUG
SHELL=sh

BUILDDIR=build
SRCDIR=src

$(BUILDDIR)/%: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(?) -o $(@)
