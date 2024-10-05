
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "decode.h"
#include "ast.h"

/* Static variables to hold buffer information, local to the file */
static char *buffer = NULL;      /* Buffer to hold the final string */
static size_t buffer_size = 0;   /* Total size of the buffer */
static size_t used_size = 0;     /* Current position in the buffer (used space) */

/* Initial buffer size and grow factor for reallocation */
#define INITIAL_BUFFER_SIZE 1024
#define BUFFER_GROW_FACTOR 2

static int add_hash_after_newline(char **buffer, size_t *buffer_size, size_t *used_size);
static void print_instr(instr_t *instr);

/* Function to add '#' after every newline and reallocate buffer if needed */
int add_hash_after_newline(char **buffer, size_t *buffer_size, size_t *used_size)
{
    size_t extra_needed = 0;

    /* Calculate how many extra '#' we need (1 for each '\n') */
    for (size_t i = 0; i < *used_size; i++)
    {
        if ((*buffer)[i] == '\n')
        {
            extra_needed++;
        }
    }

    /* If no newline found, no need to modify buffer */
    if (extra_needed == 0)
    {
        return 0;
    }

    /* Reallocate buffer if necessary to fit the extra '#' characters */
    size_t new_size = *used_size + extra_needed;
    if (new_size >= *buffer_size)
    {
        char *new_buffer = realloc(*buffer, new_size + 1); /* +1 for null terminator */
        if (!new_buffer)
        {
            return -1; /* Memory reallocation failure */
        }
        *buffer = new_buffer;
        *buffer_size = new_size + 1;
    }

    /* Process the buffer in reverse to avoid overwriting while inserting '#' */
    for (ssize_t i = *used_size - 1; i >= 0; i--)
    {
        (*buffer)[i + extra_needed] = (*buffer)[i]; /* Shift characters */
        if ((*buffer)[i] == '\n')
        {
            extra_needed--;
            (*buffer)[i + extra_needed] = '\n';
            (*buffer)[i + extra_needed+1] = '#';
        }
    }


    /* Update used size after adding '#' characters */
    *used_size = new_size;
    (*buffer)[*used_size] = '\0'; /* Ensure null-terminated string */

    return 0;
}

/* Internal function to append formatted strings to the buffer */
static int buffered_print(const char *format, ...)
{
    /* If the buffer is NULL, allocate the initial buffer size */
    if (buffer == NULL)
    {
        buffer_size = INITIAL_BUFFER_SIZE;
        buffer = (char *)malloc(buffer_size);
        if (!buffer)
        {
            return -1; /* Memory allocation failure */
        }
        buffer[0] = '#'; buffer[1] = '\0'; /* Initialize the buffer with CR */
        used_size = 1;    /* Reset the used size */
    }

    /* Estimate how much space is needed for the new formatted string */
    va_list args;
    va_start(args, format);
    int required_space = vsnprintf(NULL, 0, format, args) + 1; /* +1 for the null terminator */
    va_end(args);

    /* Check if we need to reallocate more space */
    if (used_size + required_space > buffer_size)
    {
        /* Reallocate with more space */
        size_t new_size = buffer_size * BUFFER_GROW_FACTOR;
        while (used_size + required_space > new_size)
        {
            new_size *= BUFFER_GROW_FACTOR; /* Ensure enough space */
        }

        char *new_buffer = (char *)realloc(buffer, new_size);
        if (!new_buffer)
        {
            return -1; /* Memory reallocation failure */
        }
        buffer = new_buffer;
        buffer_size = new_size;
    }

    /* Now append the formatted string to the buffer */
    va_start(args, format);
    int written = vsnprintf(buffer + used_size, buffer_size - used_size, format, args);
    va_end(args);

    /* Move the used_size forward */
    used_size += written;

	return written;
}

/* Function to start the printing process and return the buffer */
int sprint_instr(char **output_buffer, instr_t *instr)
{
	buffer = NULL;
	buffer_size = 0;
	used_size = 0;

	print_instr(instr);

    /* Assign the buffer to the caller's output pointer */
    *output_buffer = buffer;

    /* Return 0 to indicate success */
    return add_hash_after_newline(&buffer, &buffer_size, &used_size) == 0 ;
}

