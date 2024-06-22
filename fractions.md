
Programming the hp 28C with user defined functions:
========================================================

TL;DR

Long story short, computing with fractions encoded as complex numbers.

List of user defined functions:
-------------------------------

    GGT:    << -> a b 'IFTE(b, GGT(b, MOD(a,b)), a)' >>
    ->RAT:  << -> z n 'z + n*i' >>                        # Note: requires 36 CF
    ZLR:    << -> r 'RE(r)'>>
    NNR:    << -> r 'IM(r)'>>
    RMUL:   << -> p q '->RAT(ZLR(p) * ZLR(q), NNR(p) * NNR(q))' >>
    RINV:   << -> r '->RAT(NNR(r), ZLR(r)' >>
    RADD:   << -> p q '->RAT( NNR(p)*ZLR(q) + NNR(q)*ZLR(p), NNR(p)*NNR(q))' >>
    RSIM:   << -> r '->RAT( ZLR(r)/GGT(ZLR(r), NNR(r)), NNR(r)/GGT(ZLR(r), NNR(r)))' >>

    KGV:    << -> a b 'ABS(a) * ( ABS(B) / GGT(a, b) )' >>


Sample usage (with ball park timings):
--------------------------------------

    89 96 ->RAT  =>  (89,96)          # 0.1 s
    33 45 ->RAT  =>  (33,45)          # 0.1 s
    RADD         =>  (7173,4320)      # 0.5 s
    RSIM         =>  (797,480)        # 1.7 s

    1 3 ->RAT 1 CHS 4 ->RAT RADD  =>  (1,12)   # example for subtraction
    1 3 ->RAT 1 4 ->RAT RINV RMULT  =>  (4,3)  # example for division


Long story:

Since the HP28C is the model I own, we will use this as the
basis for our hp calculator programming endeavor.

The most formidable challenge posed by this model is the
limited amount of available memory. Since it is only 2k
we rather optimize for space instead of speed on this
model.

A secondary consideration is the effort of typing in a
program, since we are often forced to delete programs that
are not used to make room for new stuff. And there is no
way to save and load data or programs. So once we need
the deleted program again we have to type it in.

A third consideration is the simplicity of a solution.

E.g. consider a program to compute the greatest common
denominator of two integers.

Here is an iterative C version:

    int gcd(int a, int b) {
      int t = 0;
      while((t = a % b) != 0) {
        a = b;
        b = t;
      }
      return b;
    }

In HP 28 RPL the arguments a and b are given on the stack.
However they are consumed by any operation, so we have to
duplicate them e.g. before computing the remainder with MOD
and also duplicate the result before testing for 0. Then we
have to keep cleaning up the stack while looping and before
returning the result on stack level 1.

    << WHILE
         DUP2    : duplicate both arguments
         MOD     : compute the remainder
         DUP     : duplicate the remainder
         0 =     : and compare to 0 (this step is not required, here only for clarity)
       REPEAT    : the REPEAT consumes the comparison result
         3 ROLL  : move a down to level 1, remainder up to level 3
         DROP    : and drop a
       END       :
       3 ROLL    : move a down to level 1, remainder up to level 3
       DROP2     : and drop a and b
    >>

All the stack management rather obscures the purpose of the program
and is also alot to type.

Instead we can compute the gcd with a recursive definition, e.g. in C:

    int ggt(int a, int b) {
      return b ? ggt(b, a % b) : a;
    }

(we call this ggt since that is German for gcd ... Groesster Gemeinsamer Teiler)

This translates much more clearly into a user defined function on the HP 28C:

    GGT:  << -> a b 'IFTE(b, GGT(b, MOD(a,b)), a)' >>

(The <var-name>: before the code indicates the variable name for storing it)

As a user defined function GGT is much more useful, as it can be included in
other algebraics and functions, such as

    KGV:  << -> a b 'ABS(a) * ( ABS(B) / GGT(a, b) )' >>

to compute the least common multiple (Kleinstes Gemeinsames Vielfaches).

Of course these benefits are offset by the additional time and memory needed
at run time. So it is important to use reductions that converge quickly.
E.g. in the definition of GGT we use MOD instead of substraction for the re-
duction.

Now for something completely different ...

Fractions

The HP 28C does not support fractions natively (unlike e.g. the HP 48, which does).
But then a fraction is basically just a pair of integer numbers, so we could use
e.g two numbers on the stack or a complex number to represent fractions.
However that consumes at least the memory for two real numbers just to store two
integers. Wouldn't is be more frugal to store both numbers in a single float?

Of course it would ... so we are going to store the numerator before the decimal
point and push the denominator to the right of it (the decimal point, that is).
Then we need separate functions to get at the numerator (easy, just use IP for that)
and to move the denominator back in front of the decimal point.

