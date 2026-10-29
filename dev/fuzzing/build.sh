cd $SRC/libxlsxwriter

printenv

mkdir -p build
cmake -S . -B build -DBUILD_FUZZERS=ON -DBUILD_TESTS=ON && cmake --build build --target install

# Build the corpus using the existing xlsx files in the source
mkdir -p corpus
find $SRC/libxlsxwriter -name "*.xlsx" -exec cp {} corpus \;
zip -q $OUT/xlsx_fuzzer_seed_corpus.zip corpus/*

# OSS-FUZZ-AGENT worksheet-api-fuzzer: structured seeds + dictionary
mkdir -p seeds_worksheet_api
python3 dev/fuzzing/generate_worksheet_api_seeds.py --out seeds_worksheet_api
zip -qj $OUT/worksheet_api_fuzzer_seed_corpus.zip seeds_worksheet_api/*
cp dev/fuzzing/worksheet_api_fuzzer.dict $OUT/worksheet_api_fuzzer.dict
