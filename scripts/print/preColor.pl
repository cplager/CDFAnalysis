#!/usr/bin/env perl

# Assume it is either in the current directory or
# the same directory as the script.
(my $dir = $0) =~ s|[^/]+$||;
my $line = sprintf ("use lib '%s'; use Coloring;", $dir);
my $retval = eval $line;
die "Unable to load Coloring:\n$@\n" if ($@);


####################
## Perl Key Words ##
####################
@perl_preprocessor = ( );

@perl_keywords = 
  qw( ARGV DATA ENV SIG STDERR STDIN STDOUT atan2 bind
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

######################
## Python Key Words ##
######################

@python_keywords = 
qw( and       del       not       while    
    as        elif      global    or        with     
    assert    else      if        pass      yield    
    break     except    print              
    class     eval      exec      in                      
    continue  finally   is        return             
    def       for       lambda
    dict      list      tuple     set);

@python_preprocessor = 
  qw (from import try except finally raise);


###################
## C++ Key Words ##
###################

@c_preprocessor = 
  ("#if defined", "#if !defined", "#elif defined",
   "#include", "#define", "#undef", "#line",
   "#error", "#pragma", "#if", "#ifdef",
   "#ifndef", "#elif", "#else", "#endif" );

@C_keywords = 
  qw(and asm auto bitand bitor bool break case catch char
     class compl const const_cast continue default delete do double
     dynamic_cast else enum explicit export extern false float for friend
     goto if inline int long mutable namespace new not operator or private
     protected public register reinterpret_cast return short signed sizeof
     static static_cast struct switch template this throw true try typedef
     typeid typename union unsigned using virtual void volatile wchar_t
     while NULL Char_t Uchar_t Short_t UShort_t Int_t Uint_t Long_t Ulong_t
     Float_t Double_t Bool_t);

# Make the default Perl
my @preprocessor = @perl_preprocessor;
my @keywords     = @perl_keywords;

# Option parcing
while (@ARGV && $ARGV[0] =~ /^-/) {
   (my $arg = shift) =~ s/^-+//;
   if ($arg =~ m|^perl$|i) {
      @preprocessor = @perl_preprocessor;
      @keywords     = @perl_keywords;      
      next;
   }
   if ($arg =~ m|^python$|i) {
      @preprocessor = @python_preprocessor;
      @keywords     = @python_keywords;      
      next;
   }
   if ($arg =~ m|^c$|i) {
      @preprocessor = @C_preprocessor;
      @keywords     = @C_keywords;      
      next;
   }
   die "I don't understand '-$arg'.  Aborting.\n";
}


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