Lets do the last part first, mathematically we can shift the fractional part of
a floating point number to the left by repeatedly multiplying it by 10 until we're
done, i.e. there is no fractional part left:

    T:  << -> a 'IFTE(FP(a), T(10*a), a)' >>

To push it back in its place we just do the opposite (or inverse), i.e. divide by 10
until no integer part is left:

    TINV: << -> a 'IFTE(IP(a), T(10/a), a)' >>

So now we can write functions to extract numerator (Zaehler) and denominator (Nenner)
from a fraction disguised a floating point number:

    ZLR:  << -> 'IP(r)' >>

    NNR:  << -> r 'ABS(T(FP(r)))' >>

And also to create a fraction (or rational) in disguise from two numbers on the stack:

    ->RAT:  << -> z n 'z + TINV(n)' >>

To multiply two fractions, we got to multiply numerators and denominators:

    RMUL:  << -> p q '->RAT(ZLR(p) * ZLR(q), NNR(p) * NNR(q))' >>

To invert a fraction, we swap numerator and denominator:

    RINV:  << -> r '->RAT(NNR(r), ZLR(r)' >>

To test the functions developed so far lets invert 20/5 with

    20 5 ->RAT

This gives us 20.5 as intended ... now

    RINV

This gives us 5.2 ... oops

Now we discovered a bug in our approach so far. The problem is that
the function TINV that pushes the denominator behind the decimal point
in effect looses any trailing zeros in the denominator. Lets try with

    2000 TINV

This gives us 0.2 ... that's sad ... We could still show the missing
zeros by setting the display two four decimal places with

    4 FIX

which gives 0.2000 but that's a phyrric victory at best since retriving
the denominator with

    ZLR

gives us 2.0000 instead of 2000. This is really a problem in the definition
if T which stops multiplying by 10 as soon as the factional part is 0.

What's really problematic here is our way of encoding fractions in a
real number and we have to find a better solution. 

As mentioned (and dismissed before) we can use complex numbers instead.
For that we rewrite

    NNR:  << -> r 'IM(r)'>>

and

    ZLR:  << -> r 'RE(r)'>>

However we cannot simply use R->C instead of ->RAT in the definitions of RINV
and RMULT since R->C is a command not a function and therefore is not permitted
in algebraics and user defined functions.

Instead we gotta rewrite ->RAT as

    ->RAT:  << -> z n 'z + n*i' >>

For this to actually work we have to clear flag 36 with

    36 CF

in order to force numeric evaluation for algebraics (->NUM is not a function either).

So now we do waste more memory on fractions than neccessary, but at least it is working.
Might also be faster, actually ...

Also our definitions for RMULT and RINV still work:

     2 5 ->RAT

gets us (2,5) and

     1 2 ->RAT

gets us (1,2) and with

     RINV

we get (2,1) and multiplying 

   RMULT

results in (4,5) which looks about right. To save some space we can at least get
rid of T and TINV:

    { T TINV } PURGE

For adding two fractions the simplest formula might well be:

    a   c    a*d + c*b
    - + -  = ---------
    b   d      d * b

so as a function that would be:

    RADD:  << -> p q '->RAT( NNR(p)*ZLR(q) + NNR(q)*ZLR(p), NNR(p)*NNR(q))' >>

Let's test that:

    1 3 ->RAT 1 4 ->RAT RADD

gives (7,12) which may be confirmed by a google search ...

To simplify a fraction we can use the GGT function defined earlier:

    RSIM: << -> r '->RAT( ZLR(r)/GGT(ZLR(r), NNR(r)), NNR(r)/GGT(ZLR(r), NNR(r)))' >>

but we are getting low on memory ('RSIM STO actually does not work) so before entering
that we might want to purge KGV.

So now that leaves us with 562 bytes of memory (see end note)
... but also with the tools to do some math on fractions:

    89 96 ->RAT  =>  (89,96)
    33 45 ->RAT  =>  (33,45)
    RADD         =>  (7173,4320)
    RSIM         =>  (797,480)

Verifying that with emacs calc mode leads to same results (albeit faster).

    1 3 ->RAT 1 CHS 4 ->RAT RADD  =>  (1,12)   # example for subtraction
    1 3 ->RAT 1 4 ->RAT RINV RMULT  =>  (4,3)  # example for division


All in all quite fun what you can do with user defined functions even on the limited HP28C.
We did have to compromise some, since we could not use the optimized encoding using floats
as envisioned. But that was more due to how the decimal system works, wasn't it? Then when
using complex numbers we could not use R->C for ->RAT and rather had to write a formula
and that still required turning on numeric evaluation mode for algebraics with 36 CF.

And yet it is all UDFs, i.e. can be used in algebraics, is also very readable (except for the
German bit maybe), simple and relatively quick to type in.  

End note:

Regarding the 562 bytes left, I also had two addional programs in store:

TIMED (from the 'HP28 Insights' book)
ISPRIME (from the Calculator Culture video on the HP 28)





