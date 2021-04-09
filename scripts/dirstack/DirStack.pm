package DirStack;

# Standard Perl
use Exporter;
@ISA = qw (Exporter);

# The following is a list of subroutines that the "user" can
# call without the MasterControl:: header
@EXPORT = qw (
              add_directory
              add_unique_element
              change_to_nth_directory
              chop_of_dir_parts
              current_directory
              dir_listing
              grep_directory_names
              print_dirstring
              );

$delim = $ENV{DIRSTACK_DELIM} || ":";
$max_stack = $ENV{DIRSTACK_MAXSTACK} || 25;
$awd = "pwd";
if (defined $ENV{DIRSTACK}) {
    @dirs = split /$delim/, $ENV{DIRSTACK};
} else {
    @dirs = ($ENV{PWD});
}

sub print_dirstring {
    my $string = join $delim, @dirs;
    print $string,"\n";
}

sub add_unique_element {
    my $element = shift;
    # first add the element to the list
    unshift @dirs, $element;
    # now make sure that element isn't on the list further down
    my @problems;
    for (my $index = 1; $index <= $#dirs; ++$index) {
        if ($element eq $dirs[$index]) {
            push @problems, $index;
        }
    }
    foreach $problem (reverse @problems) {
        splice @dirs, $problem, 1;
    }
    # make sure that the list isn't too long
    while (@dirs > $max_stack) {
        pop @dirs;
    }
}

sub dir_listing {
    for (my $index = $#dirs; $index >= 0; --$index) {
        printf "%2d) %s\n", $index + 1, $dirs[$index];
    }
}

sub add_directory {
    my $directory = shift || $ENV{HOME};
    if ($directory =~ /\.\./) {
        my $problem = "";
        my $rest = "";
        # let see how bad this is
        $directory .= "/"; # just in case
        if ($directory =~ m|^((\.\./)+)|) {
            my $match = $1;
            $rest = $';
            #print "match $match: rest $rest\n";
            # are we going up any soft links
            my $current = $ENV{PWD};
            my $num = length ($match) / 3;
            my @parts = split "/", $current;
            shift @parts;
            my $parent = "/";
            if ($num > @parts) {
                $problem = "true";
            } else {
                for my $loop (1..$num) {
                    pop @parts;
                }
                $parent = "/".join("/",@parts);
            }
            chomp ($actual_current = `cd $current/$match; $awd`);
            chomp ($actual_parent = `cd $current/$match; $awd`);
            #print "$actual_current:$actual_parent\n";
            if ($actual_current ne $actual_parent) {
                $problem = "true";                
            }
                
            # are they doing this again later?
            if ($rest =~ m|\.\.|) {
                $problem = "true";
            }
            if (! $problem) {
                $directory = $actual_parent;
            }
        }            
        if ($problem) {
            # I guess we can't do better than this
            chomp ($directory = `cd $directory;$awd`);
        } else {
            $directory = "$actual_parent/$rest";
        }
    }
    # do we need to add full path
    if ($directory !~ m|^/|) {
        $directory = "$ENV{PWD}/$directory";
    }
    $directory =~ s|/\.$||; # kill trailing '/.'s
    $directory =~ s|/+$||; # kill trailing /s
    $directory =~ s|/\./|/|g; # kill '/./'s
    $directory =~ s|/+|/|g; # kill multiple //s
    # make sure we don't erase everything
    if (!length($directory)) {
        $directory = "/";
    }
    # make sure automount isn't screwing with us
    #`cd $directory > /dev/null 2>&1`;
    if (! -d $directory) {
        warn "Error: $directory is not an existing directory\n";
        return;
    }
    #print "adding $directory\n";
    add_unique_element($directory);
}

sub chop_of_dir_parts {
    my $num = shift;
    if ($num !~ /^\d+$/) {
        warn "Error: $num is not an integer.\n";
        return;
    }
    my $dir = current_directory();
    my @parts = split "/", $dir;
    shift @parts;
    for my $loop (1..$num) {
        pop @parts;
    }
    my $newdir = "/".join("/",@parts);
    add_directory ($newdir);
}

sub current_directory {
    return $dirs[0] || ".";
}

sub grep_directory_names {
    my $pattern = shift;
    for (my $index = 1; $index <= $#dirs; ++$index) {
        if ($dirs[$index] =~ /$pattern/i) {
            change_to_nth_directory($index + 1);
            last;
        }
    }
}

sub nth_directory {
    my $nth = shift;
    if ($nth !~ /^-?\d+$/) {
        warn "$nth is not an integer\n";
        return current_directory();
    }
    $nth = int ($nth);
    my $max = @dirs;
    if (abs($nth) > $max) {
        warn "$nth is bigger than the size of the list\n";
        return current_directory();
    }
    if ($nth > 0) {
        --$nth;
    }
    return $dirs[$nth];
}

sub change_to_nth_directory {
    my $nth = shift;
    add_unique_element( nth_directory($nth) );
}
1;
