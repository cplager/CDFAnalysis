#
#


echo "Running regular jobs"
shyft_pretag shyftPretag_wjets.py >&! wjets100.log
shyft_pretag shyftPretag_qcd_noniso.py >&! qcd100.log
shyft_pretag shyftPretag_qcd_iso.py >&! qcd100_iso.log
shyft_pretag shyftPretag_zjets.py >&! zjet100.log
shyft_pretag shyftPretag_ttbar.py >&! ttbar100.log

echo "Running new jobs w jes"

shyft_pretag shyftPretag_ttbar_090.py >&! ttbar090.log
shyft_pretag shyftPretag_ttbar_090.py >&! ttbar110.log

shyft_pretag shyftPretag_wjets_090.py >&! wjets090.log
shyft_pretag shyftPretag_wjets_110.py >&! wjets110.log

shyft_pretag shyftPretag_qcd_noniso_090.py >&! qcd090.log
shyft_pretag shyftPretag_qcd_noniso_110.py >&! qcd110.log

shyft_pretag shyftPretag_qcd_iso_090.py >&! qcd090_iso.log
shyft_pretag shyftPretag_qcd_iso_110.py >&! qcd110_iso.log

shyft_pretag shyftPretag_zjets_090.py >&! zjet090.log
shyft_pretag shyftPretag_zjets_110.py >&! zjets110.log

shyft_pretag shyftPretag_ttbar_090.py >&! ttbar090.log
shyft_pretag shyftPretag_ttbar_110.py >&! ttbar110.log

echo "Done!"
