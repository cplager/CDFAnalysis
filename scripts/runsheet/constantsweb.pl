#!/usr/bin/perl

##############################
 ##                          ##
  ##   CLEO 3 CONSTANTS WEB   ##
   ##                          ##
  ##      Code by Hom         ##
 ##                          ##
##############################

print "Content-type:text/html\n\n";
print "<html><head><title>Cleo III Constants Web</title></head>\n";
print "<body>";
print "<h2>Cleo III Constants Web</h2>";
print "<form action=\"http://www.lns.cornell.edu/rbin/ccon3/Cleo3ConstantsWeb\" method=GET>";
print "<!-- <input name=\"type\" type=\"hidden\" value=\"MetaVersion\">";
print " <b>Type:</b><code>MetaVersion</code>-->";
print "</select>";

print "<select name=\"type\">";
print "<option value =\"MetaVersion\">MetaVersion";
open(READ, "BDLTypes.txt");
while (<READ>){
    @type = split " ", $_;
    print "<option value=\"$type[0]\">$type[0]\n";
}
#close (READ)
print "</select><br><p>";
print "<b>Version:</b> <input name=\"version\" size=10 maxlength=10>";
print "<p>";
print "<b>Display data </b> <input type=\"checkbox\" name=displayData value=\"Yes\" checked><br>";
print "<p><input type=\"submit\" value=\"Get Constants\">";
#print "</form>";

print "</form>";
print "<hr><h2>Cleo III Version List</h2><p>\n";
print "<form action=\"http://www.lns.cornell.edu/rbin/ccon3/Cleo3VersionWeb.pl\" method=GET>";
print "<br>";

print " <b>Constants type:</b><t>";
print "<select name=\"type\">";
print "<option value =\"MetaVersion\">MetaVersion";
open(READ, "BDLTypes.txt");
while (<READ>){
    @type = split " ", $_;
    print "<option value=\"$type[0]\">$type[0]\n";
}
#close (READ)
print "</select><br><n><p>";

print "<input type=\"submit\" value=\"Get Version\">";
print "</form>";
print "</body></html>\n";








