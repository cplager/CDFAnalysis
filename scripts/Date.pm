#! /usr/local/bin/perl
#
# File:   Date.pm
# Author: Tom Meyer
# Date:   Mon Oct 15 20:36:55 EDT 2001
#
# Time-stamp: <Thu Jan 17 16:21:56 EST 2002 shift>
# 
# Description: 
# Perl object to provide access to a date, specified by DD, MM, and YY.
# Provides some useful functions for asking about "tomorrow" and "yesterday,"
#   for instance.
#
# Revision history:
#
# *************************************************************************

package Date;

# use Exporter;
# @ISA = qw (Exporter);
# 
# @EXPORT = qw (now);

# Override comparison operators
use overload 
    (
     '<=>' => \&compare,
     'cmp' => \&compare,
     '+'   => \&add,
     '-'   => \&subtract,
     '""'  => \&stringify,
     );

# Some lookup tables

# Names of months
@kMonthNames = 
    (
     "January",
     "February",
     "March",
     "April",
     "May",
     "June",
     "July",
     "August",
     "September",
     "October",
     "November",
     "December"
     );

@kShortMonthNames = map { substr($_, 0, 3) } @kMonthNames;

# Names of weekdays
@kWeekdayNames = 
    (
     "Sunday",
     "Monday",
     "Tuesday",
     "Wednesday",
     "Thursday",
     "Friday",
     "Saturday"
     );

# MonthYear combinations for which we've looked up number of days already
%kDaysInMonth = 
    (
#     "200101" => 31,
#     "200102" => 28,
#     "200103" => 31,
#     "200104" => 30,
#     "200105" => 31,
#     "200106" => 30,
#     "200107" => 31,
#     "200108" => 31,
#     "200109" => 30,
#     "200110" => 31,
#     "200111" => 30,
#     "200112" => 31,
#
#     "200201" => 31,
#     "200202" => 28,
#     "200203" => 31,
#     "200204" => 30,
#     "200205" => 31,
#     "200206" => 30,
#     "200207" => 31,
#     "200208" => 31,
#     "200209" => 30,
#     "200210" => 31,
#     "200211" => 30,
#     "200212" => 31,
     );

# Weekday for the first of each month
%kWeekdayFirstOfMonth =
    (
#     "200101" => 2,
#     "200102" => 5,
#     "200103" => 5,
#     "200104" => 1,
#     "200105" => 3,
#     "200106" => 6,
#     "200107" => 1,
#     "200108" => 4,
#     "200109" => 7,
#     "200110" => 2,
#     "200111" => 5,
#     "200112" => 7
     );

# ========================================================================= #
# Constructor
sub new
{
    # Peel off class instance as first input argument
    my $class = shift;

    # Member data will be maintained in a sc-called "anonymous hash"
    my $self = {};
    # Connect this hash to the object
    bless($self, $class);
    
    # Pass rest of input arguments to separate init routine
    my $line = "@_";
    if (!$self->_init(@_)) {
        warn "Date '$line' not valid\n";
        return;
    }

    return $self;
}
# ========================================================================= #

# ========================================================================= #
# a special Constructor
# returns today's date
sub today
{
    # Peel off class instance as first input argument
    my $class = shift;

    # Member data will be maintained in a sc-called "anonymous hash"
    my $self = {};
    # Connect this hash to the object
    bless($self, $class);

    my ($day, $month, $year) = (localtime)[3, 4, 5];
    $year += 1900;
    $month += 1;
    $self->{year}  = $year;
    $self->{month} = $month;
    $self->{day}   = $day;
    $self->_setDayOfWeek;
    return $self;
}
# ========================================================================= #

