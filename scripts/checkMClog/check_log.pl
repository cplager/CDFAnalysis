#!/usr/bin/env perl
#
# Usage:
# check_champNt_log.pl <log summary file> <log file> [<log file> [...]]
#
# Wild-cards can be used for the log file specification.
# The summary file is assumed to live in $summary_dir. The input
# log files are assumed to live in $logfile_dir.
# 
#
# 
use strict;
use warnings;

#use IO::File;


my @known_errors;
my @known_errors_truncated;

##################
#  Main program
#
my $release = "6.1.4";
my $summary_dir;
if ( $ENV{"CHECK_SUMMARY_DIR"} ) {
   $summary_dir = $ENV{"CHECK_SUMMARY_DIR"};
} else {
   $summary_dir = "/data/ncdf194/a/rs/results/$release/log";
}
my $logfile_dir;
if ( $ENV{"CHECK_LOGFILE_DIR"} ) {
   $logfile_dir = $ENV{"CHECK_LOGFILE_DIR"};
} else {
   $logfile_dir = "/data/ncdf194/a/rs/results/$release/log";
}

# Check arguments
#
if ( $#ARGV == 0 && $ARGV[0] eq "-h" ) {
   usage_message();
   exit(0);
} elsif ( $#ARGV < 1 ) {
   print "check_champNt_log.pl error:  wrong number of arguments!\n\n";
   usage_message();
   exit(1);
} 

# Open list of known error messages
#
open KNOWN_ERROR_FILE, $ENV{"KNOWN_ERROR_FILE"} 
  or die "Cannot open known error file:  $!";

print "Loading known errors:\n";

