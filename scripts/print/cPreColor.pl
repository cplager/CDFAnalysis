#!/usr/bin/env perl

use lib '/home/cplager/bin/print';
use Coloring;


###################
## C++ Key Words ##
###################
@preprocessor = ("#if defined", "#if !defined", "#elif defined",
                 "#include", "#define", "#undef", "#line",
                 "#error", "#pragma", "#if", "#ifdef",
                 "#ifndef", "#elif", "#else", "#endif" );

@keywords = qw(and asm auto bitand bitor bool break case catch char
class compl const const_cast continue default delete do double
dynamic_cast else enum explicit export extern false float for friend
goto if inline int long mutable namespace new not operator or private
protected public register reinterpret_cast return short signed sizeof
static static_cast struct switch template this throw true try typedef
typeid typename union unsigned using virtual void volatile wchar_t
while NULL Char_t Uchar_t Short_t UShort_t Int_t Uint_t Long_t Ulong_t
Float_t Double_t Bool_t);

##  @keywords = ( "and", "and_eq", "asm", "auto", "bitand", "bitor",
##               "bool", "break", "case", "catch", "char", "class",
##               "compl", "const", "const_cast", "continue", "default",
##               "delete", "do", "double", "dynamic_cast", "else", "enum",
##               "explicit", "export", "extern", "false", "float", "for",
##               "friend", "goto", "if", "inline", "int", "long",
##               "mutable", "namespace", "new", "not", "not_eq",
##               "operator", "or", "or_eq", "private", "protected",
##               "public", "register", "reinterpret_cast", "return",
##               "short", "signed", "sizeof", "static", "static_cast",
##               "struct", "switch", "template", "this", "throw", "true",
##               "try", "typedef", "typeid", "typename", "union",
##               "unsigned", "using", "virtual", "void", "volatile",
##               "wchar_t", "while", "xor", "xor_eq", "_asm", "NULL" );
##  

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

    ########################
    ## Multiline Comments ##
    ########################
    # Are we in the middle of a multi-line comment
    if ($multi) {
        # Does this comment end here?
        if (! ($line =~ m|\*/|)) {
            # this line is just a continuation of a multi-line comment.
            return surround($line, $Coloring::comment);
        } else {
            # The comment ends here...
            $line =~ s|^([^\*/]*\*/)|surround($1, $Coloring::comment)|eg;
            $multi = undef;
        }
    }

    # Are we starting a multi-line comment?
    if ($line =~ m|(/\*[^\*/]*)$|) {
        $multi = "true";
        $line =~ s|(/\*[^\*/]*$)|surround($1, $Coloring::comment)|eg;
    }

    #############
    ## Strings ##
    #############
    my $temp = "\x10temp\x10";
    # replace the '\"' and '\'' with a temporary string
    $line =~ s|\\\"|1$temp|g;
    $line =~ s|\\\'|2$temp|g;
    # color strings
    $line =~ s|(\".*?\")|surround($1, $Coloring::string)|eg;
    $line =~ s|(\'.*?\')|surround($1, $Coloring::string)|eg;
    $line =~ s|(<[^\&\|\;]+>)|surround($1, $Coloring::string)|eg;
    # put the '\"' back in (colored)
    $line =~ s|1$temp|\\\"|g;
    $line =~ s|2$temp|\\\'|g;

    #######################
    ## One line comments ##
    #######################
    $line =~ s|(//.*$)|surround($1, $Coloring::comment)|eg;
    $line =~ s|(/\*[^\*/]*\*/)|surround($1, $Coloring::comment)|eg;

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
