#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <netinet/in.h>

void usage() 
{
    printf(
        "usage: romdump FILENAME\n"
        "\n"
    );
}

typedef union 
{
    struct {
        uint8_t op:6;
        union {
            struct {
                uint8_t rs:5;
                uint8_t rt:5;
                union {
                    struct {
                        uint16_t immediate;
                    };
                    struct {
                        uint8_t rd:5;
                        uint8_t sa:5;
                        uint8_t funct:6;
                    };
                };
            };
            struct {
                uint32_t target:26;
            };
        };
    };
    uint32_t data;
} instruction_t;

enum {
    OP_SPECIAL = 0b000000,
    //OP_        = 0b000001,
    OP_JUMP    = 0b000010,
    OP_JAL     = 0b000011,
    OP_BEQ     = 0b000100,
    OP_BNE     = 0b000101,
    OP_BLEZ    = 0b000110,
    OP_BGTZ    = 0b000111,
    OP_ADDI    = 0b001000,
    OP_ADDIU   = 0b001001,
    OP_SLTI    = 0b001010,
    OP_SLTIU   = 0b001011,
    OP_ANDI    = 0b001100,
    OP_ORI     = 0b001101,
    OP_XORI    = 0b001110,
    OP_LUI     = 0b001111,
    OP_COP0    = 0b010000,
    OP_COP1    = 0b010001,
    //OP_        = 0b010010,
    //OP_        = 0b010011,
    OP_BEQL    = 0b010100,
    OP_BNEL    = 0b010101,
    OP_BGTZL   = 0b010111,
    OP_DADDI   = 0b011000,
    OP_DADDIU  = 0b011001,
    OP_LWL     = 0b100010,
    OP_LHU     = 0b100101,
    OP_SDR     = 0b101101,
    OP_CACHE   = 0b101111,
    OP_LLD     = 0b110100,
    OP_LL      = 0b110000,
    OP_SCD     = 0b111100,
    OP_SD      = 0b111111,
};

enum {
    SOP_SLL    = 0b000000,
    SOP_DSUBU  = 0b101111,
    SOP_DSLL32 = 0b111100,
};

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return 0;
    }

    const char * filename = argv[1];
    FILE * file = fopen(filename, "rb");

    char header[64];
    size_t bytesRead = 0;
    instruction_t inst;

    bytesRead = fread(header, 1, sizeof(header), file);
    if (bytesRead < sizeof(header)) {
        printf("failed to read z64 header\n");
        return 1;
    }

    printf("Dumping %.*s\n", 20, header + 32);

    int i = 0;
    int limit = 50;

    do {
        bytesRead = fread((void *)&inst, 1, sizeof(inst), file);
        inst.data = htonl(inst.data);

        switch (inst.op) {
        case OP_SPECIAL:
            switch (inst.funct) {
            case SOP_SLL:
                printf("sll\trt=%d\trd=%d\tsa=%d\n", inst.rt, inst.rd, inst.sa);
                break;
            case SOP_DSUBU:
                printf("dsubu\trs=%d\trt=%d\trd=%d\n", inst.rs, inst.rt, inst.rd);
                break;
            case SOP_DSLL32:
                printf("dsll32\trt=%d\trd=%d\tsa=%d\n", inst.rt, inst.rd, inst.sa);
                break;
            default:
                printf("special\t%d %d %d %d %d %d\n", inst.op, inst.rs, inst.rt, inst.rd, inst.sa, inst.funct);
            }
            break;
        case OP_JUMP:
            printf("j\ttarget=%d\n", inst.target);
            break;
        case OP_JAL:
            printf("jal\ttarget=%d\n", inst.target);
            break;
        case OP_BEQ:
            printf("beq\trs=%d\trt=%d\n", inst.rs, inst.rt);
            break;
        case OP_ADDI:
            printf("addi\trs=%d\trt=%d\timmediate=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_ADDIU:
            printf("addiu\trs=%d\trt=%d\timmediate=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_ANDI:
            printf("andi\trs=%d\trt=%d\timmediate=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_XORI:
            printf("xori\trs=%d\trt=%d\timmediate=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_BNEL:
            printf("bnel\trs=%d\trt=%d\timmediate=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_LWL:
            printf("lwl\tbase=%d\trt=%d\toffset=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_LHU:
            printf("lhu\tbase=%d\trt=%d\toffset=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_SDR:
            printf("sdr\tbase=%d\trt=%d\toffset=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_CACHE:
            printf("cache\tbase=%d\trt=%d\toffset=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_LLD:
            printf("lld\tbase=%d\trt=%d\toffset=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_LL:
            printf("ll\tbase=%d\trt=%d\toffset=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_SCD:
            printf("scd\tbase=%d\trt=%d\toffset=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        case OP_SD:
            printf("sd\tbase=%d\trt=%d\toffset=%d\n", inst.rs, inst.rt, inst.immediate);
            break;
        default:
            printf("unhandled %d\n", inst.op);
        };

        //printf("\tI: %d %d %d %d\n", inst.op, inst.rs, inst.rt, inst.immediate);
        //printf("\tJ: %d %d\n", inst.op, inst.target);
        //printf("\tR: %d %d %d %d %d %d\n", inst.op, inst.rs, inst.rt, inst.rd, inst.sa, inst.funct);
        
        i++;
        if (i >= limit) {
            break;
        }
    } while (bytesRead == sizeof(inst));

    fclose(file);

    return 0;
}