# ========================================================================= #
# Return time-stamp as a nice string
sub now {
    # Get current time from system
    my($sec,$min,$hour,$mday,$mon,$yr,$wday,$yday,$isdt) = localtime();

    # Fix year, month
    $mon += 1;
    $yr += 1900;

    # Use Date to format date
    my $date = Date->new(year => $yr, month => $mon, day => $mday);

    # Format time nicely
    my $time = sprintf("%02d:%02d:%02d E%1sT", $hour, $min, $sec,
                       ($isdt == 0 ? "S" : "D"));

    # Paste together
    my $str = $date->string." $time";

    return $str;
}
# ========================================================================= #

# ========================================================================= #
# ** Not to be called by outsiders **
# Initialization routine
sub _init
{
    # Peel off class instance as first input argument
    my $self = shift;
    # Handle remaining input arguments
    if (0 == @_) {
        my $today = Date->today();
        $self->{year}        = $today->year;
        $self->{month}       = $today->month;
        $self->{day}         = $today->day;
        $self->{weekday}     = $today->weekday;
    }
    # Handle different "constructors"
    elsif (1 == @_)
    {
        # Peel off whatever we got
        my $rhs = shift;
        if (ref($rhs) ne "Date") {
            $rhs =~ s/,/ /g;
        }
        # Check what it is
        if (ref($rhs) eq "Date")
        {
            # If it's a Date instance, copy over relevant members
            $self->{year}        = $rhs->year;
            $self->{month}       = $rhs->month;
            $self->{day}         = $rhs->day;
            $self->{weekday}     = $rhs->weekday;
        } 
        # YYYYMMDD      
        elsif ($rhs =~ /^\d{8}$/) {
            $self->{year} = substr $rhs, 0, 4;
            $self->{month} = substr $rhs, 4, 2;
            $self->{day} = substr $rhs, 6, 2;
        } 
        # YYMMDD      
        elsif ($rhs =~ /^\d{6}$/) {
            $self->{year} = substr $rhs, 0, 2;
            $self->{month} = substr $rhs, 2, 2;
            $self->{day} = substr $rhs, 4, 2;
        } 
        # offset from Today
        elsif ($rhs =~ /^(\-?\d{1,5})$/) {
            my $numDays = $1;
            my $day = Date->today();
            $day += $numDays;
            $self->{year}        = $day->year;
            $self->{month}       = $day->month;
            $self->{day}         = $day->day;
            $self->{weekday}     = $day->weekday;
        }
        # Try to parse this as string
        # Numbers only
        elsif ($rhs =~ m/(\d+)\D+(\d+)\D+(\d+)/) {
            $self->{month} = $1;
            $self->{day}   = $2;
            $self->{year}  = $3;
            # Check if this is a Date key since then YYYY_MM_DD
            # instead of MM_DD_YYYY
            if ($self->month >= 1000) {
                # Swap MM and YYYY
                my $month = $self->day;
                my $day   = $self->year;
                my $year  = $self->month;
                $self->{year}  = $year; 
                $self->{month} = $month;
                $self->{day}   = $day;
            }
        }
        # 12 October, 1994
        elsif ($rhs =~ m/(\d+)\W*([a-zA-Z]+)\W*(\d*)/)
        {
            $self->{month} = $2;
            $self->{day}   = $1;
            $self->{year}  = $3;
        }
        # October 12, 1994
        elsif ($rhs =~ m/([a-zA-Z]+)\W*(\d+)\D*(\d*)/)
        {
            $self->{month} = $1;
            $self->{day}   = $2;
            $self->{year}  = $3;
        }
        # month day, Numbers only
        elsif ($rhs =~ m/(\d+)\D+(\d+)/)
        {
            $self->{month} = $1;
            $self->{day}   = $2;
        }
    } # 1 argument
    # Two arguments
    elsif (2 == @_)
    {
        # If only two arguments, assume it's a list with order
        #   MONTH DAY
        $self->{month} = shift;
        $self->{day}   = shift;
    } # 3 arguments    
    # Three arguments
    elsif (3 == @_)
    {
        # If only three arguments, assume it's a list with order
        #   MONTH DAY YEAR
        $self->{month} = shift;
        $self->{day}   = shift;
        $self->{year}  = shift;
    } # 3 arguments    
    else
    {
        # Treat input list has a hash
        my %extra = @_;
        # Now the magic . . .
        @$self{keys %extra} = values %extra;
    }
    # is month numeric
    if (!$self->{month}) {
        return;
    }
    if ($self->{month} =~ /\D/) {
        # are we trying to use a 3 letter abriviation?
        foreach my $monthIndex (1..12) {
            my $short = $kShortMonthNames[$monthIndex - 1];
            if ($self->{month} =~ /$short/i) {
                # we got it
                $self->{month} = $monthIndex;
                last;
            }
        }
    }
    # if there isn't a year, use the current one
    if (!$self->year) {
        my $today = Date->today;
        $self->{year}  = $today->year;
    }
    # If any of year, month, day were not filled in, complain
    if ( !$self->year || !$self->month || !$self->day)
    {
        print "ERROR: Date::new: 'year', 'month', and 'day' ".
            "must ALL be supplied when constructing a Date!\n";
        return;
    }

    # Change year to 4 digits if necessary
    if ($self->{year} < 100)
    {
        # Assuming 21st century here
        $self->{year} += 2000;
    }

    # Check for valid year, month
    if ($self->year < 0)
    {
        print "ERROR: Date::new: 'year' must be positive!\n";
        return;
    }

    # Vaid month?
    if ( ($self->month < 1) || ($self->month > 12) )
    {
        print "month ".$self->month." year ".$self->year.
            " day ".$self->day."\n";
        print "ERROR: Date::new: 'month' must be 1..12!\n";
        return;
    }

    # Set number of days in this month
    $self->{daysInMonth} = Date::_daysInMonth($self->month, $self->year);

    # Check for valid day now
    if ( ($self->day < 1) || ($self->day > $self->daysInMonth) )
    {
        print "ERROR: Date::new: 'day' must be 1..".$self->daysInMonth."!\n";
        return;
    }

    # Set day of week if it wasn't supplied or seems strange
    if (!$self->weekday || ($self->weekday < 1) || ($self->weekday > 7) )
    {
        $self->_setDayOfWeek;
    }
    return "true";
}
# ========================================================================= #

