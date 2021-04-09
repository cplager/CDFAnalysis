#!/usr/bin/env perl

use lib '/home/cplager/bin/print';
use Coloring;


####################
## Perl Key Words ##
####################
#@preprocessor = ( );

@keywords = qw( ARGV DATA ENV SIG STDERR STDIN STDOUT atan2 bind
    binmode bless caller chdir chmod chomp chop chown chr chroot close
    closedir cmp connect continue cos crypt dbmclose dbmopen defined
    delete die do dump each else elsif endgrent endhostent endnetent
    endprotoent endpwent endservent eof eq eval exec exists exit exp
    fcntl fileno flock for foreach fork format formline ge getc
    getgrent getgrgid getgrnam gethostbyaddr gethostbyname gethostent
    getlogin getnetbyaddr getnetbyname getnetent getpeername getpgrp
    getppid getpriority getprotobyname getprotobynumber getprotoent
    getpwent getpwnam getpwuid getservbyname getservbyport getservent
    getsockname getsockopt glob gmtime goto grep gt hex if import
    index int ioctl join keys kill last lc lcfirst le length link
    listen local localtime log lstat lt m map mkdir msgctl msgget
    msgrcv msgsnd my ne next no oct open opendir ord pack package pipe
    pop pos print printf push q qq quotemeta qw qx rand read readdir
    readlink recv redo ref rename require reset return reverse
    rewinddir rindex rmdir s scalar seek seekdir select semctl semget
    semop send setgrent sethostent setnetent setpgrp setpriority
    setprotoent setpwent setservent setsockopt shift shmctl shmget
    shmread shmwrite shutdown sin sleep socket socketpair sort splice
    split sprintf sqrt srand stat study sub substr symlink syscall
    sysopen sysread system syswrite tell telldir tie tied time times
    tr truncate uc ucfirst umask undef unless unlink unpack unshift
    untie until use utime values vec wait waitpid wantarray warn while
    write y );



while (<>) {
    $line = color_line ($_);
    print "$line\n";
}

#################
## Subroutines ##
#################

sub color_line {
    my $line = shift;
    chomp $line;

    #############
    ## Strings ##
    #############
    my $temp = "\x10temp\x10";
    # replace the '\"' and '\'' with a temporary string
    $line =~ s|\\\"|1$temp|g;
    $line =~ s|\\\'|2$temp|g;
    $line =~ s|\$\'|3$temp|g;
    # color strings
    $line =~ s|(\".*?\")|surround($1, $Coloring::string)|eg;
    $line =~ s|(\'.*?\')|surround($1, $Coloring::string)|eg;
    # put the '\"' back in (colored)
    $line =~ s|1$temp|\\\"|g;
    $line =~ s|2$temp|\\\'|g;
    $line =~ s|3$temp|\$\'|g;
    
    #######################
    ## One line comments ##
    #######################
    $line =~ s|\\\#|1$temp|g;
    $line =~ s|\$\#|2$temp|g;
    $line =~ s|(\#.*$)|surround($1, $Coloring::comment)|eg;
    $line =~ s|1$temp|\\\#|g;
    $line =~ s|2$temp|\$\#|g;

    ##  ########################
    ##  ## Preprocessor words ##
    ##  ########################
    ##  foreach my $preproc (@preprocessor) {
    ##      $line =~ s|$preproc|surround($preproc, $Coloring::pre)|eg;
    ##  }

    ##############
    ## Keywords ##
    ##############
    foreach my $keyword (@keywords) {
        $line =~ s|\b$keyword\b|surround($keyword, $Coloring::keyword)|eg;
    }
    $line = remove_extra_keys($line);
    return $line;
}