while ( <KNOWN_ERROR_FILE> ) {
   push @known_errors, $_;
   $known_errors[$#known_errors] =~ s/(.*\S)(\s*$)/$1/;
   print "Known error: \"$known_errors[$#known_errors]\"\n";
   
   my $error = $known_errors[$#known_errors];
   $error =~ /(%ERLOG-\w\s)(.*)/;
   $error = $2;
   #print "1 = \"$1\" 2 = \"$2\"  error = \"$error\"\n";

   push @known_errors_truncated, ( sprintf "%20.20s", $error );
   #print "Pre trim:  \"$known_errors_truncated[$#known_errors_truncated]\"\n";
   $known_errors_truncated[$#known_errors_truncated]
     =~ s/(^\s*)(.*)(:\s*)(.*)/$2/;
   # $known_errors_truncated[$#known_errors_truncated]
   #     			=~ s/(^\s*)(\S)(.*)(\S|:)(\s*$)/\2\3\4/;

   # print "1 = \"$1\" 2 = \"$2\" 3 = \"$3\" 4 = \"$4\" \n";
   $known_errors_truncated[$#known_errors_truncated]
     =~ s/(.*)(:$)/$1/;
   #     print "Truncated: \"$known_errors_truncated[$#known_errors_truncated]\"\n";
}

# Open list of required error messages
#
# open REQUIRED_ERROR_FILE, $ENV{"REQUIRED_ERROR_FILE"}
#      or die "Cannot open requird error file:  $!";

my @logfile_list;
my @bad_logfile_list;

my @file_ops_ok_list;
my @error_messages_ok_list;
my @abort_detected_list;
my @evloop_end_found_list;
my @n_files_opened_list;
my @n_files_closed_list;
my @all_bad_file_list;
my @all_bad_error_messages_list;
my @all_bad_error_summary_messages_list;

my $all_file_ops_ok = 1;
my $all_error_messages_ok = 1;
my $all_abort_detected = 0;
my $all_evloop_end_found = 1;

my $all_n_files_opened = 0;
my $all_n_files_closed = 0;
 
my @error_summary_found_list;
my $all_error_summary_found = 1;
my $all_error_summary_ok = 1;

my @logfile_ok_list;
my $all_logfile_ok = 1;


my $logfile = new IO::File;

# Expand input log file list
#
my $ifile;
foreach $ifile ( 1 .. $#ARGV ) {
   push @logfile_list, glob( $logfile_dir . "/" . $ARGV[$ifile] );
}

# Loop over specified log files
#
my $file;
foreach $file ( @logfile_list ) {
   $logfile->open( $file, "r" ) or die "Cannot open '$file': $! ";
   print "Processing file '$file' \n";

   # Search event loop for clean file operations
   #
   my $file_ops_ok;
   my $evloop_error_messages_ok;
   my $n_files_opened;
   my $n_files_closed;
   my $evloop_abort_detected;
   my $evloop_end_found;
   my $bad_file_list;
   my $bad_error_messages_list;
 
   ($file_ops_ok, 
    $evloop_error_messages_ok, 
    $n_files_opened,
    $n_files_closed,
    $evloop_abort_detected,
    $evloop_end_found,
    $bad_file_list,
    $bad_error_messages_list)     = check_event_loop( $logfile );

   push @file_ops_ok_list, $file_ops_ok;
   #    push @error_messages_ok_list, $error_messages_ok;
   push @abort_detected_list, $evloop_abort_detected;
   push @evloop_end_found_list, $evloop_end_found;
   push @n_files_opened_list, $n_files_opened;
   push @n_files_closed_list, $n_files_closed;
   push @all_bad_file_list, @$bad_file_list;
   push @all_bad_error_messages_list, @$bad_error_messages_list;
 
   $all_file_ops_ok = ( $file_ops_ok == 0 ? 0 : $all_file_ops_ok );
   $all_error_messages_ok = ( $evloop_error_messages_ok == 0 
                              ? 0 : $all_error_messages_ok );
   $all_abort_detected = ( $evloop_abort_detected == 1
                           ? 1 : $all_abort_detected );
   $all_evloop_end_found = ( $evloop_end_found == 0 
                             ? 0 : $all_evloop_end_found );			   
 
   $all_n_files_opened += $n_files_opened;
   $all_n_files_closed += $n_files_closed;
     
   # Check error logger summary at end of log file
   #
   my $error_summary_ok;
   my $es_bad_error_messages_list;
 
   ( $error_summary_ok,
     $es_bad_error_messages_list ) = check_error_summary( $logfile );
 
   my $error_summary_found = ( $error_summary_ok < 0 ? 0 : 1 );
   push @error_summary_found_list, $error_summary_found;
 
   $all_error_summary_found = ( $error_summary_found 
                                ? $all_error_summary_found : 0 );
   $all_error_summary_ok = ( $error_summary_ok <= 0 
                             ? 0 : $all_error_summary_ok );
   if ( $error_summary_found ) {
      push @all_bad_error_summary_messages_list, @$es_bad_error_messages_list; 
   }
   # Global decision for this file
   #
   my $pass_fail;
   if ( 
       $file_ops_ok
       && 
       $evloop_error_messages_ok
       &&
       $evloop_abort_detected == 0   
       &&
       $evloop_end_found
       &&
       $error_summary_found
       && 
       $error_summary_ok
      ) {
      $pass_fail = 1;
   } else {
      $pass_fail = 0;
      print 
        "    Error detected:\n".
        "      File operations:  " . ok_fail( $file_ops_ok ) . "\n" .
        "      Event loop Error messages:  " . ok_fail( $evloop_error_messages_ok ) . "\n" .
        "      Event loop Abort detected:  " . ( $evloop_abort_detected==0 ? "No" : "$evloop_abort_detected" ) . "\n".
        "      Event loop end found:  " . ok_fail( $evloop_end_found ) . "\n" .
        "      Error summary found:  " . ok_fail( $error_summary_found )."\n".
        "      Error summary:  " . ok_fail( $error_summary_ok )."\n".
        "\n";
 	
   }
 
   push @logfile_ok_list, $pass_fail;
   $all_logfile_ok = ( $pass_fail ? $all_logfile_ok : 0 );
 
   if ( $pass_fail == 0 ) {
      push @bad_logfile_list, $file;
   }
 

}

# Print summary
#
#   my $sum_file_name = $ARGV[$#ARGV];
#   $sum_file_name =~ /(.*)(_\d*\.log)/;
#   $sum_file_name = $1;
#   $sum_file_name = ">" . $sum_file_name . ".log_summary.txt";
my $sum_file_name = ">" . $summary_dir . "/" . $ARGV[0];
# print "Summary file name = $sum_file_name\n";

open SUMMARY_FILE, $sum_file_name or die "Cannot open summary file\n";

printf SUMMARY_FILE 
  "================================================================\n". 
  "Logfile scan summary\n".
  "\n".
  "    Global result\n".
  "      Project status:  ". ( $all_logfile_ok ? "PASS\n" : "FAIL\n" ).
  "      N files opened     = $all_n_files_opened\n".
  "      N files closed     = $all_n_files_closed\n".
  "\n";

if ( !$all_logfile_ok ) {
   printf SUMMARY_FILE
     "    Error detected:\n".
     "      File operations:  " . ok_fail( $all_file_ops_ok ) . "\n" .
     "      Error messages:  " . ok_fail( $all_error_messages_ok ) . "\n" .
     "      Abort detected:  " . ok_fail( !$all_abort_detected ) . "\n".
     "      Event loop end found:  " . ok_fail( $all_evloop_end_found ). "\n".
     "      Error summary found:  " . ok_fail( $all_error_summary_found )."\n".
     "      Error summary:  " . ok_fail( $all_error_summary_ok )."\n".
     "\n";
  
   printf SUMMARY_FILE
     "    Logfiles with problems:\n";
   my $filename;
   foreach $filename ( @bad_logfile_list ) {
      printf SUMMARY_FILE
        "      $filename\n";
   }
   printf SUMMARY_FILE "\n";
}

if ( !$all_error_messages_ok ) {
   printf SUMMARY_FILE
     "    Failed error messages:\n";
 
   # Remove duplicate entries
   #
   @all_bad_error_messages_list = sort @all_bad_error_messages_list;
   my $i;
   for ( $i = $#all_bad_error_messages_list ; $i > 0 ; --$i ) {
      if ( $all_bad_error_messages_list[$i] 
           eq $all_bad_error_messages_list[$i-1] ) {
         splice @all_bad_error_messages_list, $i, 1;
      }
   }

   my $message;
   foreach $message ( @all_bad_error_messages_list ) {
      $message =~ s/%(.)/%%$1/g;
      printf SUMMARY_FILE
        "      $message";
   }
 
   printf SUMMARY_FILE "\n";
}

if ( !$all_error_summary_ok ) {
   printf SUMMARY_FILE
     "    Failed error summary messages:\n";
 
   # Remove duplicate entries
   #
   @all_bad_error_summary_messages_list = 
     sort @all_bad_error_summary_messages_list;
   my $i;
   for ( $i = $#all_bad_error_summary_messages_list ; $i > 0 ; --$i ) {
      if ( $all_bad_error_summary_messages_list[$i] 
           eq $all_bad_error_summary_messages_list[$i-1] ) {
         splice @all_bad_error_summary_messages_list, $i, 1;
      }
   }
 
   my $message;
   foreach $message ( @all_bad_error_summary_messages_list ) {
      $message =~ s/%./%%/g;
      printf SUMMARY_FILE
        "      $message\n";
   }
   printf SUMMARY_FILE "\n";
}

printf SUMMARY_FILE "    Files with problems: \n";
foreach my $file ( @all_bad_file_list ) {
   printf SUMMARY_FILE "      $file\n";
}
      


sub ok_fail {
   my $test = shift;
   return ( $test ? "OK" : "FAIL" );
}  



sub check_event_loop {
   my $logfile = shift;
   my $current_file = "";
  
   # Return values
   #
   my $file_ops_ok = 1;
   my $error_messages_ok = 1;
   my $n_files_opened = 0;
   my $n_files_closed = 0;
   my $abort_detected = 0;
   my $evloop_end_found = 0;
   my @bad_file_list;
   my @bad_error_messages_list;
  
   # Search for first file open
   #
   while ( <$logfile> ) {
      my $line = $_;
    
      study $line;
    
      if ( $line =~ /everything is done/
           ||
           $line =~ /# of processed files\s*\d*\s*fits requested limit/ ) {
         $evloop_end_found = 1;
         if ( $current_file ne "" ) {
            print "Event loop error:  last file not closed\n";
            $file_ops_ok = 0;
            push @bad_file_list, $current_file;
         }
         last;
      }
    
      if ( $line =~ /abort/i ) {
         print "Severe error in event loop:  Abnormal termination detected\n";
         $file_ops_ok = 0;
         $abort_detected = 0;
         push @bad_file_list, $current_file;
         last;
      }
    
      if ( $line =~ m/^%ERLOG/ ) {
         if ( !check_known_errors( $line ) ) {
            $error_messages_ok = 0;
            push @bad_error_messages_list, $line;
            push @bad_file_list, $current_file;
            # print "Adding bad error:  $line\n".
            # "Last on list = $bad_error_messages_list[$#bad_error_messages_list]\n";
         }
      } elsif ( $line =~ /Opened input file/ ) {
         ++$n_files_opened;

         if ( $current_file ne "" ) {
            print "Event loop error:  file opened before current file closed\n";
            $file_ops_ok = 0;
            push @bad_file_list, $current_file;
         }
      	
         $current_file = $line;
         $current_file =~ /.*\/(\w*\.\w*)\s*\**$/;
         $current_file = $1;
         # print "Current file:  $current_file\n";
      } elsif ( $line =~ /Closing Input File/ ) {
         ++$n_files_closed;
         my $file_closed = $line;
         $file_closed =~ /.*\/(\w*\.\w*)\s*after/;
         $file_closed = $1;
         if ( $file_closed ne $current_file ) {
            print "Severe error:  file closed ($file_closed) != file opened ",
              "($current_file)\n";
            $file_ops_ok = 0;
            push @bad_file_list, ( $current_file, $file_closed ); 
         }
      
         $current_file = "";
      }
      
   }
  
   # Require at least one file to have been opened
   #
   $file_ops_ok = ( $n_files_opened ? $file_ops_ok : 0 );
  
   # print "bad errors before return: @bad_error_messages_list\n";

   return ( $file_ops_ok,
            $error_messages_ok,
            $n_files_opened,
            $n_files_closed,
            $abort_detected,
            $evloop_end_found,
            \@bad_file_list,
            \@bad_error_messages_list );
}



sub check_error_summary {
   my $logfile = shift;
  
   my @bad_error_messages_list;
  
   my $error_messages_ok = 1;
  
   # Find the error log summary
   #
   my $summary_found = 0;
   my $line0 = $_;
   if ( $line0 =~ /Termination Summary/ ) {
      $summary_found = 1;
   } else {
      while ( <$logfile> ) {
         my $line = $_;
         if ( $line =~ /Termination Summary/ ) {
            $summary_found = 1;
            last;
         }
      }
   }
   if ( $summary_found == 0 ) {
      print "No Termination Summary found!\n";
      return ( -1 );
   }
  
   # Skip down to error messages
   #
   <$logfile>;
   <$logfile>;
   <$logfile>;
   <$logfile>;
  
   while ( <$logfile> ) {
      my $line = $_;
    
      if ( $line =~ /^\s*$/ ) {
         last;
      }
    
      $line =~ /^\s*\d*\s(.{20})\s/;
      my $message = $1;
      $message =~ s/(.*\S)(\s*)$/$1/;
      # print "\"$message\"...";
    
      $error_messages_ok = ( check_known_errors_truncated( $message ) == 0
                             ? 0 : $error_messages_ok );
      if ( check_known_errors_truncated( $message ) == 0 ) {
         $error_messages_ok = 0;
         push @bad_error_messages_list, $message;
      }
    
      #  print ( $error_messages_ok ? "OK\n" : "FAIL\n" );
   }
  
   return ( $error_messages_ok,
            \@bad_error_messages_list );
}


sub check_known_errors {
   my $error_message = shift;
  
   my $message;
   foreach $message ( @known_errors ) {
      if ( $error_message =~ /$message/ ) {
         return 1;
      }
   }
   # print "Failed error:  \"$error_message\"\n";
  
   return 0;
}


sub check_known_errors_truncated {
   my $error_message = shift;
  
   my $message;
   foreach $message ( @known_errors_truncated ) {
      if ( $error_message =~ /$message/ ) {
         return 1;
      }
   }
   return 0;
}


sub usage_message {
  print "Usage:\n".
        "  check_champNt_log.pl <log_summary_file> <log_file> [<log_file> [...]]\n".
	"\n".
	"    where:\n".
	"      <log_summary_file> = destination of scan results\n".
	"      <log_file> = input log file(s) to be scanned. Wild-card values\n".
	"                   are accepted, but mulst be quoted with \"\\\"\n".
	"\n".
	"  check_champNt_log.pl -h\n".
	"\n".
	"    Prints this message\n".
	"\n";
}