# ========================================================================= #
# Comparison operator for Date instances
sub compare
{
    my ($lhs, $rhs) = @_;

    return ( ($lhs->{year}  <=> $rhs->{year})  ||
            ($lhs->{month} <=> $rhs->{month}) ||
            ($lhs->{day}   <=> $rhs->{day}) );
}
# ========================================================================= #

# ========================================================================= #
# stringify
sub stringify {
    my $self = shift;
    #my $retval = $self->month."/".$self->day."/".$self->year;
    #return $retval;
    return $self->monthNameFull." ".$self->day.", ". $self->year;
}
# ========================================================================= #

# ========================================================================= #
# cvsDate
sub cvsDate {
    my $self = shift;
    return $self->day."-".$self->monthName.$self->year;
}
# ========================================================================= #


# ========================================================================= #
# Addition operator for Date, integer
sub add
{
    # Peel off class instance as first input argument
    my $self = shift;

    # Number of days to add to this one
    my $value = shift;

    # Increment day count
    my $newDay = $self->copy;
    $newDay->{day} += $value;

    # Adjust weekday---easy since days of week are never skipped
    $newDay->{weekday} = ($newDay->weekday-1 + $value) % 7 + 1;;

    # But now correct for running over month boundaries

    # Correct for going forward past end of month
    while ($newDay->day > $newDay->daysInMonth)
    {
        $newDay->{day} -= $newDay->daysInMonth;
        $newDay->_incrementMonth;
    }

    # Correct for going backward past start of month
    while ($newDay->day < 1)
    {
        $newDay->_decrementMonth;
        $newDay->{day} += $newDay->daysInMonth;
    }

    return $newDay;
}
# ========================================================================= #

