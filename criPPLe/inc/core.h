// Process Protection
#define PS_PROT_OFFSET 0x87a

typedef struct _PS_PROT {

    UCHAR Level;
    UCHAR Type   : 3;   // Pos 0 (3 Bits)
    UCHAR Audit  : 1;   // Pos 3 (1 Bits)
    UCHAR Signer : 4;   // Pos 4 (4 Bits)

} PS_PROT, *PPS_PROT;

// Target Process

typedef struct _TARGET {

    int pid;
    
} TARGET, *PTARGET;

// Test struct for first IOCTL

typedef struct _TEST_DATA {

    int data;

} TEST_DATA, *PTEST_DATA;

// Test struct for IOCTL response

typedef struct _TEST_RESP {

    int data;
    
} TEST_RESP, *PTEST_RESP;