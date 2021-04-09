#!/usr/bin/bash
tar -czvf CODeBlue.tgz --exclude \*~ --exclude \*.o --exclude \*.exe  --exclude \*.d --exclude \*.dll --exclude \*.root --exclude \*.\*ps --exclude \*.gif --exclude \*.tgz --exclude \*root.\? *

echo scp CODeBlue.tgz fcdflnx2.fnal.gov:public_html/internal/Analysis/Combination/CODeBLUE/