# ========================================================================= #
# Subtraction operator for Date, integer
sub subtract
{
    # Peel off class instance as first input argument
    my $self = shift;

    # Number of days to subtract from this one
    my $value = shift;

    return $self->add(-$value);
}
# ========================================================================= #

# ========================================================================= #
# Return date as a nice string
sub string
{
    # Peel off class instance as first input argument
    my $self = shift;
    
    my $str = sprintf("%3s %3s %02d, %04d", $self->weekdayName,
                      $self->monthName, $self->day, $self->year);

    return $str;
}
# ========================================================================= #

# ========================================================================= #
# Get year
sub year
{
    # Peel off class instance as first input argument
    my $self = shift;

    return $self->{year};
}
# ========================================================================= #

# ========================================================================= #
# Get month number
sub month
{
    # Peel off class instance as first input argument
    my $self = shift;

    return $self->{month};
}
# ========================================================================= #

# ========================================================================= #
# Get full month name
sub monthNameFull
{
    # Peel off class instance as first input argument
    my $self = shift;

    return $kMonthNames[$self->{month}-1];
}
# ========================================================================= #

# ========================================================================= #
# Get month name---short
sub monthName
{
    # Peel off class instance as first input argument
    my $self = shift;
    
    #return substr $self->monthNameFull, 0, 3;
    return $kShortMonthNames[$self->{month}-1];
}
# ========================================================================= #

# ========================================================================= #
# Get day of week, 1..7
sub weekday
{
    # Peel off class instance as first input argument
    my $self = shift;

    return $self->{weekday};
}
# ========================================================================= #

# ========================================================================= #
# Get full name of day of week
sub weekdayNameFull
{
    # Peel off class instance as first input argument
    my $self = shift;

    return $kWeekdayNames[$self->weekday-1];
}
# ========================================================================= #

# ========================================================================= #
# Get day of week---short form
sub weekdayName
{
    # Peel off class instance as first input argument
    my $self = shift;

    return substr $kWeekdayNames[$self->weekday-1], 0, 3;
}
# ========================================================================= #

# ========================================================================= #
# Get day of month
sub day
{
    # Peel off class instance as first input argument
    my $self = shift;

    return $self->{day};
}
# ========================================================================= #

# ========================================================================= #
# Generate a unique "key" for this Date
sub key
{
    # Peel off class instance as first input argument
    my $self = shift;

    return sprintf("%02d%02d%02d", $self->{year}-2000, $self->{month},
                   $self->{day});
}
# ========================================================================= #

# ========================================================================= #
# Generate a short "key" for this Date
sub short_key
{
    # Peel off class instance as first input argument
    my $self = shift;

    return sprintf("%02d%02d", $self->{year}-2000, $self->{month});
}
# ========================================================================= #

# ========================================================================= #
# prints out something for this Date
# used for debugging
sub print
{
    # Peel off class instance as first input argument
    my $self = shift;
	print $self->string."\n";
	#printf ("%s, %s %2d, %4d\n", $self->weekdayName, $self->monthName,
	#		$self->day, $self->year);
}
# ========================================================================= #

# ========================================================================= #
# Get days in month
sub daysInMonth
{
    # Peel off class instance as first input argument
    my $self = shift;

    return $self->{daysInMonth};
}
# ========================================================================= #

# ========================================================================= #
# Get "tomorrow"
sub tomorrow
{
    # Peel off class instance as first input argument
    my $self = shift;
    
    my $tmrrw = $self + 1;

    return $tmrrw;
}
# ========================================================================= #

# ========================================================================= #
# Get "yesterday"
sub yesterday
{
    # Peel off class instance as first input argument
    my $self = shift;

    my $ystrdy = $self - 1;

    return $ystrdy;
}
# ========================================================================= #

