
Computer Math  on the 28C
=========================

The HP 16C programmable calculator had functions to manipulate and display binary integers, supporting the common bases, 16 (hex), 10 (decimal), 8 (octal) and 2 (binary) and arbitrary word sizes up to 64 bit. Almost identical to what the 28C offers (in the BINARY menu). However, the 16C also supports singned binary integers. It could be configured to use either ones complement or twos complement representation for negative numbers.

Ones complement simply inverts the bits of a given number to represent its negative alter ego, e.g with 4 bit binary intergers

    0001b = 1d, so 1110b = -1d

This can be done easily using the NOT function on the 28C. With this the range of representable numbers is

    -( 2^(WS-1) - 1 )  to  2^(WS-1) - 1    e.g. -7 to 7  with a word size (WS) of 4

The only issue is with this representation you get a signed zero, i.e.

    0000b = 0d, so 1111b = -0d

Twos complement resolves this issue by adding one in the negative range, therefore

    0001b = 1d,   1111b = -1d

With this the range is extended (ever so slightly) to

    -( 2^(WS-1) )  to  2^(WS-1) - 1    e.g. -8 to 7  with a word size (WS) of 4
    
A simple program to convert a positive binary to its twos complement is therefore given as

    << NOT 1 + >>

or more wastefully readable as

<< -> b '(NOT b) + 1' >> for a user defined function.
