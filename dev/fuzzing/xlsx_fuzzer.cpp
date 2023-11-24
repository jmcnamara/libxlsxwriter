#include <cstdint>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>

#include "xlsxwriter.h"

const std::string mem_dir{"/dev/shm"};
const std::string file_template = "/fuzzXXXXXX";
char temp_file_dir[FILENAME_MAX] = {0};

/**
 * \brief: Performs all prep-work needed for continuous fuzzing
 * \return: Whether initialization was successful
 */
int init_for_fuzzing()
{
    // Initialize the temporary file directory, based off what is available on the system

    if (0 == access(mem_dir.c_str(), W_OK | R_OK))
    {
        // We can read and write to the in-memory directory
        memcpy(temp_file_dir, mem_dir.c_str(), strnlen(mem_dir.c_str(), FILENAME_MAX));
    }
    else
    {
        // Default to a temporary directory
        const char* tmp_prefix = getenv("TMPDIR");
        if (nullptr == tmp_prefix)
        {
            tmp_prefix = "/tmp";
        }
        memcpy((void*) temp_file_dir, tmp_prefix, strnlen(tmp_prefix, FILENAME_MAX));
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, const size_t size)
{
    static bool init_fuzzing = init_for_fuzzing();

    char fuzz_file[FILENAME_MAX + 1] = {0};
    int fuzz_fd = 0;
    int ret = -1;
    ssize_t wc = 0;
    size_t byte_len;
    lxw_workbook *workbook = nullptr;
    lxw_worksheet *worksheet = nullptr;
    FuzzedDataProvider fdp{data, size};
    std::vector<std::uint8_t> file_bytes{};

    strncpy(fuzz_file, temp_file_dir, strlen(temp_file_dir));
    strncat(fuzz_file, file_template.c_str(), file_template.length());

    if ((fuzz_fd = mkstemp(fuzz_file)) < 0)
    {
        goto fail;
    }

    byte_len = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
    file_bytes = fdp.ConsumeBytes<uint8_t>(byte_len);
    write(fuzz_fd, file_bytes.data(), std::min(file_bytes.size(), byte_len));

    workbook = workbook_new(fuzz_file);
    worksheet = workbook_add_worksheet(workbook, nullptr);

    for (int row = 0; row < fdp.ConsumeIntegralInRange(0, 25); ++row)
    {
        for (int col = 0; col < fdp.ConsumeIntegralInRange(0, 25); ++col)
        {
            worksheet_write_string(worksheet, row, col, fdp.ConsumeRandomLengthString().c_str(), nullptr);
        }
    }

    ret = 0;

    fail:
    if (nullptr != workbook)
    {
        workbook_close(workbook);
    }
    close(fuzz_fd);
    unlink(fuzz_file);
    return ret;
}
