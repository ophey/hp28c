
Computer Math  on the 28C
=========================

The HP 16C programmable calculator had functions to manipulate and display binary integers, supporting the common bases, 16 (hex), 10 (decimal), 8 (octal) and 2 (binary) and arbitrary word sizes up to 64 bit. Almost identical to what the 28C offers (in the BINARY menu). However, the 16C also supports singned binary integers. It could be configured to use either ones complement or twos complement representation for negative numbers.

Ones complement simply inverts the bits of a given number to represent its negative alter ego, e.g with 4 bit binary intergers

    0001b = 1d, so 1110b = -1d

This can easily be done using the NOT function on the 28C. With this the range of representable numbers is

    -( 2^(WS-1) - 1 )  to  2^(WS-1) - 1    e.g. -7 to 7  with a word size (WS) of 4

The only issue is with this representation you get a signed zero, i.e.

    0000b = 0d, so 1111b = -0d

Twos complement resolves this issue by adding one in the negative range, therefore

    0001b = 1d,   1111b = -1d

With this the range is extended (ever so slightly) to

    -( 2^(WS-1) )  to  2^(WS-1) - 1    e.g. -8 to 7  with a word size (WS) of 4
    
A simple program to convert a positive binary to its twos complement is therefore given as

    ->2C:  << NOT 1 + >>

or more wastefully readable as

    ->2C:  << -> b '(NOT b) + 1' >> for a user defined function.

With this we can add and subtract binary numbers as follows, (WS=4, base 10, alpha mode entry):

    #5 #2 ->2C +  [ENTER]  ==>  #3

This is like computing 5-2 as 5 2 CHS + with real numbers, i.e. ->2C acts like CHS but for binaries.
Care has to be taken, not to exceed the range, e.g

    #5 #14 + [ENTER]  ==>  #3

will (perhaps unintentionallay) subtract #2 instead of add #14 since

    #2 ->2c [ENTER]  ==>  #14

Twos complement conversion shares some properties with CHS, e.g. just as

    x CHS CHS  ==>  x

also

    b ->2C ->2c  ==>  b , with -(2^(WS-1) - 1) <= b <= 2^(WS-1) - 1
    
Interestingly enough

    -(2^(WS-1)) ->2C  ==>  -(2^(WS-1))

e.g. for WS = 4

    -(2^(4-1)) = -8 = #1000b

and

    #1000b ->2C = (NOT #1000b) + 1 = #0111b + 1 = #1000b




Of course ordinary conversion to real with B->R does not take twos complement for binary into account.

For this we have to write our own C2->R program. So how do we do this.

First we have to decide if the number is negative. In both complement representations this is the case whenever the highest bit is set. This is the case whenever a given binary, say b

    b >= 2^(WS-1)

so in RPL our test would be

    b 2 RCWS 1 - ^ R->B >=

Nah, this can be done quicker. Laboriously computing 2^(WS - 1) and converting to binary will always lead to

    #100 .. 0

i.e binrary integer with the highest bit set only. This can be done quick by taking a binary one and rotating it right just once, so our test now is:

    b #1 RR >=


In this case we have to flip the sign on b (with ->2C as it were), convert that to real (B->R) and change the sign on the resulting real:

    b ->2C B->R NEG

Otherwise (i.e. most significant bit not set) we just convert b to real with B->R:

    b B->R

So here ist the complete progam for converting signed binaries to real:

    C2->R:  << -> b << IF
                         b # 1 RR >=            // b >= 2^(wordsize - 1) i.e. MSB is set
                       THEN                     //
                         b ->2C B->R NEG        // negate twos complement of b
                       ELSE                     //
                         b B->R                 // just b
                       END
                    >> >>


