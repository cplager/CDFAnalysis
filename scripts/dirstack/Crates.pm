package Crates;

$sttr = "vmet5";
$sbus = "vmet11";
$script_dir = "/home/trigger3";

INIT
{
%vmeCrates = (sttr => "vmet5",
              axtr => "vmet13",
              l1d  => "vmet20",
              lumi => "vmet20",
              trcr => "vmet20",
              ccd  => "vmet47");
%useSlots =  (sttr => 6,
              axtr => 6,
              l1d  => 16,
              lumi => 16,
              trcr => 16,
              ccd  => 6);

%scripts = (trcr => "l1d", lumi => "l1d");
@all_crates = ();
}

# END
# {
#     foreach $crate (@all_crates) {
#         $crate->close_crate();
#     }
# }
