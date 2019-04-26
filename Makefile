CFLAGS=-g -Itest -I.
test/bin/float_add_aproc_ex_example : test/float_add_aproc_ex.c test/float_add_aproc_ex_example.c aproc.c buf.c dtypes.c
	$(CC) $(CFLAGS) -o $@ $^

test/bin/bbo_add : test/bbo_add_test.c aproc.c buf.c dtypes.c bbo_add.c buf_bin_op.c vmath_native.c
	$(CC) $(CFLAGS) -o $@ $^
