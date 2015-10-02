# WTF – Smartlink Terrible Crypto

This is an implementation of the crypto algorithm TP-Link
Easy Smart Switches use to encrypt their configuration
traffic with.

It also includes the key they use for this because they use
one key globally. Idiots!

Thanks to @jdi-li who reverse-engineered the algorithm and
they key (by decompiling some badly-obfuscated Java code).
He's implementing an open source tool to communicate with
the switches: https://github.com/jdi-li/smrtlink

# The Algorithm

It's a symmetric stream cypher.

Two steps: First a keystream with the length of the plaintext
is generated using a built-in PRNG, then the plaintext is
XORed with the keystream.

No padding or salting is performed, as the original
algorithm didn't do so either.

## Decrypt

Same as encrypting but with the cyphertext as input, since
`A = xor(xor(A, B), B)`.  (XORing with some data A and
XORing again produces the input).

## PRNG

I haven't been able to identify this PRNG. I assume it's
a proprietary algorithm.

It essentially uses indirect access to key elements (using
a value from the key as a key index, and doing that again...),
swapping key elements and one accumulative variable to
generate a chaotic behaviour.

I am pretty sure that the PRNG is non cryptographic.

## Security

Obviously this should not be used seriously. It's not
cryptographically safe.

Each switch stores *THE SAME* static key. In the Java source
this key was found. It was a bit scrambled but the
scrambling algorithm is also stored in the Java source.
This repo stores the resulting key in tplink_kompromat_key.
For this reason their entire crypto is broken regardless of
how safe the algorithm is.

The keystream can be recovered when the plaintext is known
(simply by XORing).

Any other plaintext can then be recovered, simply by using the
keystream of a sufficient length.

I think the keystream's symbol frequency is very similar to the
key's. Additionally the keystream can only contains symbols
the key also contains.
This makes it a lot easier to recover the key using
*enhanced* brute force.

It should also be possible to recover the key from
a sufficiently long keystream by trying different bytes that
would produce a specific keystream, but I haven't yet
produced a viable exploit for this.

# Building

Run make with a C++14 compiler.
Compiler will be used as linker driver.

# LICENSE

Copyright © 2015 Karolin Varner

Released under zlib; buy me a drink if you like it.

By filing a pull request you agree that your contribution
will also be publicised under the same conditions.

## zlib license

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this
software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it
and redistribute it freely, subject to the following
restrictions:

1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgement
   in the product documentation would be appreciated but is
   not required.
2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original
   software.
3. This notice may not be removed or altered from any source
   distribution.
