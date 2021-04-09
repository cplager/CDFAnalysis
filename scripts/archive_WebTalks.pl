#!/usr/bin/env perl
#!/usr/bin/perl

use LWP;
if (!eval "use Term::ReadKey") {
    if ($@) {
        $badPassword = "true";
        warning();
     }
}

if (! $badPassword) {
    use Term::ReadKey;
}

#use PromptUtil;
use vars '@ISA';
@ISA = 'LWP::UserAgent';

die "Usage: $0 http://www-cdf.fnal.gov/internal/WebTalks/0303/030301_random.html\n" unless (@ARGV);

my $url = shift @ARGV;
@dont = @ARGV;
@ARGV = ();

foreach $number (@dont) {
    if ($number =~ /(\d+)\-(\d+)/) {
        $start = $1;
        $finish = $2;
        for $loop ($start..$finish) {
            $key = sprintf "%02d", $loop;
            push @list, $key;
        }
        next;
    }
    if ($number =~ /(^\d+)/) {
        $key = sprintf "%02d", $1;
        push @list, $key;
        next;
    }
    if ($number =~ /^-only/i) {
        $only = "true";
        warn "only\n";
        next;
    }
}

if ($only) {
    for $loop (0..99) {
        $key = sprintf "%02d", $loop;
        $marked{$key} = "true";

    }
}


foreach $number (@list) {
    if ($only) {
        print "releasing $number\n";
        delete $marked{$number};
    } else {
        $marked{$key} = "true";
    }
}

my $agent    = __PACKAGE__->new;
my $request  = HTTP::Request->new(GET => $url);

my $response = $agent->request($request);
$response->is_success or die "$url: ",$response->message,"\n";

(my $filename = $url) =~ s|.*/||g;
die unless $filename;
($directory = $filename) =~ s/\..*$//; # get rid of suffix
mkdir $directory, 0755;
chdir $directory;
open (TARGET, ">index.html") or die;
binmode TARGET;

@lines = split /\n/, $response->content;

$save_files = "";
$dont_print = "";
$num = @lines;
$author = "";

foreach $_ (@lines) {
    # Comment Commands
    if (/START NOCOPY LINES/) {
        $dont_print = "true";
        next;
    }
    if (/STOP NOCOPY LINES/) {
        $dont_print = "";
        next;
    }
    if (/START SCHEDULE/) {
        $in_schedule = "true";
        next;
    }
    if (/STOP SCHEDULE/) {
        $in_schedule = "";
        next;
    }
    # Don't copy lines between START NOCOPY and STOP NOCOPY
    next if $dont_print;
    # Grab files betwen table markers
    if (m|\<\s*table|i  && $in_schedule) {
        $save_files = "true";
    }
    if (m|\<\s*/table|i) {
        $save_files = "";
    }
    
    if ($save_files) {
        if (/Talk line for (\w+)/) {
            $author = $1;
            ++$count;
            #warn "author $author\n";
        }
        if (m|\<a\s+href\s*=\s*\"([^\"]+)\"|) {
            $weblink = $1;
            ($newlink = $weblink) =~ s|.*/||g;
            # add authors name if not there
            if ($newlink !~ /$author/i) {
                $newlink = "$author\_$newlink";
            }
            if (! $newlink) {
                $newlink = "index".(++$index_number).".html";
            }
            # make sure that $newlink is unique
            $newlink = sprintf "%02d_%s", $count, $newlink;
            $newlink = unique_name($newlink);
            $webhash{$weblink} = $newlink;

            s|\<a\s+href\s*=\s*\"([^\"]+)\"|<a href=\"$newlink\"|;
        }
    }

    # clean up the html
    s|Add Links Below||;
    s|\<\s*/?form[^\>]*\>||ig;
    s|\<\s*/?input[^\>]*\>||ig;
    if ($in_schedule) {
        push @schedule, $_;
    }
    print TARGET "$_\n";
}
close TARGET;

