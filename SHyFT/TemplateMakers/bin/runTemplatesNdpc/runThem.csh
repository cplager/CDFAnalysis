echo "Starting jobs"
shyft_pretag shyftPretag_data_38X.py >&! dataRun.log
shyft_pretag shyftPretag_dataNonIsoKIT_38X.py >&! qcdRun.log
shyft_pretag shyftPretag_tchan_38X.py >&! tchanRun.log
shyft_pretag shyftPretag_ttbar_38X.py >&! ttbarRun.log
shyft_pretag shyftPretag_tWchan_38X.py >&! tWRun.log
shyft_pretag shyftPretag_wjets_38X.py >&! wjetsRun.log
shyft_pretag shyftPretag_zjets_38X.py >&! zjetsRun.log
echo "done "
