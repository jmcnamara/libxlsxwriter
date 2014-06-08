#/bin/bash

# Perform some minor clean-ups/fixes to the docs.

perl -i -pe "s/Related&#160;Pages/Contents/" html/*.html
perl -i -pe "s/Related Pages/Contents/"      html/*.html
perl -i -pe "s/_page/_8h/"                   html/pages.html
perl -i ../dev/release/fix_example_docs.pl   html/examples.html

