
Probabilities and Binomial Distribution
=======================================

The end of my son's school education draws near, and as is the custom when there is nothing decent left to learn the math teacher reaches for her old book on probability therory to confuse the kids even more.

So they get into combinatorics with drawing red and black balls from a bowl and hell if I know if we are supposed to put them back.

Whatever, the way I see it, it's as good an excuse as any to get out my HP28C and see what it can do. Turns out, not so much on its own, doesn't even have the COMB and PERM functions, like some newer models (heck even the 28S has them).

For the Binomial Distribution we need the COMB function, so let's start with that. Here is a quote from some HP 48G book on this stuff:

 [COMB]: Calculates the number of combinations of n (level 2) items taken r (level 1) at a time;

Now the 48 had that function build in so there is no formula in the book. But there is a similar book for the 28C which has a formula and even a better idea, i.e. start with the PERM function and use that in the definition of COMB.

So here goes:

    PERM: << -> x y 'FACT(x)/FACT(x-y)' >>

and

    COMB: << -> x y 'PERM(x,y)/FACT(y)' >>

That's basically where the 28C book stops regarding these matters ...

So back to the 48 book where the provide this definition for the binomial probability distribution:

    BPD: << -> x n p 'COMB(n,x)*p^x*(1-p)^(n-x)' >>

(actually, there is a typo in the book where they skipped the x* part in the formula ...)



