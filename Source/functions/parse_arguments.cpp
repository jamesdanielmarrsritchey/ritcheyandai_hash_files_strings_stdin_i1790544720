// Description: Parses command-line name-value pairs, applies default algorithm/format/delimiter/overwrite values, and applies last-enabled-source precedence.
#include "arguments.hpp" // Imports the argument data structure and parser declaration.
#include <string> // Imports std::string for argument comparisons and values.
bool fun_fb513b28_parse_arguments_i1790544721_v3(int var_fb513b28_argument_count_num, char* var_fb513b28_argument_values_arr[], ArgumentValues& var_fb513b28_parsed_arguments_obj) { // Starts command-line parsing.
    if (var_fb513b28_argument_count_num <= 1) { // Checks whether the command line contains any application arguments.
        return false; // Reports failure because at least one enabled input source is required even though algorithm, format, delimiter, and overwrite have defaults.
    } // Ends the no-arguments check.
    for (int var_fb513b28_index_num = 1; var_fb513b28_index_num < var_fb513b28_argument_count_num; var_fb513b28_index_num += 2) { // Walks through option positions while treating the following item as each value.
        if ((var_fb513b28_index_num + 1) >= var_fb513b28_argument_count_num) { // Checks whether the current option is missing its required value.
            return false; // Reports malformed input when a value is absent.
        } // Ends the missing-value check.
        std::string var_fb513b28_name_str = var_fb513b28_argument_values_arr[var_fb513b28_index_num]; // Copies the current option name into a string for readable comparisons.
        std::string var_fb513b28_value_str = var_fb513b28_argument_values_arr[var_fb513b28_index_num + 1]; // Copies the current option value exactly as supplied by the shell.
        if (var_fb513b28_name_str == "--algorithm") { // Checks for the optional algorithm argument.
            var_fb513b28_parsed_arguments_obj.var_ba1a60f9_algorithm_str = var_fb513b28_value_str; // Replaces the default SHA3-512 algorithm with the supplied value.
        } else if (var_fb513b28_name_str == "--format") { // Checks for the optional checksum representation argument.
            var_fb513b28_parsed_arguments_obj.var_ba1a60f9_format_str = var_fb513b28_value_str; // Replaces the default hexadecimal representation with the supplied value.
        } else if (var_fb513b28_name_str == "--delimiter") { // Checks for the optional checksum-file delimiter argument.
            var_fb513b28_parsed_arguments_obj.var_ba1a60f9_delimiter_str = var_fb513b28_value_str; // Stores the exact requested delimiter, which is ignored unless destination_file is also used.
        } else if (var_fb513b28_name_str == "--source_file") { // Checks for a file input source.
            var_fb513b28_parsed_arguments_obj.var_ba1a60f9_source_type_uns = InputSourceType::SourceFile; // Makes the file source the currently selected source.
            var_fb513b28_parsed_arguments_obj.var_ba1a60f9_source_value_str = var_fb513b28_value_str; // Stores the selected source file path.
        } else if (var_fb513b28_name_str == "--input_string") { // Checks for a command-line string input source.
            var_fb513b28_parsed_arguments_obj.var_ba1a60f9_source_type_uns = InputSourceType::InputString; // Makes the string source the currently selected source.
            var_fb513b28_parsed_arguments_obj.var_ba1a60f9_source_value_str = var_fb513b28_value_str; // Stores the exact input string, including an intentionally empty string.
        } else if (var_fb513b28_name_str == "--stdin") { // Checks for the standard-input source switch.
            if ((var_fb513b28_value_str == "true") || (var_fb513b28_value_str == "t")) { // Enables standard input for either accepted true value.
                var_fb513b28_parsed_arguments_obj.var_ba1a60f9_source_type_uns = InputSourceType::StandardInput; // Makes standard input the currently selected source.
                var_fb513b28_parsed_arguments_obj.var_ba1a60f9_source_value_str.clear(); // Clears any earlier source path or string because standard input has no stored source value.
            } // Ends accepted standard-input true-value checking so all other values are ignored.
        } else if (var_fb513b28_name_str == "--destination_file") { // Checks for the optional checksum destination path.
            var_fb513b28_parsed_arguments_obj.var_ba1a60f9_destination_provided_boo = true; // Records that a destination was explicitly requested.
            var_fb513b28_parsed_arguments_obj.var_ba1a60f9_destination_file_str = var_fb513b28_value_str; // Stores the requested destination path.
        } else if (var_fb513b28_name_str == "--overwrite") { // Checks for the optional overwrite permission switch.
            var_fb513b28_parsed_arguments_obj.var_ba1a60f9_overwrite_boo = (var_fb513b28_value_str == "true"); // Enables overwrite only when the exact value is true and otherwise preserves the false behavior.
        } else if (var_fb513b28_name_str == "--help") { // Recognizes help as a valid argument even though main handles it before normal parsing.
            continue; // Ignores the help value because help execution is handled before this function is normally called.
        } else { // Handles all unrecognized argument names.
            return false; // Reports failure for an unsupported command-line argument.
        } // Ends the option-name selection chain.
    } // Ends the name-value argument loop.
    if (var_fb513b28_parsed_arguments_obj.var_ba1a60f9_algorithm_str.empty() == true) { // Checks whether an explicitly supplied algorithm value was empty.
        return false; // Reports failure because an empty explicit algorithm must not silently revert to the default.
    } // Ends algorithm-value validation.
    if (var_fb513b28_parsed_arguments_obj.var_ba1a60f9_format_str.empty() == true) { // Checks whether an explicitly supplied format value was empty.
        return false; // Reports failure because an empty explicit format must not silently revert to the default.
    } // Ends format-value validation.
    if (var_fb513b28_parsed_arguments_obj.var_ba1a60f9_source_type_uns == InputSourceType::None) { // Checks whether any enabled input source was selected.
        return false; // Reports failure when the required input source is missing.
    } // Ends the input source requirement check.
    return true; // Reports successful argument parsing when the required input source and all supplied values are available.
} // Ends command-line parsing.
