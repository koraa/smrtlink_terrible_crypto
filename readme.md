# WTF – Smartlink Terrible Crypto

This is an implementation of the crypto (RC4) algorithm TP-Link
Easy Smart Switches use to encrypt their configuration
traffic with.

It also includes the key they use for this because they use
one key globally. Idiots!

Thanks to @jdi-li who reverse-engineered the algorithm and
they key (by decompiling some badly-obfuscated Java code).
He's implementing an open source tool to communicate with
the switches: https://github.com/jdi-li/smrtlink

# The Algorithm

Turns out this is actually the [ RC4 ](
https://en.wikipedia.org/wiki/RC4#Pseudo-random_generation_algorithm_.28PRGA.29) (which I didn't recognise).

Thanks to @BenBE1987 for pointing this out!

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
