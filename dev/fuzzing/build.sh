cd $SRC/libxlsxwriter

printenv

mkdir -p build
cmake -S . -B build -DBUILD_FUZZERS=ON -DBUILD_TESTS=ON && cmake --build build --target install

# Build the corpus using the existing xlsx files in the source
mkdir -p corpus
find $SRC/libxlsxwriter -name "*.xlsx" -exec cp {} corpus \;
zip -q $OUT/xlsx_fuzzer_seed_corpus.zip corpus/*

# OSS-FUZZ-AGENT workbook-props-fuzzer: structured seeds + dictionary
mkdir -p seeds_workbook_props
python3 dev/fuzzing/generate_workbook_props_seeds.py --out seeds_workbook_props
zip -qj $OUT/workbook_props_fuzzer_seed_corpus.zip seeds_workbook_props/*
cp dev/fuzzing/workbook_props_fuzzer.dict $OUT/workbook_props_fuzzer.dict