# ========================================================================= #
# Copy a Date
sub copy
{
    # Peel off class instance as first input argument
    my $self = shift;

    my $copy = Date->new(year    => $self->{year},
                         month   => $self->{month},
                         day     => $self->{day},
                         weekday => $self->{weekday});

    # Copy over all fields in case there are extra ones
    foreach my $key (keys %$self )
    {
        $copy->{$key} = $self->{$key};
    }

    return $copy;
}
# ========================================================================= #

# ========================================================================= #
# Turn a Date key into month, day, year
# ** Static **
sub decodeKey
{
    my $key = shift;
    my @parts = split("_", $key);
    my $year = $parts[0];
    my $mon  = $parts[1];
    my $day = $parts[2];

    return $year, $mon, $day;
}
# ========================================================================= #

# ========================================================================= #
# ** Static **
# Check if given key is legitimately possible
sub legalKey
{
    my $dateKey = shift;

    my ($year, $mon, $day) = Date::decodeKey($dateKey);
    
    if ( ($year !~ m|^\d+$|) || ($mon !~ m|^\d+$|) || ($day !~ m|^\d+$|) ) {
        return;
    }

    if ( ($year < 0) || ($year > 10000) ) {
        return;
    }

    if ( ($mon < 1) || ($mon > 12) ) {
        return;
    }

    if ( ($day < 1) || ($day > 31) ) {
        return;
    }

    return "true";
}
# ========================================================================= #

# ========================================================================= #
# ** Not to be called by outsiders **
# Set day of week for a Date
sub _setDayOfWeek
{
    # Peel off class instance as first input argument
    my $self = shift;

    # Look up first of month
    my $firstDay = Date::_firstOfMonth($self->month, $self->year);

    # Compute weekday for our day
    my $wday = ($firstDay-1 + $self->day-1) % 7 + 1;

    $self->{weekday} = $wday;
}
# ========================================================================= #

# ========================================================================= #
# ** Not to be called by outsiders **
# Increment month by one, handle year as well
sub _incrementMonth
{
    # Peel off class instance as first input argument
    my $self = shift;

    $self->{month} += 1;
    if ($self->month > 12)
    {
        $self->{month} = 1;
        $self->{year} += 1;
    }
    $self->{daysInMonth} = _daysInMonth($self->month, $self->year);
}
# ========================================================================= #

# ========================================================================= #
# ** Not to be called by outsiders **
# Decrement month by one, handle year as well
sub _decrementMonth
{
    # Peel off class instance as first input argument
    my $self = shift;

    $self->{month} -= 1;
    if ($self->month < 1)
    {
        $self->{month} = 12;
        $self->{year} -=  1;
    }
    $self->{daysInMonth} = _daysInMonth($self->month, $self->year);
}
# ========================================================================= #

# ========================================================================= #
# ** Not to be called by outsiders **
# "Static"
# Create key for internal has tables
sub _monthKey {
    my ($mo, $yr) = @_;

    # Construct hash key for internal lookup table
    my $moyrKey = sprintf("%04d%02d", $yr, $mo);

    return $moyrKey;
}
# ========================================================================= #

# ========================================================================= #
# ** Not to be called by outsiders **
# "Static"
# Look up number of days in a particular month
sub _daysInMonth
{
    my ($mo, $yr) = @_;

    # Construct hash key for internal lookup table
    my $moyrKey = _monthKey($mo, $yr);

    # Check if this is already in our private lookup table
    my $numDays = 0;
    # if ($kDaysInMonth{$moyrKey} > 0) #CLP 011016
    if (exists $kDaysInMonth{$moyrKey})
    {
        # Take it
        $numDays = $kDaysInMonth{$moyrKey};
    }
    else
    {
        # Compute it
        my @weekdayList = Date::_weekdaysOfMonth($mo, $yr);

        $numDays = @weekdayList - 1;
    }

    return $numDays;
}
# ========================================================================= #

