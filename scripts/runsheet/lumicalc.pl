#!/usr/bin/perl

##############################
##                          ##
##  Luminosity Calculator   ##
##                          ##
##  Code by Homa.           ##
##                          ##
##############################

print "Content-type:text/html\n\n";

##This part reads from Web Interface
read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
@pairs = split(/&/, $buffer);
foreach $pair (@pairs) {
    ($name, $value) = split(/=/, $pair);
    $value =~ tr/+/ /;
    $value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
    $FORM{$name} = $value;
}

$url = "http://lnson3.lns.cornell.edu/cleo3elog/lumc.html";

print "<html><head><title>Luminosity Calculator Results</title></head>\n";
print "<body>\n";
print "<h2>Luminosity Calculator Results</h2>\n";
print "Selected Runs:  ";
print $FORM{'firstRun'};
print " through ";
print $FORM{'lastRun'};
print "<br><p><p>";
##  This part does some error checking, quite useless acutally...
if (!(exists $FORM{'runSelect'})) {
    print "You didn't select a run!<p>\n";
} 

##  Here's where the data resides.  Pop it open!
#$file = "/cdat/lnsro4/disk1/hennessy/cleo3/runmanager/runlist/runlist2.dat";
$file = "/home/cleo31/lumicalc/runlist.dat";
open (SOURCE, $file) or warn "Can't open $file\n"; 

while (<SOURCE>){
    @pp = split " ", $_;
    if((($pp[0] >= $FORM{'firstRun'}) and ($pp[0] <= $FORM{'lastRun'})) or (($FORM{'firstRun'} == '') and ($FORM{'lastRun'} == ''))){  
	if (($pp[1] =~ "Yes") and ($pp[5] =~ "Global|DataTaking")){
	    $allRuns = $allRuns + $pp[3];
	if ($pp[9] =~ "ON"){
	    $onRuns = $onRuns + $pp[3];
	    if (($pp[4] >= 0) and ($pp[4] <= 4.8)){
		$sum1sOn = $sum1sOn + $pp[3];
		$sum1s = $sum1s + $pp[3];
	    }
	    if (($pp[4] >= 4.9) and ($pp[4] <= 5.1)){
		$sum2sOn = $sum2sOn + $pp[3];
		$sum2s = $sum2s + $pp[3];
	    }
	    if (($pp[4] >= 5.14) and ($pp[4] <= 5.2)){
		$sum3sOn = $sum3sOn + $pp[3];
		$sum3s = $sum3s + $pp[3];
	    }
	    }## end check for On
	if ($pp[9] =~ "OFF"){
	    $offRuns = $offRuns + $pp[3];
	    if (($pp[4] >= 0) and ($pp[4] <= 4.8)){
		$sum1sOff = $sum1sOff + $pp[3];
		$sum1s = $sum1s + $pp[3];
	    }
	    if (($pp[4] >= 4.9) and ($pp[4] <= 5.1)){
		$sum2sOff = $sum2sOff + $pp[3];
		$sum2s = $sum2s + $pp[3];
	    }
	    if (($pp[4] >= 5.14) and ($pp[4] <= 5.2)){
		$sum3sOff = $sum3sOff + $pp[3];
		$sum3s = $sum3s + $pp[3];
	    }
	    }## end On run search
	
	if ($pp[9] =~ "SCAN"){
	    $scanRuns = $scanRuns + $pp[3];
	    if (($pp[4] >= 0) and ($pp[4] <= 4.8)){
		$sum1s = $sum1s + $pp[3];		
		$sum1sScan = $sum1sScan + $pp[3];
	    }
	    if (($pp[4] >= 4.9) and ($pp[4] <= 5.1)){
		$sum2sScan = $sum2sScan + $pp[3];
		$sum2s = $sum2s + $pp[3];
	    }
	    if (($pp[4] >= 5.14) and ($pp[4] <= 5.2)){
		$sum3sScan = $sum3sScan + $pp[3];
		$sum3s = $sum3s + $pp[3];	
	    }
}## end Scan run search
}##end run range processing
}##end check phor good data
}##end file processing	      
       

## ********* This part prints the sum of the selected runs ***************
## ********************* It is Very Ugly ************
if($FORM{'runSelect'} =~ 'a') {    
    print "Sum for All Runs: <br>\n\n";
    print $allRuns;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'b') {    
    print "Sum for 1S Runs: <br>\n\n";
    print $sum1s;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'c') {    
    print "Sum for 2S Runs: <br>\n\n";
    print $sum2s;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'd') {    
    print "Sum for 3S Runs: <br>\n\n";
    print $sum3s;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'e') {    
    print "Sum for All On Resonance Runs: <br>\n\n";
    print $onRuns;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'f') {    
    print "Sum for On Resonance 1S Runs: <br>\n\n";
    print $sum1sOn;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'g') {    
    print "Sum for On Resonance 2S Runs: <br>\n\n";
    print $sum2sOn;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'h') {    
    print "Sum for On Resonance 3S Runs: <br>\n\n";
    print $sum3sOn;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'i') {    
    print "Sum for All Off Resonance Runs: <br>\n\n";
    print $offRuns;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'j') {    
    print "Sum for All Off Resonance 1S Runs: <br>\n\n";
    print $sum1sOff;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'k') {    
    print "Sum for All Off Resonance 2S Runs: <br>\n\n";
    print $sum2sOff;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'l') {    
    print "Sum for All Off Resonance 3S Runs: <br>\n\n";
    print $sum3sOff;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'm') {    
    print "Sum for All Scan Runs: <br>\n\n";
    print $scanRuns;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'n') {    
    print "Sum for All Scan 1S Runs: <br>\n\n";
    print $sum1sScan;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'o') {    
    print "Sum for All Scan 2S Runs: <br>\n\n";
    print $sum2sScan;
} ##if ($FORM)

if($FORM{'runSelect'} =~ 'p') {    
    print "Sum for All Scan 3S Runs: <br>\n\n";
    print $sum3sScan;
} ##if ($FORM)
print "<br><br><br>Return to the <A HREF=\"$url\">Luminosity Calculator Page</A>\n\n";

print "</body></html>\n";



