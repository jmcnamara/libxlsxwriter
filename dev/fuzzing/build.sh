cd $SRC/libxlsxwriter

printenv

mkdir -p build
cmake -S . -B build -DBUILD_FUZZERS=ON -DBUILD_TESTS=ON && cmake --build build --target install

# Build the corpus using the existing xlsx files in the source
mkdir -p corpus
find $SRC/libxlsxwriter -name "*.xlsx" -exec cp {} corpus \;
zip -q $OUT/xlsx_fuzzer_seed_corpus.zip corpus/*

# OSS-FUZZ-AGENT chart-fuzzer: structured seeds + dictionary
mkdir -p seeds_chart
python3 dev/fuzzing/generate_chart_seeds.py --out seeds_chart
zip -qj $OUT/chart_fuzzer_seed_corpus.zip seeds_chart/*
cp dev/fuzzing/chart_fuzzer.dict $OUT/chart_fuzzer.dict