open (SHORT, ">short.html") or die;
open (LI_ONLY, ">short_li.html") or die;

$line = "";
print LI_ONLY "<ul>\n";
foreach $_ (@schedule) {
    s|a href=\"|a href=\"$directory/|;
    print SHORT "$_\n";
    $line .= $_;
    # Do we have a whole line yet
    if ($line =~ m|</tr>|i) {
        if (($line =~ /title/i) && ($line =~ /name/i) && 
            ($line =~ /length/i) &&($line =~ /links/i)) {
            # this must be the title line
            # don't bother
            $line = "";
            next;
        }
        $line =~ s|</?td[^>]*>||g;
        $line =~ s|</?tr[^>]*>||g;
        $line =~ s|</a><a|</a>&nbsp;&nbsp;<a|g;
        $line =~ s|</b><i|</b>&nbsp;&nbsp;<i|g;
        $line =~ s|<!--[^>]*-->||g;
        print LI_ONLY "<li> $line\n";
        $line = "";
        next;
    }
}
print LI_ONLY "</ul>\n";
close (SHORT);
close (LI_ONLY);
print "\n";

# letw's try to do this in some reasonable order
foreach $link (keys %webhash) {
    $file = $webhash{$link};
    $backwards{$file} = $link;
}

foreach $file (sort keys %backwards) {
    $link = $backwards{$file};
    if ($file =~ /^(\d{2})/) {
        next if ($marked{$1});
    }
    $| = 1;
    print "\nSaving $link as $file\n";
    $| = 0;
    save_file_from_link($link, $file);
}

sub get_basic_credentials {
  my ($self, $realm, $url) = @_;
  my @list = %name_hash;
  if ($name_hash{$realm} && $passwd_hash{$realm} && ($url ne $last_url)) {
      return ($name_hash{$realm}, $passwd_hash{$realm});
  }
  $last_url = $url;
  my $old = $|;
  $| = 1;
  print "Getting $url\n";
  print "Enter username and password for realm \"$realm\".\n";
  print "username: ";
  chomp (my $name = <>);
  return unless $name;
  my $passwd;
  print "password: ";
  if ($badPassword) {
      chomp ($passwd = <>);
  } else {
      ReadMode 'noecho';
      $passwd = ReadLine 0;
      chomp $passwd;
      ReadMode 'normal';
  }
  $| = $old;
  $name_hash{$realm} = $name;
  $passwd_hash{$realm} = $passwd;
  return ($name,$passwd);
}

sub save_file_from_link {
    my $url = shift;
    my $filename = shift;

    my $agent    = __PACKAGE__->new;
    my $request  = HTTP::Request->new(GET => $url);
    
    my $response = $agent->request($request);
    local $index_number;
     if (!$response->is_success) {
        warn "$url: ",$response->message,"\n";
        return;
    }
    
    binmode TARGET;
    open (TARGET, ">$filename") or die;
    binmode TARGET;
    print TARGET $response->content;
    return $filename;
}

sub unique_name {
    my $filename = shift;
    $filename =~ s/[^\w-\.]+/_/g;
    if (! $marked{$filename}) {
        $marked{$filename} = "true";
        return $filename;
    }
    # break up file name
    my ($base, $suffix);
    if ($filename =~ /\./) {
        $base = $`;
        $suffix = $'; #';
    } else {
        $base = $filename;
        $suffix = "";
    }
    $counter = 0;
    do {
        ++$counter;
        $filename = $base.$counter.".".$suffix;        
    } while ($marked{ $filename });
    $marked{$filename} = "true";
    return $filename;
}

sub warning {
        warn "::WARNING:: 

Term::ReadKey is NOT installed on this machine.  This means
that if you want to continue, you must type your web password WITH echo
(e.g. The person sitting behind you will be able to read it).  If you want
to continue, make sure that nobody else is watching and that you 'clear' the
screan after this script is finished.  Type <ctrl>-c to exit now.\n\n"; 

}