instr_type_e get_instr_type(unsigned char *data)
{
    unsigned int opcode = bin_to_int(data, 6, 0);
    unsigned int funct3 = bin_to_int(data, 14, 12);
    switch (opcode)
    {
    case 0b0110011:
        return R_type;
    case 0b0010011:
        switch (funct3)
        {
        case 0b001:
        case 0b101:
            return I_SHAMT_type;
        default:
            return I_type;
        }
    case 0b1100111:
    case 0b0000011:
        return I_type;
    case 0b1100011:
        return B_type;
    case 0b0110111:
    case 0b0010111:
        return U_type;
    case 0b1101111:
        return J_type;
    case 0b0100011:
        return S_type;
    default:
        puts("Unknown instruction type in get_instr_type!!!");
        exit(1);
    }
    return 0;
}

unsigned int bin_to_int(unsigned char *data, int from, int to)
{
    unsigned int res = 0;
    for (int i = from; i >= to; i--)
    {
        int byteOffset = i / 8;
        int bitOffset = i % 8;
        int bit = (data[byteOffset] >> bitOffset) & 1;
        res <<= 1;
        if (bit)
            res |= 1;
    }
    return res;
}

void print_bin(unsigned int val, int bits)
{
    if (bits == 0)
        return;
    print_bin(val >> 1, bits - 1);
    buffered_print(val & 1 ? "1" : "0");
}

void print_instr_R_type(unsigned char *data)
{
    buffered_print("+---------+-----+-----+--------+-----+---------+\n");
    buffered_print("|  funct7 | rs2 | rs1 | funct3 |  rd |  opcode |\n");
    buffered_print("+---------+-----+-----+--------+-----+---------+\n");
    buffered_print("| ");
    print_bin(bin_to_int(data, 31, 25), 7);
    buffered_print(" ");                                  // funct7
    buffered_print("| x%02d ", bin_to_int(data, 24, 20)); // rs2
    buffered_print("| x%02d ", bin_to_int(data, 19, 15)); // rs1
    buffered_print("|    ");
    print_bin(bin_to_int(data, 14, 12), 3);
    buffered_print(" ");                                 // funct3
    buffered_print("| x%02d ", bin_to_int(data, 11, 7)); // rd
    buffered_print("| ");
    print_bin(bin_to_int(data, 6, 0), 7);
    buffered_print(" |\n"); // opcode
    buffered_print("+---------+-----+-----+--------+-----+---------+\n");
}

void print_instr_I_type(unsigned char *data)
{
    buffered_print("+---------------+-----+--------+-----+---------+\n");
    buffered_print("|    immi[11:0] | rs1 | funct3 |  rd |  opcode |\n");
    buffered_print("+---------------+-----+--------+-----+---------+\n");
    buffered_print("|         0x%03x ", bin_to_int(data, 31, 20)); // Immi[11:0]
    buffered_print("| x%02d ", bin_to_int(data, 19, 15));          // rs1
    buffered_print("|    ");
    print_bin(bin_to_int(data, 14, 12), 3);
    buffered_print(" ");                                 // funct3
    buffered_print("| x%02d ", bin_to_int(data, 11, 7)); // rd
    buffered_print("| ");
    print_bin(bin_to_int(data, 6, 0), 7);
    buffered_print(" |\n"); // opcode
    buffered_print("+---------------+-----+--------+-----+---------+\n");
}

void print_instr_I_SHAMT_type(unsigned char *data)
{
    buffered_print("+---------+-----+-----+--------+-----+---------+\n");
    buffered_print("|  funct7 |shamt| rs1 | funct3 |  rd |  opcode |\n");
    buffered_print("+---------+-----+-----+--------+-----+---------+\n");
    buffered_print("| ");
    print_bin(bin_to_int(data, 31, 25), 7);
    buffered_print(" ");                                  // funct7
    buffered_print("| 0x%02x", bin_to_int(data, 19, 15)); // shamt
    buffered_print("| x%02d ", bin_to_int(data, 19, 15)); // rs1
    buffered_print("|    ");
    print_bin(bin_to_int(data, 14, 12), 3);
    buffered_print(" ");                                 // funct3
    buffered_print("| x%02d ", bin_to_int(data, 11, 7)); // rd
    buffered_print("| ");
    print_bin(bin_to_int(data, 6, 0), 7);
    buffered_print(" |\n"); // opcode
    buffered_print("+---------------+-----+--------+-----+---------+\n");
}

