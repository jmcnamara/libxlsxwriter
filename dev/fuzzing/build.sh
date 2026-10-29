cd $SRC/libxlsxwriter

printenv

mkdir -p build
cmake -S . -B build -DBUILD_FUZZERS=ON -DBUILD_TESTS=ON && cmake --build build --target install

# Build the corpus using the existing xlsx files in the source
mkdir -p corpus
find $SRC/libxlsxwriter -name "*.xlsx" -exec cp {} corpus \;
zip -q $OUT/xlsx_fuzzer_seed_corpus.zip corpus/*

# OSS-FUZZ-AGENT xlsx-corpus-dict-options: structured seeds + dictionary for
# the existing xlsx_fuzzer (corpus-only pair; no harness change)
mkdir -p seeds_xlsx
python3 dev/fuzzing/generate_xlsx_seeds.py --out seeds_xlsx
zip -qj $OUT/xlsx_fuzzer_seed_corpus.zip seeds_xlsx/*
cp dev/fuzzing/xlsx_fuzzer.dict $OUT/xlsx_fuzzer.dict
