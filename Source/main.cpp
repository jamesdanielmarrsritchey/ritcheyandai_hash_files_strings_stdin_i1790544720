// Description: Provides the command-line entry point, help short-circuit, input-source setup, hash dispatch, checksum formatting, optional checksum-file writing, and script-friendly success or failure output.
#include "arguments.hpp" // Imports command-line parsing, defaults, and input-source definitions.
#include "checksum_format.hpp" // Imports conversion of hexadecimal digests to the selected output format and the canonical format enumeration.
#include "checksum_writer.hpp" // Imports the isolated destination-file writer.
#include "hash_dispatcher.hpp" // Imports selection of the built-in hashing algorithms.
#include "help_content.hpp" // Imports RCFF help-content output.
#include <fstream> // Provides binary file input for source_file.
#include <iostream> // Provides standard input, standard output, and standard error.
#include <sstream> // Provides a stream wrapper around input_string bytes.
#include <string> // Provides std::string for checksums and source labels.
int main(int var_416b589c_argument_count_num, char* var_416b589c_argument_values_arr[]) { // Starts the language-required application entry point.
    for (int var_416b589c_index_num = 1; var_416b589c_index_num < var_416b589c_argument_count_num; var_416b589c_index_num += 2) { // Scans only option-name positions so a literal input string of --help is not mistaken for the help option.
        if (std::string(var_416b589c_argument_values_arr[var_416b589c_index_num]) == "--help") { // Checks whether help was requested anywhere in the name-value argument sequence.
            bool var_416b589c_help_result_boo = fun_c74ac24b_output_help_content_i1790544731_v2(var_416b589c_argument_values_arr[0]); // Outputs only the Content value from the RCFF help content file and performs no hashing.
            if (var_416b589c_help_result_boo == false) { // Checks whether the help content file could not be found or parsed.
                std::cerr << "FALSE\n"; // Emits the project's simple machine-readable failure value to standard error.
                return 1; // Returns a nonzero process status for the failed help operation.
            } // Ends help failure handling.
            return 0; // Stops the application immediately after successful help output so normal operation never runs.
        } // Ends help-option detection.
    } // Ends the early help scan.
    ArgumentValues var_416b589c_arguments_obj; // Creates storage initialized with the project-defined algorithm, format, delimiter, and overwrite defaults.
    bool var_416b589c_parse_result_boo = fun_fb513b28_parse_arguments_i1790544721_v3(var_416b589c_argument_count_num, var_416b589c_argument_values_arr, var_416b589c_arguments_obj); // Parses command-line pairs and resolves the last enabled input source.
    if (var_416b589c_parse_result_boo == false) { // Checks whether arguments were missing, malformed, or unsupported.
        std::cerr << "FALSE\n"; // Emits the required false error result to standard error.
        return 1; // Returns a nonzero process status for invalid normal-operation arguments.
    } // Ends argument parsing failure handling.
    std::ifstream var_416b589c_file_stream_obj; // Creates a file stream that is opened only when source_file is the selected source.
    std::istringstream var_416b589c_string_stream_obj(var_416b589c_arguments_obj.var_ba1a60f9_source_value_str, std::ios::in | std::ios::binary); // Creates a binary-preserving stream over the selected string value when input_string is used.
    std::istream* var_416b589c_input_stream_obj = nullptr; // Stores a pointer to whichever stream represents the selected input source.
    std::string var_416b589c_checksum_source_label_str = "-"; // Defaults the checksum-file source field to the conventional dash marker used for non-file input.
    if (var_416b589c_arguments_obj.var_ba1a60f9_source_type_uns == InputSourceType::SourceFile) { // Checks whether the last enabled input source is source_file.
        if (var_416b589c_arguments_obj.var_ba1a60f9_source_value_str.empty() == true) { // Rejects an empty file path before attempting to open a source.
            std::cerr << "FALSE\n"; // Emits the required false error result to standard error.
            return 1; // Returns a nonzero status because an empty path cannot identify a source file.
        } // Ends empty source-file path checking.
        var_416b589c_file_stream_obj.open(var_416b589c_arguments_obj.var_ba1a60f9_source_value_str, std::ios::in | std::ios::binary); // Opens the requested source file without text transformations.
        if (var_416b589c_file_stream_obj.is_open() == false) { // Checks whether the requested source file could not be opened for reading.
            std::cerr << "FALSE\n"; // Emits the required false error result to standard error.
            return 1; // Returns a nonzero status because hashing cannot proceed without the requested file bytes.
        } // Ends source-file open validation.
        var_416b589c_input_stream_obj = &var_416b589c_file_stream_obj; // Selects the opened file stream as the bytes to hash.
        var_416b589c_checksum_source_label_str = var_416b589c_arguments_obj.var_ba1a60f9_source_value_str; // Uses the exact supplied source filename in checksum-file records so compatible checksum tools can locate the same path when hexadecimal/default-delimiter output is used.
    } else if (var_416b589c_arguments_obj.var_ba1a60f9_source_type_uns == InputSourceType::InputString) { // Checks whether the last enabled source is input_string.
        var_416b589c_input_stream_obj = &var_416b589c_string_stream_obj; // Selects the stream containing the exact command-line string bytes, including a valid empty string.
    } else if (var_416b589c_arguments_obj.var_ba1a60f9_source_type_uns == InputSourceType::StandardInput) { // Checks whether enabled stdin was the last selected source.
        var_416b589c_input_stream_obj = &std::cin; // Selects standard input so piped or redirected bytes can be hashed as they are read.
    } else { // Handles an unexpected source state defensively even though the parser already rejects missing sources.
        std::cerr << "FALSE\n"; // Emits the required false error result to standard error.
        return 1; // Returns a nonzero status because no valid input stream is available.
    } // Ends selected-input stream setup.
    std::string var_416b589c_hex_checksum_str; // Creates storage for the canonical hexadecimal checksum returned by the selected algorithm.
    bool var_416b589c_hash_result_boo = fun_f9ae9163_dispatch_hash_i1790544729_v1(*var_416b589c_input_stream_obj, var_416b589c_arguments_obj.var_ba1a60f9_algorithm_str, var_416b589c_hex_checksum_str); // Runs the requested built-in hashing implementation against the selected input stream.
    if (var_416b589c_hash_result_boo == false) { // Checks whether the algorithm was unsupported or source streaming failed.
        std::cerr << "FALSE\n"; // Emits the required false error result to standard error.
        return 1; // Returns a nonzero status for hashing failure.
    } // Ends hashing failure handling.
    std::string var_416b589c_formatted_checksum_str; // Creates binary-safe storage for the checksum representation requested by the format argument.
    ChecksumOutputFormat var_416b589c_output_format_uns = ChecksumOutputFormat::Hexadecimal; // Initializes the canonical output format before the formatter replaces it with the selected representation.
    bool var_416b589c_format_result_boo = fun_d2b6c4a1_format_checksum_i1790544732_v2(var_416b589c_hex_checksum_str, var_416b589c_arguments_obj.var_ba1a60f9_format_str, var_416b589c_formatted_checksum_str, var_416b589c_output_format_uns); // Converts the canonical digest to hexadecimal, Base64, raw binary, textual binary, or decimal representation output.
    if (var_416b589c_format_result_boo == false) { // Checks whether the requested format was unsupported or the internal digest could not be converted safely.
        std::cerr << "FALSE\n"; // Emits the required false error result to standard error.
        return 1; // Returns a nonzero status for checksum-formatting failure.
    } // Ends checksum-formatting failure handling.
    if (var_416b589c_arguments_obj.var_ba1a60f9_destination_provided_boo == true) { // Checks whether the caller also requested a checksum output file.
        bool var_416b589c_compatible_record_boo = (var_416b589c_output_format_uns == ChecksumOutputFormat::Hexadecimal) && (var_416b589c_arguments_obj.var_ba1a60f9_delimiter_str == "  "); // Enables GNU filename escaping only for the default hexadecimal/two-space record form intended for standard checksum-tool compatibility.
        bool var_416b589c_write_result_boo = fun_958a9821_write_checksum_i1790544730_v3(var_416b589c_arguments_obj.var_ba1a60f9_destination_file_str, var_416b589c_formatted_checksum_str, var_416b589c_checksum_source_label_str, var_416b589c_arguments_obj.var_ba1a60f9_delimiter_str, var_416b589c_arguments_obj.var_ba1a60f9_overwrite_boo, var_416b589c_compatible_record_boo); // Writes one checksum record using the requested delimiter and isolated overwrite-protected writer.
        if (var_416b589c_write_result_boo == false) { // Checks whether the requested destination could not be written safely and completely.
            std::cerr << "FALSE\n"; // Emits the required false error result to standard error.
            return 1; // Returns a nonzero status and intentionally avoids printing a success checksum when destination writing failed.
        } // Ends destination-write failure handling.
    } // Ends optional destination-file processing.
    if (var_416b589c_output_format_uns == ChecksumOutputFormat::Binary) { // Checks whether the caller requested raw digest bytes rather than a textual representation.
        std::cout.write(var_416b589c_formatted_checksum_str.data(), static_cast<std::streamsize>(var_416b589c_formatted_checksum_str.size())); // Writes exactly the raw digest bytes to standard output without appending a textual newline byte.
    } else { // Handles all textual output representations.
        std::cout << var_416b589c_formatted_checksum_str << '\n'; // Prints the selected textual checksum representation followed by a conventional line ending.
    } // Ends raw-versus-text standard-output handling.
    if (std::cout.good() == false) { // Checks whether standard output failed while writing either raw bytes or textual output.
        return 1; // Returns a nonzero status when the requested checksum could not be fully emitted to standard output.
    } // Ends standard-output failure checking.
    return 0; // Returns the standard successful process status after hashing, formatting, and any requested destination write complete.
} // Ends the language-required application entry point.
