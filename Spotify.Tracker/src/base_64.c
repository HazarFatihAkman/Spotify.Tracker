#include "../include/base_64.h"

const char encoding_table[] = { 
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/' };

char* base64_encode(char *str) {
    int len = strlen(str),
        len_8_bit = len * 8,
        len_6_bit = len_8_bit % 6 == 0 ? len_8_bit / 6 : (len_8_bit / 6) + 1,
        miss_bit_count = 0;

    int i,
        k = 0,
        j = 0,
        z = 0;

    int binary_8bit[len_8_bit],
        decimal_values[len_6_bit];

    char *response_str = malloc(len_6_bit * sizeof(char));

                                                                            // to 8bit
    for (i = 0; i < len; i++) {
        for (j = 7; j >= 0; j--) {
            binary_8bit[k] = (str[i] >> j) & 1;
            k++;
        }
    }

    k = 0;
    j = 2;

    char *temp_str = malloc(8 * sizeof(char));
                                                                            // you should add 00 at the start because that should be 8-bit code
    temp_str[0] = '0';
    temp_str[1] = '0';

                                                                            // to decimal and get letter
    for (i = 1; i <= len_8_bit; i++) {
        if (i % 6 == 0) {                                                   // binary_8-bit to 6-bit first
            temp_str[j] = binary_8bit[z] + '0';                             // get binary number and set it
            response_str[k] = encoding_table[strtol(temp_str, NULL, 2)];    // convert decimal to letter from encoding table
            miss_bit_count = 6 - (len_8_bit - i);                            // check missing count of 8-bit
            j = 2;
            k++;
        }
        else {
            temp_str[j] = binary_8bit[z] + '0';
            j++;
        }
                                                                            // add missing ones to last and convert last decimal to letter to make it 6-bit
        for (miss_bit_count; miss_bit_count > 0 && miss_bit_count < 6 && i == len_8_bit; miss_bit_count--) {
            temp_str[j] = '0';
            j++;
            if (miss_bit_count == 1) {
                response_str[k] = encoding_table[strtol(temp_str, NULL, 2)];
            }
        }
        z++;
    }

    free(temp_str);
    temp_str = NULL;

    return response_str;
}
