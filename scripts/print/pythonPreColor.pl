#!/usr/bin/env perl

use lib '/home/cplager/bin/print';
use Coloring;


####################
## Perl Key Words ##
####################
#@preprocessor = ( );

@keywords = qw( and       del       not       while    
                as        elif      global    or        with     
                assert    else      if        pass      yield    
                break     except    print              
                class     eval      exec      in                      
                continue  finally   is        return             
                def       for       lambda
                dict      list      tuple     set);

@preprocessor = qw (from import try except finally raise);


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

    ########################
    ## Preprocessor words ##
    ########################
    foreach my $preproc (@preprocessor) {
        $line =~ s|$preproc|surround($preproc, $Coloring::pre)|eg;
    }

    ##############
    ## Keywords ##
    ##############
    foreach my $keyword (@keywords) {
        $line =~ s|\b$keyword\b|surround($keyword, $Coloring::keyword)|eg;
    }
    $line = remove_extra_keys($line);
    return $line;
}
