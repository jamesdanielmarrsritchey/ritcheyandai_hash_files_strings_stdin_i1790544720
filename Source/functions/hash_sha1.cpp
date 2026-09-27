// Description: Implements SHA-1 directly in C++ and hashes bytes from a stream without external hashing libraries.
#include "hash_sha1.hpp" // Imports the SHA-1 function declaration.
#include <array> // Provides fixed-size arrays for the chaining state and message schedule.
#include <cstddef> // Provides std::size_t and std::ptrdiff_t.
#include <cstdint> // Provides fixed-width unsigned integer types.
#include <iomanip> // Provides hexadecimal width and fill formatting.
#include <sstream> // Provides an output string stream for hexadecimal conversion.
#include <vector> // Provides a small pending-byte buffer used across stream reads.
bool fun_99bdf04b_hash_sha1_stream_i1790544724_v1(std::istream& var_99bdf04b_input_stream_obj, std::string& var_99bdf04b_checksum_str) { // Starts SHA-1 stream hashing.
    std::array<std::uint32_t, 5> var_99bdf04b_state_arr = {0x67452301U, 0xefcdab89U, 0x98badcfeU, 0x10325476U, 0xc3d2e1f0U}; // Initializes the five standard SHA-1 chaining values.
    std::uint64_t var_99bdf04b_total_bytes_num = 0U; // Counts original unpadded bytes for the final length field.
    std::vector<std::uint8_t> var_99bdf04b_pending_arr; // Stores bytes that do not yet form a complete 64-byte block.
    var_99bdf04b_pending_arr.reserve(8256U); // Reserves room for one source chunk plus an incomplete block.
    auto var_99bdf04b_process_block_obj = [&](const std::uint8_t* var_99bdf04b_block_arr) { // Defines localized processing for one 512-bit SHA-1 block.
        std::array<std::uint32_t, 80> var_99bdf04b_schedule_arr{}; // Allocates the eighty-word SHA-1 message schedule.
        for (std::size_t var_99bdf04b_word_index_num = 0U; var_99bdf04b_word_index_num < 16U; ++var_99bdf04b_word_index_num) { // Decodes the sixteen words stored directly in the input block.
            std::size_t var_99bdf04b_offset_num = var_99bdf04b_word_index_num * 4U; // Calculates the byte offset of the current 32-bit word.
            var_99bdf04b_schedule_arr[var_99bdf04b_word_index_num] = (static_cast<std::uint32_t>(var_99bdf04b_block_arr[var_99bdf04b_offset_num]) << 24U) | (static_cast<std::uint32_t>(var_99bdf04b_block_arr[var_99bdf04b_offset_num + 1U]) << 16U) | (static_cast<std::uint32_t>(var_99bdf04b_block_arr[var_99bdf04b_offset_num + 2U]) << 8U) | static_cast<std::uint32_t>(var_99bdf04b_block_arr[var_99bdf04b_offset_num + 3U]); // Decodes one big-endian message word.
        } // Ends input-word decoding.
        for (std::size_t var_99bdf04b_word_index_num = 16U; var_99bdf04b_word_index_num < 80U; ++var_99bdf04b_word_index_num) { // Expands the sixteen input words into the full eighty-word schedule.
            std::uint32_t var_99bdf04b_expanded_num = var_99bdf04b_schedule_arr[var_99bdf04b_word_index_num - 3U] ^ var_99bdf04b_schedule_arr[var_99bdf04b_word_index_num - 8U] ^ var_99bdf04b_schedule_arr[var_99bdf04b_word_index_num - 14U] ^ var_99bdf04b_schedule_arr[var_99bdf04b_word_index_num - 16U]; // XORs the four schedule words required by SHA-1.
            var_99bdf04b_schedule_arr[var_99bdf04b_word_index_num] = (var_99bdf04b_expanded_num << 1U) | (var_99bdf04b_expanded_num >> 31U); // Rotates the expanded word left by one bit.
        } // Ends message schedule expansion.
        std::uint32_t var_99bdf04b_a_num = var_99bdf04b_state_arr[0]; // Copies the first chaining value into working register A.
        std::uint32_t var_99bdf04b_b_num = var_99bdf04b_state_arr[1]; // Copies the second chaining value into working register B.
        std::uint32_t var_99bdf04b_c_num = var_99bdf04b_state_arr[2]; // Copies the third chaining value into working register C.
        std::uint32_t var_99bdf04b_d_num = var_99bdf04b_state_arr[3]; // Copies the fourth chaining value into working register D.
        std::uint32_t var_99bdf04b_e_num = var_99bdf04b_state_arr[4]; // Copies the fifth chaining value into working register E.
        for (std::uint32_t var_99bdf04b_round_index_num = 0U; var_99bdf04b_round_index_num < 80U; ++var_99bdf04b_round_index_num) { // Performs all eighty SHA-1 rounds.
            std::uint32_t var_99bdf04b_function_num = 0U; // Holds the nonlinear round function result.
            std::uint32_t var_99bdf04b_constant_num = 0U; // Holds the round-dependent additive constant.
            if (var_99bdf04b_round_index_num < 20U) { // Selects the first SHA-1 round group.
                var_99bdf04b_function_num = (var_99bdf04b_b_num & var_99bdf04b_c_num) | ((~var_99bdf04b_b_num) & var_99bdf04b_d_num); // Applies the choose function for rounds zero through nineteen.
                var_99bdf04b_constant_num = 0x5a827999U; // Uses the first SHA-1 additive constant.
            } else if (var_99bdf04b_round_index_num < 40U) { // Selects the second SHA-1 round group.
                var_99bdf04b_function_num = var_99bdf04b_b_num ^ var_99bdf04b_c_num ^ var_99bdf04b_d_num; // Applies parity for rounds twenty through thirty-nine.
                var_99bdf04b_constant_num = 0x6ed9eba1U; // Uses the second SHA-1 additive constant.
            } else if (var_99bdf04b_round_index_num < 60U) { // Selects the third SHA-1 round group.
                var_99bdf04b_function_num = (var_99bdf04b_b_num & var_99bdf04b_c_num) | (var_99bdf04b_b_num & var_99bdf04b_d_num) | (var_99bdf04b_c_num & var_99bdf04b_d_num); // Applies the majority function for rounds forty through fifty-nine.
                var_99bdf04b_constant_num = 0x8f1bbcdcU; // Uses the third SHA-1 additive constant.
            } else { // Selects the fourth SHA-1 round group.
                var_99bdf04b_function_num = var_99bdf04b_b_num ^ var_99bdf04b_c_num ^ var_99bdf04b_d_num; // Applies parity for rounds sixty through seventy-nine.
                var_99bdf04b_constant_num = 0xca62c1d6U; // Uses the fourth SHA-1 additive constant.
            } // Ends round-group selection.
            std::uint32_t var_99bdf04b_rotated_a_num = (var_99bdf04b_a_num << 5U) | (var_99bdf04b_a_num >> 27U); // Rotates A left by five bits as required by each round.
            std::uint32_t var_99bdf04b_temporary_num = var_99bdf04b_rotated_a_num + var_99bdf04b_function_num + var_99bdf04b_e_num + var_99bdf04b_constant_num + var_99bdf04b_schedule_arr[var_99bdf04b_round_index_num]; // Computes the new working value modulo 2^32.
            var_99bdf04b_e_num = var_99bdf04b_d_num; // Rotates D into E.
            var_99bdf04b_d_num = var_99bdf04b_c_num; // Rotates C into D.
            var_99bdf04b_c_num = (var_99bdf04b_b_num << 30U) | (var_99bdf04b_b_num >> 2U); // Rotates B left by thirty bits and stores the result in C.
            var_99bdf04b_b_num = var_99bdf04b_a_num; // Rotates A into B.
            var_99bdf04b_a_num = var_99bdf04b_temporary_num; // Stores the newly computed value in A.
        } // Ends the eighty SHA-1 rounds.
        var_99bdf04b_state_arr[0] += var_99bdf04b_a_num; // Adds working register A back into the first chaining value.
        var_99bdf04b_state_arr[1] += var_99bdf04b_b_num; // Adds working register B back into the second chaining value.
        var_99bdf04b_state_arr[2] += var_99bdf04b_c_num; // Adds working register C back into the third chaining value.
        var_99bdf04b_state_arr[3] += var_99bdf04b_d_num; // Adds working register D back into the fourth chaining value.
        var_99bdf04b_state_arr[4] += var_99bdf04b_e_num; // Adds working register E back into the fifth chaining value.
    }; // Ends localized SHA-1 block processing.
    char var_99bdf04b_buffer_arr[8192]; // Allocates a fixed-size source read buffer.
    while (true) { // Repeats source reads until explicit end-of-file or an I/O error is detected.
        var_99bdf04b_input_stream_obj.read(var_99bdf04b_buffer_arr, sizeof(var_99bdf04b_buffer_arr)); // Attempts to read the next fixed-size source chunk without relying on implicit stream truthiness.
        std::streamsize var_99bdf04b_bytes_read_num = var_99bdf04b_input_stream_obj.gcount(); // Records how many bytes were actually read into the current source chunk.
        if (var_99bdf04b_bytes_read_num <= 0) { // Checks whether the read produced no bytes that need hashing.
            if (var_99bdf04b_input_stream_obj.eof() == true) { // Checks explicitly for normal end-of-file.
                break; // Stops source streaming after all bytes have been processed.
            } // Ends explicit end-of-file handling.
            if ((var_99bdf04b_input_stream_obj.bad() == true) || (var_99bdf04b_input_stream_obj.fail() == true)) { // Checks explicitly for a non-EOF stream failure.
                return false; // Reports failure rather than hashing incomplete source data.
            } // Ends explicit stream-failure handling.
            continue; // Retries only if the stream reported neither bytes, EOF, nor a failure state.
        } // Ends no-bytes handling.
        var_99bdf04b_total_bytes_num += static_cast<std::uint64_t>(var_99bdf04b_bytes_read_num); // Adds the current chunk to the original message length.
        for (std::streamsize var_99bdf04b_copy_index_num = 0; var_99bdf04b_copy_index_num < var_99bdf04b_bytes_read_num; ++var_99bdf04b_copy_index_num) { // Copies the chunk into the pending buffer.
            var_99bdf04b_pending_arr.push_back(static_cast<std::uint8_t>(var_99bdf04b_buffer_arr[var_99bdf04b_copy_index_num])); // Preserves each source byte exactly.
        } // Ends current-chunk copying.
        std::size_t var_99bdf04b_processed_bytes_num = 0U; // Tracks complete bytes transformed from the pending buffer.
        while ((var_99bdf04b_pending_arr.size() - var_99bdf04b_processed_bytes_num) >= 64U) { // Processes every complete 64-byte SHA-1 block.
            var_99bdf04b_process_block_obj(var_99bdf04b_pending_arr.data() + var_99bdf04b_processed_bytes_num); // Transforms the next complete block.
            var_99bdf04b_processed_bytes_num += 64U; // Advances past the processed block.
        } // Ends current complete-block processing.
        if (var_99bdf04b_processed_bytes_num > 0U) { // Checks whether the pending buffer contains processed bytes that can be discarded.
            var_99bdf04b_pending_arr.erase(var_99bdf04b_pending_arr.begin(), var_99bdf04b_pending_arr.begin() + static_cast<std::ptrdiff_t>(var_99bdf04b_processed_bytes_num)); // Retains only the partial tail for the next source read.
        } // Ends pending-buffer compaction.
    } // Ends source streaming.
    if (var_99bdf04b_input_stream_obj.bad() == true) { // Checks whether a genuine I/O failure interrupted the stream.
        return false; // Reports failure rather than returning a digest for incomplete data.
    } // Ends source error checking.
    std::uint64_t var_99bdf04b_total_bits_num = var_99bdf04b_total_bytes_num * 8U; // Converts the original byte length into the bit length used by SHA-1 padding.
    var_99bdf04b_pending_arr.push_back(0x80U); // Appends the one bit that starts SHA-family padding.
    while ((var_99bdf04b_pending_arr.size() % 64U) != 56U) { // Adds zeros until eight bytes remain in the final 64-byte block.
        var_99bdf04b_pending_arr.push_back(0x00U); // Appends one zero-padding byte.
    } // Ends zero padding.
    for (int var_99bdf04b_length_index_num = 7; var_99bdf04b_length_index_num >= 0; --var_99bdf04b_length_index_num) { // Appends the eight-byte original bit length in big-endian order.
        var_99bdf04b_pending_arr.push_back(static_cast<std::uint8_t>((var_99bdf04b_total_bits_num >> (8U * static_cast<std::uint32_t>(var_99bdf04b_length_index_num))) & 0xffU)); // Adds one length byte from most significant to least significant.
    } // Ends length-field construction.
    for (std::size_t var_99bdf04b_final_offset_num = 0U; var_99bdf04b_final_offset_num < var_99bdf04b_pending_arr.size(); var_99bdf04b_final_offset_num += 64U) { // Processes all final padded blocks.
        var_99bdf04b_process_block_obj(var_99bdf04b_pending_arr.data() + var_99bdf04b_final_offset_num); // Transforms one final 64-byte block.
    } // Ends final padded block processing.
    std::ostringstream var_99bdf04b_hex_stream_obj; // Creates a stream for lowercase hexadecimal output.
    var_99bdf04b_hex_stream_obj << std::hex << std::nouppercase << std::setfill('0'); // Configures stable lowercase, zero-padded hexadecimal formatting.
    for (std::size_t var_99bdf04b_state_index_num = 0U; var_99bdf04b_state_index_num < var_99bdf04b_state_arr.size(); ++var_99bdf04b_state_index_num) { // Writes each 32-bit chaining value in standard big-endian display order.
        var_99bdf04b_hex_stream_obj << std::setw(8) << var_99bdf04b_state_arr[var_99bdf04b_state_index_num]; // Writes exactly eight hexadecimal characters for the current state word.
    } // Ends digest serialization.
    var_99bdf04b_checksum_str = var_99bdf04b_hex_stream_obj.str(); // Returns the final 160-bit SHA-1 digest as hexadecimal text.
    return true; // Reports successful SHA-1 hashing.
} // Ends SHA-1 stream hashing.
