// Description: Implements SHA3-512 directly in C++ using Keccak-f[1600] without external hashing libraries.
#include "hash_sha3_512.hpp" // Imports the SHA3-512 function declaration.
#include <array> // Provides fixed-size arrays for the Keccak state, temporary lanes, offsets, and constants.
#include <cstddef> // Provides std::size_t and std::ptrdiff_t.
#include <cstdint> // Provides fixed-width unsigned integer types.
#include <iomanip> // Provides hexadecimal width and fill formatting.
#include <sstream> // Provides an output string stream for hexadecimal conversion.
#include <vector> // Provides a small pending-byte buffer used across stream reads.
bool fun_e09a33f2_hash_sha3_512_stream_i1790544728_v1(std::istream& var_e09a33f2_input_stream_obj, std::string& var_e09a33f2_checksum_str) { // Starts SHA3-512 stream hashing.
    constexpr std::size_t var_e09a33f2_rate_num = 72U; // Sets the SHA3-512 sponge rate to 576 bits or 72 bytes.
    constexpr std::size_t var_e09a33f2_output_bytes_num = 64U; // Sets the SHA3-512 digest size to 512 bits or 64 bytes.
    std::array<std::uint64_t, 25> var_e09a33f2_state_arr{}; // Initializes the complete 1600-bit Keccak state to zero.
    const std::array<std::uint64_t, 24> var_e09a33f2_round_constants_arr = { // Stores the twenty-four Keccak-f round constants.
        0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL, 0x8000000080008000ULL, // Stores round constants 0 through 3.
        0x000000000000808bULL, 0x0000000080000001ULL, 0x8000000080008081ULL, 0x8000000000008009ULL, // Stores round constants 4 through 7.
        0x000000000000008aULL, 0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL, // Stores round constants 8 through 11.
        0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL, 0x8000000000008003ULL, // Stores round constants 12 through 15.
        0x8000000000008002ULL, 0x8000000000000080ULL, 0x000000000000800aULL, 0x800000008000000aULL, // Stores round constants 16 through 19.
        0x8000000080008081ULL, 0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL // Stores round constants 20 through 23.
    }; // Ends the Keccak round-constant array.
    const std::array<unsigned int, 25> var_e09a33f2_rotation_offsets_arr = { // Stores the Keccak rho rotation offset for each lane indexed as x plus five times y.
        0U, 1U, 62U, 28U, 27U, // Stores offsets for y equals zero.
        36U, 44U, 6U, 55U, 20U, // Stores offsets for y equals one.
        3U, 10U, 43U, 25U, 39U, // Stores offsets for y equals two.
        41U, 45U, 15U, 21U, 8U, // Stores offsets for y equals three.
        18U, 2U, 61U, 56U, 14U // Stores offsets for y equals four.
    }; // Ends the Keccak rho offset array.
    auto var_e09a33f2_permute_obj = [&]() { // Defines the localized Keccak-f[1600] permutation used after each absorbed block.
        for (std::size_t var_e09a33f2_round_index_num = 0U; var_e09a33f2_round_index_num < 24U; ++var_e09a33f2_round_index_num) { // Performs all twenty-four Keccak-f rounds.
            std::array<std::uint64_t, 5> var_e09a33f2_columns_arr{}; // Stores the theta parity of each x column.
            std::array<std::uint64_t, 5> var_e09a33f2_theta_arr{}; // Stores the theta adjustment for each x column.
            for (std::size_t var_e09a33f2_x_num = 0U; var_e09a33f2_x_num < 5U; ++var_e09a33f2_x_num) { // Calculates the parity of every state column.
                var_e09a33f2_columns_arr[var_e09a33f2_x_num] = var_e09a33f2_state_arr[var_e09a33f2_x_num] ^ var_e09a33f2_state_arr[var_e09a33f2_x_num + 5U] ^ var_e09a33f2_state_arr[var_e09a33f2_x_num + 10U] ^ var_e09a33f2_state_arr[var_e09a33f2_x_num + 15U] ^ var_e09a33f2_state_arr[var_e09a33f2_x_num + 20U]; // XORs the five lanes belonging to the current x column.
            } // Ends theta column parity calculation.
            for (std::size_t var_e09a33f2_x_num = 0U; var_e09a33f2_x_num < 5U; ++var_e09a33f2_x_num) { // Calculates the theta adjustment for every x column.
                std::uint64_t var_e09a33f2_neighbor_num = var_e09a33f2_columns_arr[(var_e09a33f2_x_num + 1U) % 5U]; // Loads the next column parity that must be rotated left by one bit.
                std::uint64_t var_e09a33f2_rotated_neighbor_num = (var_e09a33f2_neighbor_num << 1U) | (var_e09a33f2_neighbor_num >> 63U); // Rotates the neighboring parity left by one bit.
                var_e09a33f2_theta_arr[var_e09a33f2_x_num] = var_e09a33f2_columns_arr[(var_e09a33f2_x_num + 4U) % 5U] ^ var_e09a33f2_rotated_neighbor_num; // Combines the previous-column parity and rotated next-column parity.
            } // Ends theta adjustment calculation.
            for (std::size_t var_e09a33f2_y_num = 0U; var_e09a33f2_y_num < 5U; ++var_e09a33f2_y_num) { // Applies theta to each row of state lanes.
                for (std::size_t var_e09a33f2_x_num = 0U; var_e09a33f2_x_num < 5U; ++var_e09a33f2_x_num) { // Applies the correct x-column theta adjustment to each lane.
                    var_e09a33f2_state_arr[var_e09a33f2_x_num + (5U * var_e09a33f2_y_num)] ^= var_e09a33f2_theta_arr[var_e09a33f2_x_num]; // XORs the theta adjustment into the current lane.
                } // Ends theta application across one row.
            } // Ends theta application to the state.
            std::array<std::uint64_t, 25> var_e09a33f2_reordered_arr{}; // Allocates the intermediate state used for rho rotations and pi lane movement.
            for (std::size_t var_e09a33f2_y_num = 0U; var_e09a33f2_y_num < 5U; ++var_e09a33f2_y_num) { // Visits every source row for rho and pi.
                for (std::size_t var_e09a33f2_x_num = 0U; var_e09a33f2_x_num < 5U; ++var_e09a33f2_x_num) { // Visits every source lane for rho and pi.
                    std::size_t var_e09a33f2_source_index_num = var_e09a33f2_x_num + (5U * var_e09a33f2_y_num); // Calculates the flat index of the source lane.
                    unsigned int var_e09a33f2_rotation_num = var_e09a33f2_rotation_offsets_arr[var_e09a33f2_source_index_num]; // Loads the rho rotation amount for the source lane.
                    std::uint64_t var_e09a33f2_lane_num = var_e09a33f2_state_arr[var_e09a33f2_source_index_num]; // Copies the source lane before rotation.
                    std::uint64_t var_e09a33f2_rotated_lane_num = (var_e09a33f2_rotation_num == 0U) ? var_e09a33f2_lane_num : ((var_e09a33f2_lane_num << var_e09a33f2_rotation_num) | (var_e09a33f2_lane_num >> (64U - var_e09a33f2_rotation_num))); // Rotates the lane left while explicitly avoiding a shift by sixty-four for the zero-offset lane.
                    std::size_t var_e09a33f2_destination_x_num = var_e09a33f2_y_num; // Applies the pi mapping new x equals old y.
                    std::size_t var_e09a33f2_destination_y_num = ((2U * var_e09a33f2_x_num) + (3U * var_e09a33f2_y_num)) % 5U; // Applies the pi mapping new y equals two x plus three y modulo five.
                    var_e09a33f2_reordered_arr[var_e09a33f2_destination_x_num + (5U * var_e09a33f2_destination_y_num)] = var_e09a33f2_rotated_lane_num; // Stores the rho-rotated lane at its pi destination.
                } // Ends rho and pi processing for one row.
            } // Ends rho and pi processing for all lanes.
            for (std::size_t var_e09a33f2_y_num = 0U; var_e09a33f2_y_num < 5U; ++var_e09a33f2_y_num) { // Applies chi independently to every state row.
                for (std::size_t var_e09a33f2_x_num = 0U; var_e09a33f2_x_num < 5U; ++var_e09a33f2_x_num) { // Applies chi to each lane in the current row.
                    var_e09a33f2_state_arr[var_e09a33f2_x_num + (5U * var_e09a33f2_y_num)] = var_e09a33f2_reordered_arr[var_e09a33f2_x_num + (5U * var_e09a33f2_y_num)] ^ ((~var_e09a33f2_reordered_arr[((var_e09a33f2_x_num + 1U) % 5U) + (5U * var_e09a33f2_y_num)]) & var_e09a33f2_reordered_arr[((var_e09a33f2_x_num + 2U) % 5U) + (5U * var_e09a33f2_y_num)]); // Combines the current lane with its two neighbors using the Keccak chi rule.
                } // Ends chi processing for one row.
            } // Ends chi processing for all rows.
            var_e09a33f2_state_arr[0] ^= var_e09a33f2_round_constants_arr[var_e09a33f2_round_index_num]; // Applies iota by XORing the current round constant into lane zero.
        } // Ends all Keccak-f rounds.
    }; // Ends localized Keccak-f permutation logic.
    auto var_e09a33f2_absorb_block_obj = [&](const std::uint8_t* var_e09a33f2_block_arr) { // Defines localized absorption for exactly one SHA3-512 rate block.
        for (std::size_t var_e09a33f2_lane_index_num = 0U; var_e09a33f2_lane_index_num < (var_e09a33f2_rate_num / 8U); ++var_e09a33f2_lane_index_num) { // Decodes every 64-bit lane present in the rate portion of the block.
            std::uint64_t var_e09a33f2_lane_num = 0ULL; // Initializes the little-endian lane assembled from eight input bytes.
            for (std::size_t var_e09a33f2_byte_index_num = 0U; var_e09a33f2_byte_index_num < 8U; ++var_e09a33f2_byte_index_num) { // Adds all eight bytes to the current lane.
                var_e09a33f2_lane_num |= static_cast<std::uint64_t>(var_e09a33f2_block_arr[(var_e09a33f2_lane_index_num * 8U) + var_e09a33f2_byte_index_num]) << (8U * var_e09a33f2_byte_index_num); // Places the current byte into its little-endian position in the lane.
            } // Ends lane byte assembly.
            var_e09a33f2_state_arr[var_e09a33f2_lane_index_num] ^= var_e09a33f2_lane_num; // XORs the assembled message lane into the corresponding rate lane of the state.
        } // Ends rate-lane absorption.
        var_e09a33f2_permute_obj(); // Applies Keccak-f after the complete rate block has been absorbed.
    }; // Ends localized rate-block absorption.
    std::vector<std::uint8_t> var_e09a33f2_pending_arr; // Stores source bytes that do not yet form a complete 72-byte rate block.
    var_e09a33f2_pending_arr.reserve(8192U + var_e09a33f2_rate_num); // Reserves room for a source chunk plus one incomplete rate block.
    char var_e09a33f2_buffer_arr[8192]; // Allocates a fixed-size source read buffer.
    while (true) { // Repeats source reads until explicit end-of-file or an I/O error is detected.
        var_e09a33f2_input_stream_obj.read(var_e09a33f2_buffer_arr, sizeof(var_e09a33f2_buffer_arr)); // Attempts to read the next fixed-size source chunk without relying on implicit stream truthiness.
        std::streamsize var_e09a33f2_bytes_read_num = var_e09a33f2_input_stream_obj.gcount(); // Records how many bytes were actually read into the current chunk.
        if (var_e09a33f2_bytes_read_num <= 0) { // Checks whether the read produced no bytes that need hashing.
            if (var_e09a33f2_input_stream_obj.eof() == true) { // Checks explicitly for normal end-of-file.
                break; // Stops source streaming after all bytes have been processed.
            } // Ends explicit end-of-file handling.
            if ((var_e09a33f2_input_stream_obj.bad() == true) || (var_e09a33f2_input_stream_obj.fail() == true)) { // Checks explicitly for a non-EOF stream failure.
                return false; // Reports failure rather than hashing incomplete source data.
            } // Ends explicit stream-failure handling.
            continue; // Retries only if the stream reported neither bytes, EOF, nor a failure state.
        } // Ends no-bytes handling.
        for (std::streamsize var_e09a33f2_copy_index_num = 0; var_e09a33f2_copy_index_num < var_e09a33f2_bytes_read_num; ++var_e09a33f2_copy_index_num) { // Copies the current source chunk into the pending-byte buffer.
            var_e09a33f2_pending_arr.push_back(static_cast<std::uint8_t>(var_e09a33f2_buffer_arr[var_e09a33f2_copy_index_num])); // Preserves every source byte exactly.
        } // Ends current-chunk copying.
        std::size_t var_e09a33f2_processed_bytes_num = 0U; // Tracks complete rate-block bytes absorbed during this read cycle.
        while ((var_e09a33f2_pending_arr.size() - var_e09a33f2_processed_bytes_num) >= var_e09a33f2_rate_num) { // Absorbs every complete 72-byte rate block now available.
            var_e09a33f2_absorb_block_obj(var_e09a33f2_pending_arr.data() + var_e09a33f2_processed_bytes_num); // Absorbs and permutes the next complete rate block.
            var_e09a33f2_processed_bytes_num += var_e09a33f2_rate_num; // Advances past the absorbed rate block.
        } // Ends complete rate-block absorption.
        if (var_e09a33f2_processed_bytes_num > 0U) { // Checks whether absorbed bytes can be removed from the pending buffer.
            var_e09a33f2_pending_arr.erase(var_e09a33f2_pending_arr.begin(), var_e09a33f2_pending_arr.begin() + static_cast<std::ptrdiff_t>(var_e09a33f2_processed_bytes_num)); // Retains only the incomplete source tail needed for final padding or the next read.
        } // Ends pending-buffer compaction.
    } // Ends source streaming.
    if (var_e09a33f2_input_stream_obj.bad() == true) { // Checks whether a genuine source I/O failure interrupted hashing.
        return false; // Reports failure rather than returning a digest for incomplete data.
    } // Ends source error checking.
    std::size_t var_e09a33f2_message_tail_size_num = var_e09a33f2_pending_arr.size(); // Records where SHA-3 domain padding begins in the final rate block.
    var_e09a33f2_pending_arr.resize(var_e09a33f2_rate_num, 0x00U); // Extends the final partial block to one complete rate block filled with zeros.
    var_e09a33f2_pending_arr[var_e09a33f2_message_tail_size_num] ^= 0x06U; // Applies the SHA-3 domain-separation bits and the first padding bit.
    var_e09a33f2_pending_arr[var_e09a33f2_rate_num - 1U] ^= 0x80U; // Applies the final multi-rate padding bit at the end of the rate block.
    var_e09a33f2_absorb_block_obj(var_e09a33f2_pending_arr.data()); // Absorbs the final padded block and performs the final permutation before squeezing.
    std::ostringstream var_e09a33f2_hex_stream_obj; // Creates a stream for lowercase hexadecimal digest encoding.
    var_e09a33f2_hex_stream_obj << std::hex << std::nouppercase << std::setfill('0'); // Configures lowercase hexadecimal formatting with zero padding.
    for (std::size_t var_e09a33f2_output_index_num = 0U; var_e09a33f2_output_index_num < var_e09a33f2_output_bytes_num; ++var_e09a33f2_output_index_num) { // Squeezes the first sixty-four bytes of the state as the SHA3-512 digest.
        std::uint8_t var_e09a33f2_digest_byte_num = static_cast<std::uint8_t>((var_e09a33f2_state_arr[var_e09a33f2_output_index_num / 8U] >> (8U * (var_e09a33f2_output_index_num % 8U))) & 0xffULL); // Extracts one digest byte from the little-endian Keccak lane layout.
        var_e09a33f2_hex_stream_obj << std::setw(2) << static_cast<unsigned int>(var_e09a33f2_digest_byte_num); // Writes exactly two hexadecimal characters for the digest byte.
    } // Ends digest squeezing and hexadecimal serialization.
    var_e09a33f2_checksum_str = var_e09a33f2_hex_stream_obj.str(); // Returns the final 512-bit SHA3-512 digest as hexadecimal text.
    return true; // Reports successful SHA3-512 hashing.
} // Ends SHA3-512 stream hashing.
