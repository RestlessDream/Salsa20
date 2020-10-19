# Salsa20

## Description
- *main.c* - provides a command line user interface;
- *salsa20.h, salsa20.c* - contains the main part of Salsa20 alghorithm, fully compliant to standard.
- *static.h, static.c* - contains the static part of algorithm (e.g. *expend_16, expend_32 ...*);
- *types.h* - contains typedef aliases;
- *util.h, util.c* - contains utility methods (e.g. *rotate_left, little_endian ...*);

### Unit tests:
- *test_salsa20.c* - Salsa20 standard test set;
