# make.pl
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
    `mkdir -p obj`;
    chdir "src";
    `gcc -c *.c`;
#    `mv *.o ../obj`;
#    chdir "../";
    `ar rcs libbabel.a *.o`;
    `rm *.o`;
    `mv *.a ../obj`;
    chdir "../";

}

sub build{
    `mkdir -p bin`;
    my @objs = `ls obj`;
    my $obj_string = "";
    for(@objs){ chomp $_; $obj_string .= "obj/$_ " };
    my $build_string =
        "gcc test/main.c $obj_string -Isrc -o bin/test";
    `$build_string`;
}

sub clean{
    `rm -rf obj`;
    `rm -rf bin`;
}

