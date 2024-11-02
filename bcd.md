
Binary Coded Decimals (BCD)

Now what is this? Binary Coded Decimals use groups of 4 bits (aka nybbles) in a word to encode the numbers 0 to 9.
There are numerous ways to do this, but the most straigthforward scheme is aptly called "natural binary coded decimal" and uses the usual bit patterns for 0 to 9, then skips A-F and moves to the next nybble.

As a first example consider the decimal number 123. Each digit in this number can be encoded in 4 bits as follows:

    #1 = #0001
    #2 = #0010
    #3 = #0011

Now assume a word size of 16 bits the the natural BCD encoding for 123 is

    #0000 0001 0010 0011

(Actually on the 28C leading zeros are not displayed and are shown here only for clarity; the same goes for separating the nybbles with spaces ...)

In BCD hexadecimal representation the original number is shown, e.g.

    #0123

is BCD in base 16 (HEX) for our example (123).

However the same in base 10 (decimal) 

    #291

doesn't tell you much about the original number.

So a simple idea to convert from BCD to real numbers on the 28C might be by string conversion of the hex representation, cutting of the "# " prefix, as follows:

    BCD->R:  << HEX ->STR DUP SIZE 3 SWAP SUB STR-> >>

Let's try this:

    BIN #000100100011 BCD->R  ==>  123

Looks alright.

Should be even simpler the other way around, from real to BCD.

Just turn on hexadecimal,

    HEX

convert the real to a string

    ->STR

prepend "#"

    "#" SWAP +

and extract the binary from the string

    STR->

So here is the program:

    R->BCD:  << HEX ->STR "#" SWAP + STR-> >>



