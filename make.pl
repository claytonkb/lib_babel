#!/usr/bin/perl
#
# Use this to build lib_babel on any standard *nix with GCC
#
# Use at your own risk. I use Perl instead of autotools because the latter
#    are an opaque morass of Mad Hatter gibberish.
#
#   Usage:
#
#   Build all:
#       perl make.pl
#
#   Clean:
#       perl make.pl clean
$optimize=0;

unless ($#ARGV > -1) {
    libs();
    build();
}
else{
    if($ARGV[0] eq "clean"){
        clean();
    }
    elsif($ARGV[0] eq "libs"){
        libs();
    }
    elsif($ARGV[0] eq "all"){
        libs();
        build();
    }
}

sub libs{
    `mkdir -p lib`;
    chdir "src";
    `gcc -O3 -c *.c` if $optimize;
    `gcc -c *.c` unless $optimize;
    `ar rcs libbabel.a *.o`;
    `rm *.o`;
    `mv *.a ../lib`;
    chdir "../";

}

sub build{
    `mkdir -p bin`;
    chdir "test";
    my $build_string =
        "gcc -c main.c -I../src";
    `$build_string`;
    $build_string =
        "gcc main.o -lbabel -L../lib -o ../bin/test";
    `$build_string`;
}

sub clean{
    `rm -rf lib`;
    `rm -rf bin`;
}

