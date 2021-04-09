################################
# File: config.pl
# Author: mdj
# Revision history:
# 
#    v1.1(04/13/2000): Edited to switch to ASCII databases
#    v1.0(11/25/1999): File created
#
# Description:
#  Stores useful variable definitions for use in programs.
# Input:
#  N/A.
# Output:
#  N/A.
# Effects:
#  Defines variables.
#
################################

# configuration
$record_sep = "\x01";
$field_sep = "\x02";
$conditions_file = 'conditions';
$data_dir = "/nfs/cleoonl/web/cleo3elog/data";
$archive_dir = $data_dir."/archive";
$archive_index = $data_dir."/archive_index.txt";
$notes_ext = 'notes';
$db_ext = 'dat';
$current_name = 'current';
@extens = ($notes_ext, $db_ext);
@month_names = ('January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December');
$cal_prog = 'cal';

$http_url = "http://lnson3.lns.cornell.edu";
$prog_url = "/rbin/runsheet";
$home_url = "http://lnson3.lns.cornell.edu/cleo3elog/";
$view_day_url = "$prog_url/displayDay.pl";
$view_current_url = "$prog_url/currentRun.pl";
$view_month_url = "$prog_url/displayMonth.pl";
$view_run_url = "$prog_url/displayRun.pl";
$view_pass1_url = "$prog_url/displayPass1.pl";
$view_offlinePass1List_url = "$prog_url/listOfflinePass1.pl";
$data_actionUrl = "$prog_url/addData.pl";
$trip_actionUrl = "$prog_url/addTrip.pl";
$comment_actionUrl = "$prog_url/addComment.pl";
$endRun_actionUrl = "$prog_url/doEndRun.pl";
$pass1_dirname = "pass1";
$pass1_dir = "/nfs/cleoonl/httpd/rbin/runsheet/$pass1_dirname";
$pass1_url = "http://www.lns.cornell.edu/~cleo31/private/Pass1Plots";
$offlinePass1_url = "http://www.lns.cornell.edu/~cleo31/private/OfflinePass1Plots";
$offlinePass1_dirname = "/nfs/cleoonl/httpd/rbin/runsheet/offlinePass1";
@offlineDirs = qw (run Pass1Histos);

# Magic luminosity that shows up for RandomLow runs or other strange cases
$kMagicLumi = 1.1767e8;


######################################
##        Expert Mailing List       ##
## Remember to use '\@' and not '@' ##
## when adding a full email address ##
######################################

@expert_groups = qw (runmgmt muon_expt cc_expt rich_expt dr_expt si_expt 
		     daq_expt  trigger_expt hv_expt gas_expt cooling_expt 
                     energy_expt drca_expt kreinick_expt scan_expt 
                     calibration shifters_expt);

@runmgmt = qw (swg idanko mrs43 selina);
@muon_expt = qw (swg swnam savinov tom);
@cc_expt = qw (selina dtgong tom bkh);
@rich_expt = qw (erdene artuso\@suhep.phy.syr.edu);
@dr_expt = qw (dpp tom kme mrs43);
@si_expt = qw (mlw33 victor wilksen idanko);
@daq_expt = qw (hubert wilksen idanko);
@trigger_expt = qw (trigger3 idanko);
@hv_expt = qw (jgt idanko);
@gas_expt = qw (bkh dww3 cstepan ksp\@lns62.lns.cornell.edu mlw33 smith\@hep.umn.edu erdene);
@cooling_expt = qw (awarburt ksp\@lns62.lns.cornell.edu);
@energy_expt = qw (mrs43);
@drca_expt = qw (hennessy);
@kreinick_expt = qw (dlk idanko);
@scan_expt = qw (mccann+cleo.scan\@cyrus.watson.org idanko);
@calibration = qw (swnam hs tom bkh selina dtgong erdene hennessy bsa kme mlw33 victor hubert mrs43 idanko);
@shifters_expt = (); # blank so it just sends mail to the shifters

1; # the last thing that a required file needs to have is a non-zero value






































