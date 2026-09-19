#include <stdio.h>
#include <string.h>

int main(void)
{
    FILE *input, *output;

    char line[256];
    char opcode[20];
    char operand[20];

    int opcode_value;
    int operand_value;
    int sum = 0;
    int count = 0;
    int checksum;

    input = fopen("program.asm", "r");
    if (input == NULL)
    {
        printf("Error: Cannot open program.asm\n");
        return 1;
    }

    output = fopen("program.hex", "w");
    if (output == NULL)
    {
        printf("Error: Cannot create program.hex\n");
        fclose(input);
        return 1;
    }

    fprintf(output, ":00000000");
    while (fgets(line, sizeof(line), input) != NULL)
    {
        opcode[0] = '\0';
        operand[0] = '\0';

        sscanf(line, "%19s %19s", opcode, operand);

        if (strcmp(opcode, "LDI") == 0) opcode_value = 0x01;
        else if (strcmp(opcode, "ADD") == 0) opcode_value = 0x02;
        else if (strcmp(opcode, "SUB") == 0) opcode_value = 0x03;
        else if (strcmp(opcode, "OUT") == 0) opcode_value = 0x08;
        else if (strcmp(opcode, "HLT") == 0) opcode_value = 0xFF;
        else {
            printf("Error: Invalid instruction code found: %s\n", opcode);
            fclose(input);
            fclose(output);
            // remove("program.hex");
            return 1;
        }

        if (operand[0] == '\0') operand_value = 0x00;
        else if (sscanf(operand, "%x", &operand_value) != 1) operand_value = 0x00;

        operand_value &= 0xFF;

        sum += opcode_value;
        count++;
        sum += operand_value;
        count++;

        fprintf(output, "%02X%02X", opcode_value, operand_value);
    }

    count &= 0xFF;
    sum += count;
    sum &= 0xFF;
    checksum = (~sum + 1) & 0xFF;
    fprintf(output, "%02X\n", checksum);

    fseek(output, 1, SEEK_SET);
    fprintf(output, "%02X", count);

    fseek(output, 0, SEEK_END);
    fprintf(output, ":00000001FF");

    fclose(input);
    fclose(output);

    printf("Assembly successful\n");

    return 0;
}