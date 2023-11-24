cd $SRC/libxlsxwriter

printenv

mkdir -p build
cmake -S . -B build -DBUILD_FUZZERS=ON && cmake --build build --target install

# Build the corpus using the existing xlsx files in the source
mkdir -p corpus
find $SRC/libxlsxwriter -name "*.xlsx" -exec cp {} corpus \;
zip -q $OUT/xlsx_fuzzer_seed_corpus.zip corpus/*