void print_instr_B_type(unsigned char *data)
{
    buffered_print("+----------+----+----+------+----------+-------+\n");
    buffered_print("|O[12|10:5]| rs2| rs1|funct3| O[4:1|11]| opcode|\n");
    buffered_print("+----------+----+----+------+----------+-------+\n");
    buffered_print("| ");
    print_bin(bin_to_int(data, 31, 31), 1); // offset[12]
    buffered_print(" ");
    print_bin(bin_to_int(data, 30, 25), 6); // offset[10:5]
    buffered_print(" | x%02d", bin_to_int(data, 24, 20)); // rs1
    buffered_print("| x%02d|  ", bin_to_int(data, 19, 15)); // rs2
    print_bin(bin_to_int(data, 14, 12), 3); // funct3
    buffered_print(" |   ");
    print_bin(bin_to_int(data, 11, 8), 4); // offset[4:1]
    buffered_print(" ");
    print_bin(bin_to_int(data, 7, 7), 1); // offset[11]
    buffered_print(" |");
    print_bin(bin_to_int(data, 6, 0), 7);
    buffered_print("|\n"); // opcode
    buffered_print("+----------+----+----+------+----------+-------+\n");
}

void print_instr_U_type(unsigned char *data) {
    buffered_print("+------------------------------+-----+---------+\n");
    buffered_print("|                  immi[31:12] |  rd |  opcode |\n");
    buffered_print("+------------------------------+-----+---------+\n");
    buffered_print("|         ");
    print_bin(bin_to_int(data, 31, 12), 20); // immi[31:12]
    buffered_print(" | x%02d | ", bin_to_int(data, 11, 7)); // rd
    print_bin(bin_to_int(data, 6, 0), 7); // opcode
    buffered_print(" |\n");
    buffered_print("+------------------------------+-----+---------+\n");
}

void print_instr_J_type(unsigned char *data) {
    buffered_print("+------------------------------+-----+---------+\n");
    buffered_print("|       immi[20|10:1|11|19:12] |  rd |  opcode |\n");
    buffered_print("+------------------------------+-----+---------+\n");
    buffered_print("|      ");
    print_bin(bin_to_int(data, 31, 31), 1); // immi[20]
    buffered_print(" ");
    print_bin(bin_to_int(data, 30, 21), 10); // immi[10:1]
    buffered_print(" ");
    print_bin(bin_to_int(data, 20, 20), 1); // immi[11]
    buffered_print(" ");
    print_bin(bin_to_int(data, 19, 12), 8); // immi[19:12]
    buffered_print(" | x%02d | ", bin_to_int(data, 11, 7));
    print_bin(bin_to_int(data, 6, 0), 7); // opcode
    buffered_print(" |\n");
    buffered_print("+------------------------------+-----+---------+\n");
}

void print_instr_S_type(unsigned char *data)
{
    buffered_print("+----------+---+---+--------+---------+--------+\n");
    buffered_print("|immi[11:5]|rs2|rs1| funct3 |immi[4:0]| opcode |\n");
    buffered_print("+----------+---+---+--------+---------+--------+\n");
    buffered_print("|  ");
    print_bin(bin_to_int(data, 31, 25), 7); // immi[11:5]
    buffered_print(" |x%02d|x%02d|    ", bin_to_int(data, 24, 20), bin_to_int(data, 19, 15)); // rs1 and rs2
    print_bin(bin_to_int(data, 14, 12), 3); // funct3
    buffered_print(" |   ");
    print_bin(bin_to_int(data, 11, 7), 5); // immi[4:0]
    buffered_print(" | ");
    print_bin(bin_to_int(data, 6, 0), 7); // opcode
    buffered_print("|\n");
    buffered_print("+----------+---+---+--------+---------+--------+\n");
}

void print_instr(instr_t *instr)
{
    switch (get_instr_type(instr->data))
    {
    case R_type:
        print_instr_R_type(instr->data);
        break;
    case I_type:
        print_instr_I_type(instr->data);
        break;
    case I_SHAMT_type:
        print_instr_I_SHAMT_type(instr->data);
        break;
    case B_type:
        print_instr_B_type(instr->data);
        break;
    case U_type:
        print_instr_U_type(instr->data);
        break;
    case J_type:
        print_instr_J_type(instr->data);
        break;
    case S_type:
        print_instr_S_type(instr->data);
        break;
    default:
        puts("Unknown instruction type in print_instr!!!");
        exit(1);
    }
}
