Description
-----------

A Babel library. See claytonkb/Babel for more info on Babel.

A note on why I don't use make
------------------------------

Most projects utilize a Makefile to automate their build process. This has the
important benefit that free software projects are easy to build across many
platforms.

Over the years, I've experimented with using autotools to manage builds but
I've finally given up on them completely. The idea of autotools is awesome.
But the actual tools are byzantine, incomprehensible and unusuable.

A build automation tool should be *simpler* than what it is automating. The GCC
command-line is not complex. I find that directly issuing build commands at the
command-line is far simpler than using autotools.

The hardest part of learning a new language is usually learning the syntax --
as bad as the syntax of autotools is, the semantics are even worse. I suppose
if I were one of those people who eats, breathes and dreams in Bash, I would
love the autotools. I'm not and I don't.

Instead of make, I use a Perl script or a simple set of Bash functions to build
my projects. Perl is a venerable and well-integrated tool on *nix platforms.
Unlike autotools, Perl scripts can be debugged and it is easy to write a lucid
subset of the Perl syntax to perform any imaginable command-line task. Perl's
semantics are so transparent that they are obvious from the syntax. For tasks
like tooling GCC, it is downright trivial. Does it have all the bells &
whistles of make? Nope, but it will build my one-coder-scale projects without
any problems. And if problems do arise, you can easily debug them using perl -d
or any other method you prefer.

I don't expect people to do it my way. If you'd like to contribute a Makefile
for any of my projects, please feel free to pull a repo and drop a *working*
Makefile into it. I just find the whole "everyone uses make, why can't you?"
argument mildly frustrating. I don't use make because it's a *horrible* tool.

[What's wrong with GNU make?](http://www.conifersystems.com/whitepapers/gnu-make/)

