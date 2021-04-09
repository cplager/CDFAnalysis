#!/usr/bin/perl

require "config.pl";

#########################################################
# This script rebuilds the archive index.  In general   #
# the index is maintained by archiveOldFiles.pl but if  #
# it should need regenerating, this is the script  to   #
# do it.                                                #
#########################################################

@files = split /\n/, `find $archive_dir`;

foreach $file (@files){

  if( $file =~ /\/(\d{6})\-(\d\d)-(\d\d)-(\d{4})\.notes/ ){

    $run = $1;
    $date = $4.$2.$3;
    $file =~ s/\.notes$//;

    push @filesToIndex, { file => $file,
			  run => $run,
			  date => $date };
  }
}

open( INDEX, ">$archive_index" );

foreach $file (@filesToIndex){

  print INDEX $file->{ run }."\t".
    $file->{ date }."\t".
    $file->{ file }."\n";

}

close INDEX;

exit;