# ========================================================================= #
# ** Not to be called by outsiders **
# "Static"
# Look up weekday of first of a particular month
sub _firstOfMonth
{
    my ($mo, $yr) = @_;

    # Construct hash key for internal lookup table
    my $moyrKey = _monthKey($mo, $yr);

    # Check if this is already in our private lookup table
    my $firstWeekday = 0;
    if (exists $kWeekdayFirstOfMonth{$moyrKey})
    {
        # Take it
        $firstWeekday = $kWeekdayFirstOfMonth{$moyrKey};
    }
    else
    {
        # Compute it
        my @weekdayList = Date::_weekdaysOfMonth($mo, $yr);

        $firstWeekday = $weekdayList[1];
    }

    return $firstWeekday;
}
# ========================================================================= #

# ========================================================================= #
# ** Not to be called by outsiders **
# "Static"
# Determine day of week for every day of a particular month, year
sub _weekdaysOfMonth
{
    my($mo, $yr) = @_;

    my $moyrKey = _monthKey($mo, $yr);
#    print " -- Computing weekdays for $moyrKey --\n";

    # Grab output of unix cal command
    my @cal = `cal $mo $yr`;

    # Check for errors in this call
    if ($? > 0)
    {
        print STDERR "ERROR: Date::weekdaysOfMonth: ".
            "cal command appears unhappy!\n";
        return ();
    }

    # Remove "Month Year" line
    chomp(my $mo_yr = shift @cal);

    # Remove "Weekday names" line
    chomp(my $wkdays = shift @cal);

    # Now prepare an array which returns the day-of-the-week for
    #   each day
    my @weekdays = ();
    my $wk = 0;
    my $day = 1;

    # Forget about day "0" of the month
    push @weekdays, 0;

    foreach (@cal)
    {
        chomp;

        # Separate string into digits
        my @days = split ' ', $_;
        my $dayCnt = @days;

        # Skip blank line(s)
        if ($dayCnt < 1)
        {
            next;
        }
        ++$wk;

        # Handle first day of first week
        if ($day == 1)
        {
            my $offset = 7 - $dayCnt;
            $weekdays[$day++] = $offset;

            # We've already taken care of one day
            --$dayCnt;
        }

        # Add days for the rest of the week
        for (my $i = 0; $i < $dayCnt; ++$i)
        {
            # Assign next day of week based on previous day
            $weekdays[$day++] = ($weekdays[$day-1]+1) % 7;
        }
    }

    # Change to day of week 1..7 instead of 0..6
    for (my $i = 0; $i < @weekdays; ++$i)
    {
        $weekdays[$i] += 1;
    }

    # Add these results to private lookup tables

    # Weekday of the first of the month
    $kWeekdayFirstOfMonth{$moyrKey} = $weekdays[1];

    # Number of days in the month
    $kDaysInMonth{$moyrKey} = @weekdays - 1;

    return @weekdays;
}
# ========================================================================= #

# ========================================================================= #
# Constructs and returns last day of month for a Date instance
sub endOfMonth
{
    # Peel off class instnace as first input argument
    my $self = shift;

    # Keep month and year
    my $mo = $self->month;
    my $yr = $self->year;

    # And now look up last day of the month
    my $day  = _daysInMonth($mo, $yr);

    # Construct new date
    my $lastDate = Date->new($mo, $day, $yr);

    return $lastDate;
}
# ========================================================================= #

# ========================================================================= #
# Constructs and returns first day of month for a Date instance
sub firstOfMonth
{
    # Peel off class instnace as first input argument
    my $self = shift;

    # Keep month and year
    my $mo = $self->month;
    my $yr = $self->year;

    # First of the month
    my $day  = 1;

    # Construct new date
    my $firstDate = Date->new($mo, $day, $yr);

    return $firstDate;
}
# ========================================================================= #

# ========================================================================= #
# Destructor
#sub DESTROY {
#    my $self = shift;
#    print "Destroying date $self\n";
#}
# ========================================================================= #

# All packages end with a 1
1;
