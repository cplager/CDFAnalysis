#!/usr/bin/perl

require "config.pl";

##########################################################
# This script takes *.dat and *.notes files that         #
# are over one week old and files them in the archive.   #
# At the same time it maintains an index of the archive. #
##########################################################

@dataDirFiles = split /\n/, `ls -1 $data_dir`;

foreach $file (@dataDirFiles){

  if( ( $file =~ /(\d{3})(\d)(\d\d)\-(\d\d)\-(\d\d)\-(\d{4})\./ ) &&
      ( olderThanOneWeek( $file ) ) ) {
    
    my $subDir1 = $archive_dir."/".($1*1000)."/";
    my $subDir2 = $subDir1.($1*1000+$2*100)."/";

    push @filesToArchive, { source => $data_dir."/".$file,
			    subDir1 => $subDir1,
			    subDir2 => $subDir2,
			    dest => $subDir2.$file,
			    run => $1.$2.$3,
			    date => $6.$4.$5 };
  }
}

open( INDEX, ">>$archive_index" );

foreach $entry (@filesToArchive){

  if( ! -d $entry->{ subDir1 } ){

    system( "mkdir $entry->{ subDir1 }" );
  }
  if( ! -d $entry->{ subDir2 } ){

    system( "mkdir $entry->{ subDir2 }" );
  }

  system( "mv $entry->{ source } $entry->{ dest }" );

  if( $entry->{ dest } =~ /\.notes$/ ){

    $entry->{ dest } =~ s/\.notes$//;

    print INDEX $entry->{ run }."\t".
      $entry->{ date }."\t".
      $entry->{ dest }."\n";
  }
}

close INDEX;

exit;

sub olderThanOneWeek{

  my $fileName = shift;

  if( $currentDate == 0 ){

    my $thisYear = (localtime)[5];
    my $thisDayOfYear = (localtime)[7];

    $currentDate = $thisYear*365 + $thisDayOfYear;
  }

  $fileName =~ /\d{6}\-(\d\d)\-(\d\d)\-(\d{4})/;

  #no need to be super precise here -- 30.4 is the average number
  #of days in a month

  my $fileDate = ( ($3 - 1900) * 365 ) + ( ($1 - 1) * 30.4) + $2;

  return( ($currentDate - $fileDate) > 7 );
}
