CFLAGS=-Itest -I.
test/bin/float_add_aproc_ex_example : test/float_add_aproc_ex.c test/float_add_aproc_ex_example.c aproc.c buf.c dtypes.c
	$(CC) $(CFLAGS) -o $@ $^
