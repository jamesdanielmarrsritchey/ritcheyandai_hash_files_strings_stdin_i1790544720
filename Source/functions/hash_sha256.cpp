// Description: Implements SHA-256 directly in C++ and hashes bytes from a stream without external hashing libraries.
#include "hash_sha256.hpp" // Imports the SHA-256 function declaration.
#include <array> // Provides fixed-size arrays for state, constants, and the message schedule.
#include <cstddef> // Provides std::size_t and std::ptrdiff_t.
#include <cstdint> // Provides fixed-width unsigned integer types.
#include <iomanip> // Provides hexadecimal width and fill formatting.
#include <sstream> // Provides an output string stream for hexadecimal conversion.
#include <vector> // Provides a small pending-byte buffer used across stream reads.
bool fun_b8a95e6b_hash_sha256_stream_i1790544725_v1(std::istream& var_b8a95e6b_input_stream_obj, std::string& var_b8a95e6b_checksum_str) { // Starts SHA-256 stream hashing.
    std::array<std::uint32_t, 8> var_b8a95e6b_state_arr = {0x6a09e667U, 0xbb67ae85U, 0x3c6ef372U, 0xa54ff53aU, 0x510e527fU, 0x9b05688cU, 0x1f83d9abU, 0x5be0cd19U}; // Initializes the eight standard SHA-256 chaining values.
    const std::array<std::uint32_t, 64> var_b8a95e6b_constants_arr = { // Stores the sixty-four SHA-256 round constants.
        0x428a2f98U, 0x71374491U, 0xb5c0fbcfU, 0xe9b5dba5U, 0x3956c25bU, 0x59f111f1U, 0x923f82a4U, 0xab1c5ed5U, // Stores constants 0 through 7.
        0xd807aa98U, 0x12835b01U, 0x243185beU, 0x550c7dc3U, 0x72be5d74U, 0x80deb1feU, 0x9bdc06a7U, 0xc19bf174U, // Stores constants 8 through 15.
        0xe49b69c1U, 0xefbe4786U, 0x0fc19dc6U, 0x240ca1ccU, 0x2de92c6fU, 0x4a7484aaU, 0x5cb0a9dcU, 0x76f988daU, // Stores constants 16 through 23.
        0x983e5152U, 0xa831c66dU, 0xb00327c8U, 0xbf597fc7U, 0xc6e00bf3U, 0xd5a79147U, 0x06ca6351U, 0x14292967U, // Stores constants 24 through 31.
        0x27b70a85U, 0x2e1b2138U, 0x4d2c6dfcU, 0x53380d13U, 0x650a7354U, 0x766a0abbU, 0x81c2c92eU, 0x92722c85U, // Stores constants 32 through 39.
        0xa2bfe8a1U, 0xa81a664bU, 0xc24b8b70U, 0xc76c51a3U, 0xd192e819U, 0xd6990624U, 0xf40e3585U, 0x106aa070U, // Stores constants 40 through 47.
        0x19a4c116U, 0x1e376c08U, 0x2748774cU, 0x34b0bcb5U, 0x391c0cb3U, 0x4ed8aa4aU, 0x5b9cca4fU, 0x682e6ff3U, // Stores constants 48 through 55.
        0x748f82eeU, 0x78a5636fU, 0x84c87814U, 0x8cc70208U, 0x90befffaU, 0xa4506cebU, 0xbef9a3f7U, 0xc67178f2U // Stores constants 56 through 63.
    }; // Ends the SHA-256 constants array.
    std::uint64_t var_b8a95e6b_total_bytes_num = 0U; // Counts original source bytes for the final length field.
    std::vector<std::uint8_t> var_b8a95e6b_pending_arr; // Stores bytes that do not yet form a complete 64-byte block.
    var_b8a95e6b_pending_arr.reserve(8256U); // Reserves room for one source chunk plus a partial block.
    auto var_b8a95e6b_process_block_obj = [&](const std::uint8_t* var_b8a95e6b_block_arr) { // Defines localized processing for one SHA-256 message block.
        std::array<std::uint32_t, 64> var_b8a95e6b_schedule_arr{}; // Allocates the sixty-four-word SHA-256 message schedule.
        for (std::size_t var_b8a95e6b_word_index_num = 0U; var_b8a95e6b_word_index_num < 16U; ++var_b8a95e6b_word_index_num) { // Decodes the sixteen words stored directly in the message block.
            std::size_t var_b8a95e6b_offset_num = var_b8a95e6b_word_index_num * 4U; // Calculates the byte offset of the current word.
            var_b8a95e6b_schedule_arr[var_b8a95e6b_word_index_num] = (static_cast<std::uint32_t>(var_b8a95e6b_block_arr[var_b8a95e6b_offset_num]) << 24U) | (static_cast<std::uint32_t>(var_b8a95e6b_block_arr[var_b8a95e6b_offset_num + 1U]) << 16U) | (static_cast<std::uint32_t>(var_b8a95e6b_block_arr[var_b8a95e6b_offset_num + 2U]) << 8U) | static_cast<std::uint32_t>(var_b8a95e6b_block_arr[var_b8a95e6b_offset_num + 3U]); // Decodes one big-endian 32-bit word.
        } // Ends input-word decoding.
        for (std::size_t var_b8a95e6b_word_index_num = 16U; var_b8a95e6b_word_index_num < 64U; ++var_b8a95e6b_word_index_num) { // Expands the initial words into the remaining schedule entries.
            std::uint32_t var_b8a95e6b_previous15_num = var_b8a95e6b_schedule_arr[var_b8a95e6b_word_index_num - 15U]; // Loads the schedule word used by small sigma zero.
            std::uint32_t var_b8a95e6b_previous2_num = var_b8a95e6b_schedule_arr[var_b8a95e6b_word_index_num - 2U]; // Loads the schedule word used by small sigma one.
            std::uint32_t var_b8a95e6b_sigma0_num = ((var_b8a95e6b_previous15_num >> 7U) | (var_b8a95e6b_previous15_num << 25U)) ^ ((var_b8a95e6b_previous15_num >> 18U) | (var_b8a95e6b_previous15_num << 14U)) ^ (var_b8a95e6b_previous15_num >> 3U); // Calculates SHA-256 small sigma zero.
            std::uint32_t var_b8a95e6b_sigma1_num = ((var_b8a95e6b_previous2_num >> 17U) | (var_b8a95e6b_previous2_num << 15U)) ^ ((var_b8a95e6b_previous2_num >> 19U) | (var_b8a95e6b_previous2_num << 13U)) ^ (var_b8a95e6b_previous2_num >> 10U); // Calculates SHA-256 small sigma one.
            var_b8a95e6b_schedule_arr[var_b8a95e6b_word_index_num] = var_b8a95e6b_schedule_arr[var_b8a95e6b_word_index_num - 16U] + var_b8a95e6b_sigma0_num + var_b8a95e6b_schedule_arr[var_b8a95e6b_word_index_num - 7U] + var_b8a95e6b_sigma1_num; // Produces the next schedule word modulo 2^32.
        } // Ends schedule expansion.
        std::uint32_t var_b8a95e6b_a_num = var_b8a95e6b_state_arr[0]; // Copies chaining value zero into working register A.
        std::uint32_t var_b8a95e6b_b_num = var_b8a95e6b_state_arr[1]; // Copies chaining value one into working register B.
        std::uint32_t var_b8a95e6b_c_num = var_b8a95e6b_state_arr[2]; // Copies chaining value two into working register C.
        std::uint32_t var_b8a95e6b_d_num = var_b8a95e6b_state_arr[3]; // Copies chaining value three into working register D.
        std::uint32_t var_b8a95e6b_e_num = var_b8a95e6b_state_arr[4]; // Copies chaining value four into working register E.
        std::uint32_t var_b8a95e6b_f_num = var_b8a95e6b_state_arr[5]; // Copies chaining value five into working register F.
        std::uint32_t var_b8a95e6b_g_num = var_b8a95e6b_state_arr[6]; // Copies chaining value six into working register G.
        std::uint32_t var_b8a95e6b_h_num = var_b8a95e6b_state_arr[7]; // Copies chaining value seven into working register H.
        for (std::size_t var_b8a95e6b_round_index_num = 0U; var_b8a95e6b_round_index_num < 64U; ++var_b8a95e6b_round_index_num) { // Performs all sixty-four SHA-256 compression rounds.
            std::uint32_t var_b8a95e6b_big_sigma1_num = ((var_b8a95e6b_e_num >> 6U) | (var_b8a95e6b_e_num << 26U)) ^ ((var_b8a95e6b_e_num >> 11U) | (var_b8a95e6b_e_num << 21U)) ^ ((var_b8a95e6b_e_num >> 25U) | (var_b8a95e6b_e_num << 7U)); // Calculates SHA-256 big sigma one from E.
            std::uint32_t var_b8a95e6b_choose_num = (var_b8a95e6b_e_num & var_b8a95e6b_f_num) ^ ((~var_b8a95e6b_e_num) & var_b8a95e6b_g_num); // Calculates the SHA-256 choose function.
            std::uint32_t var_b8a95e6b_temporary1_num = var_b8a95e6b_h_num + var_b8a95e6b_big_sigma1_num + var_b8a95e6b_choose_num + var_b8a95e6b_constants_arr[var_b8a95e6b_round_index_num] + var_b8a95e6b_schedule_arr[var_b8a95e6b_round_index_num]; // Computes temporary value one modulo 2^32.
            std::uint32_t var_b8a95e6b_big_sigma0_num = ((var_b8a95e6b_a_num >> 2U) | (var_b8a95e6b_a_num << 30U)) ^ ((var_b8a95e6b_a_num >> 13U) | (var_b8a95e6b_a_num << 19U)) ^ ((var_b8a95e6b_a_num >> 22U) | (var_b8a95e6b_a_num << 10U)); // Calculates SHA-256 big sigma zero from A.
            std::uint32_t var_b8a95e6b_majority_num = (var_b8a95e6b_a_num & var_b8a95e6b_b_num) ^ (var_b8a95e6b_a_num & var_b8a95e6b_c_num) ^ (var_b8a95e6b_b_num & var_b8a95e6b_c_num); // Calculates the SHA-256 majority function.
            std::uint32_t var_b8a95e6b_temporary2_num = var_b8a95e6b_big_sigma0_num + var_b8a95e6b_majority_num; // Computes temporary value two modulo 2^32.
            var_b8a95e6b_h_num = var_b8a95e6b_g_num; // Rotates working register G into H.
            var_b8a95e6b_g_num = var_b8a95e6b_f_num; // Rotates working register F into G.
            var_b8a95e6b_f_num = var_b8a95e6b_e_num; // Rotates working register E into F.
            var_b8a95e6b_e_num = var_b8a95e6b_d_num + var_b8a95e6b_temporary1_num; // Computes the new E value modulo 2^32.
            var_b8a95e6b_d_num = var_b8a95e6b_c_num; // Rotates working register C into D.
            var_b8a95e6b_c_num = var_b8a95e6b_b_num; // Rotates working register B into C.
            var_b8a95e6b_b_num = var_b8a95e6b_a_num; // Rotates working register A into B.
            var_b8a95e6b_a_num = var_b8a95e6b_temporary1_num + var_b8a95e6b_temporary2_num; // Computes the new A value modulo 2^32.
        } // Ends compression rounds.
        var_b8a95e6b_state_arr[0] += var_b8a95e6b_a_num; // Adds working register A back into chaining value zero.
        var_b8a95e6b_state_arr[1] += var_b8a95e6b_b_num; // Adds working register B back into chaining value one.
        var_b8a95e6b_state_arr[2] += var_b8a95e6b_c_num; // Adds working register C back into chaining value two.
        var_b8a95e6b_state_arr[3] += var_b8a95e6b_d_num; // Adds working register D back into chaining value three.
        var_b8a95e6b_state_arr[4] += var_b8a95e6b_e_num; // Adds working register E back into chaining value four.
        var_b8a95e6b_state_arr[5] += var_b8a95e6b_f_num; // Adds working register F back into chaining value five.
        var_b8a95e6b_state_arr[6] += var_b8a95e6b_g_num; // Adds working register G back into chaining value six.
        var_b8a95e6b_state_arr[7] += var_b8a95e6b_h_num; // Adds working register H back into chaining value seven.
    }; // Ends localized SHA-256 block processing.
    char var_b8a95e6b_buffer_arr[8192]; // Allocates a fixed-size source read buffer.
    while (true) { // Repeats source reads until explicit end-of-file or an I/O error is detected.
        var_b8a95e6b_input_stream_obj.read(var_b8a95e6b_buffer_arr, sizeof(var_b8a95e6b_buffer_arr)); // Attempts to read the next fixed-size source chunk without relying on implicit stream truthiness.
        std::streamsize var_b8a95e6b_bytes_read_num = var_b8a95e6b_input_stream_obj.gcount(); // Records how many bytes were actually read into the current source chunk.
        if (var_b8a95e6b_bytes_read_num <= 0) { // Checks whether the read produced no bytes that need hashing.
            if (var_b8a95e6b_input_stream_obj.eof() == true) { // Checks explicitly for normal end-of-file.
                break; // Stops source streaming after all bytes have been processed.
            } // Ends explicit end-of-file handling.
            if ((var_b8a95e6b_input_stream_obj.bad() == true) || (var_b8a95e6b_input_stream_obj.fail() == true)) { // Checks explicitly for a non-EOF stream failure.
                return false; // Reports failure rather than hashing incomplete source data.
            } // Ends explicit stream-failure handling.
            continue; // Retries only if the stream reported neither bytes, EOF, nor a failure state.
        } // Ends no-bytes handling.
        var_b8a95e6b_total_bytes_num += static_cast<std::uint64_t>(var_b8a95e6b_bytes_read_num); // Adds the current chunk size to the original message length.
        for (std::streamsize var_b8a95e6b_copy_index_num = 0; var_b8a95e6b_copy_index_num < var_b8a95e6b_bytes_read_num; ++var_b8a95e6b_copy_index_num) { // Copies the current source chunk into the pending-byte buffer.
            var_b8a95e6b_pending_arr.push_back(static_cast<std::uint8_t>(var_b8a95e6b_buffer_arr[var_b8a95e6b_copy_index_num])); // Preserves each source byte exactly.
        } // Ends current-chunk copying.
        std::size_t var_b8a95e6b_processed_bytes_num = 0U; // Tracks complete pending bytes processed during this read cycle.
        while ((var_b8a95e6b_pending_arr.size() - var_b8a95e6b_processed_bytes_num) >= 64U) { // Processes every complete 64-byte message block.
            var_b8a95e6b_process_block_obj(var_b8a95e6b_pending_arr.data() + var_b8a95e6b_processed_bytes_num); // Transforms the next complete SHA-256 block.
            var_b8a95e6b_processed_bytes_num += 64U; // Advances past the transformed block.
        } // Ends complete-block processing.
        if (var_b8a95e6b_processed_bytes_num > 0U) { // Checks whether processed bytes can be discarded from the pending buffer.
            var_b8a95e6b_pending_arr.erase(var_b8a95e6b_pending_arr.begin(), var_b8a95e6b_pending_arr.begin() + static_cast<std::ptrdiff_t>(var_b8a95e6b_processed_bytes_num)); // Retains only the incomplete tail for the next read.
        } // Ends pending-buffer compaction.
    } // Ends source streaming.
    if (var_b8a95e6b_input_stream_obj.bad() == true) { // Checks whether a genuine source I/O failure occurred.
        return false; // Reports failure instead of returning a digest for incomplete input.
    } // Ends source error checking.
    std::uint64_t var_b8a95e6b_total_bits_num = var_b8a95e6b_total_bytes_num * 8U; // Converts the original message length from bytes to bits.
    var_b8a95e6b_pending_arr.push_back(0x80U); // Appends the mandatory one bit followed by seven zero bits.
    while ((var_b8a95e6b_pending_arr.size() % 64U) != 56U) { // Pads with zeros until the final eight-byte length field fits at the end of a block.
        var_b8a95e6b_pending_arr.push_back(0x00U); // Appends one zero-padding byte.
    } // Ends zero padding.
    for (int var_b8a95e6b_length_index_num = 7; var_b8a95e6b_length_index_num >= 0; --var_b8a95e6b_length_index_num) { // Appends the original bit length in big-endian order.
        var_b8a95e6b_pending_arr.push_back(static_cast<std::uint8_t>((var_b8a95e6b_total_bits_num >> (8U * static_cast<std::uint32_t>(var_b8a95e6b_length_index_num))) & 0xffU)); // Stores one byte of the eight-byte SHA-256 length field.
    } // Ends length-field construction.
    for (std::size_t var_b8a95e6b_final_offset_num = 0U; var_b8a95e6b_final_offset_num < var_b8a95e6b_pending_arr.size(); var_b8a95e6b_final_offset_num += 64U) { // Processes every final padded block.
        var_b8a95e6b_process_block_obj(var_b8a95e6b_pending_arr.data() + var_b8a95e6b_final_offset_num); // Transforms one final SHA-256 block.
    } // Ends final padded block processing.
    std::ostringstream var_b8a95e6b_hex_stream_obj; // Creates a stream for lowercase hexadecimal digest output.
    var_b8a95e6b_hex_stream_obj << std::hex << std::nouppercase << std::setfill('0'); // Configures lowercase hexadecimal encoding with zero padding.
    for (std::size_t var_b8a95e6b_state_index_num = 0U; var_b8a95e6b_state_index_num < var_b8a95e6b_state_arr.size(); ++var_b8a95e6b_state_index_num) { // Serializes all eight 32-bit chaining values.
        var_b8a95e6b_hex_stream_obj << std::setw(8) << var_b8a95e6b_state_arr[var_b8a95e6b_state_index_num]; // Writes exactly eight hexadecimal characters for the current state word.
    } // Ends digest serialization.
    var_b8a95e6b_checksum_str = var_b8a95e6b_hex_stream_obj.str(); // Returns the final 256-bit SHA-256 digest as hexadecimal text.
    return true; // Reports successful SHA-256 hashing.
} // Ends SHA-256 stream hashing.
