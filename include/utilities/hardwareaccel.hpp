#include <immintrin.h>

#include <cpuid.h>
#include <stdio.h>

/*struct HardwareAccelerationSettings
{
    string CPUName;

    #pragma region FEATURE_FLAGS
    //Features
    bool SSE3;                //  0   sse3          * NEEDED
    bool PCLMULQDQ;           //  1   pclmulqdq
    bool DTES64;              //  2   dtes64
    bool MONITOR;             //  3   monitor
    bool DS_CPI;              //  4   ds-cpi
    bool VMX;                 //  5   vmx
    bool SMX;                 //  6   smx
    bool EST;                 //  7   est
    bool TM2;                 //  8   tm2
    bool SSSE3;               //  9   ssse3         * NEEDED
    bool CNXT_ID;             //  10  cnxt-id
    bool SDBG;                //  11  sdbg
    bool FMA;                 //  12  fma
    bool CX16;                //  13  cx16
    bool XTPR;                //  14  xtpr
    bool PDCM;                //  15  pdcm
    bool PCID;                //  17  pcid
    bool DCA;                 //  18  dca
    bool SSE4_1;              //  19  sse4.1         * NEEDED
    bool SSE4_2;              //  20  sse4.2         * NEEDED
    bool X2APIC;              //  21  x2apic
    bool MOVBE;               //  22  movbe
    bool POPCNT;              //  23  popcnt
    bool TSC_DEADLINE;        //  24  tsc-deadline
    bool AES;                 //  25  aes
    bool XSAVE;               //  26  xsave
    bool OSXSAVE;             //  27  osxsave
    bool AVX;                 //  28  avx             * NEEDED
    bool F16C;                //  29  f16c
    bool RDRND;               //  30  rdrnd
    bool HYPERVISOR;
    //
    //  EDX FEATURE FLAG BREAKOUT
    //--------------------------------------------------
    bool FPU;                 //  0   fpu
    bool VME;                 //  1   vme
    bool DE;                  //  2   de
    bool PSE;                 //  3   pse
    bool TSC;                 //  4   tsc
    bool MSR;                 //  5   msr
    bool PAE;                 //  6   pae
    bool MCE;                 //  7   mce
    bool CX6;                 //  8   cx8
    bool APIC;                //  9   apic
    bool SEP;                 //  11  sep
    bool MTRR;                //  12  mtrr
    bool PGE;                 //  13  pge
    bool MCA;                 //  14  mca
    bool CMOV;                //  15  cmov
    bool PAT;                 //  16  pat
    bool PSE_36;              //  17  pse-36
    bool PSN;                 //  18  psn
    bool CLFSH;               //  19  clfsh
    bool DS;                  //  21  ds
    bool ACPI;                //  22  acpi   
    bool MMX;                 //  23  mmx           * NEEDED
    bool FXSR;                //  24  fxsr
    bool SSE;                 //  25  sse           * NEEDED
    bool SSE2;                //  26  sse2          * NEEDED
    bool SS;                  //  27  ss
    bool HTT;                 //  28  htt
    bool TM;                  //  29  tm
    bool IA64;                //  30  ia64
    bool PBE;                 //  31  pbe
    #pragma endregion

    // EXTENDED FEATURES 1
    #pragma region EXTENDED_FEATURES_1
    bool FSGSBASE;
    bool IA32_TSC_ADJUST_MSR;
    bool SGX;
    bool BMI1;
    bool HLE;
    bool AVX2;
    bool FDP_EXCPTN_ONLY;
    bool SMEP;
    bool BMI2;
    bool ERMS;
    bool INVPCID;
    bool RTM;
    bool RDT_M_PQM;
    bool FPU_CS_FPU_DS_DEP;
    bool MPX;
    bool RDT_A_PQE;
    bool AVX512_F;
    bool AVX512_DQ;
    bool RDSEED;
    bool ADX;
    bool SMAP;
    bool AVX512_IFMA;
    bool CLFLUSHOPT;
    bool CLWB;
    bool PT;
    bool AVX512_PF;
    bool AVX512_ER;
    bool AVX512_CD;
    bool SHA;
    bool AVX512_BW;
    bool AVX512_VL;

    bool PREFETCHWT1;
    bool AVX512_VBMI;
    bool UMIP;
    bool PKU;
    bool OSPKE;
    bool WAITPKG;
    bool AVX512_VBMI2;
    bool CET_SS;
    bool GFNI;
    bool VAES;
    bool VPCLMULQDQ;
    bool AVX512_VNNI;
    bool AVX512_BITALG;
    bool TME;
    bool AVX512_VPOPCNTDQ;
    bool IA57;
    bool MAWAU;
    bool RDPID;
    bool KL;
    bool BUS_LOCK_DETECT;
    bool MOVDIRI;
    bool MOVDIR64B;
    bool ENQCMD;
    bool SGX_LC;
    bool PKS;

    bool SGX_KEYS;
    bool AVX512_4VNNIW;
    bool AVX512_4FMAPS;
    bool FSRM;
    bool UINTR;
    bool AVX512_VP2INTERSECT;
    bool SRDBS_CTRL;
    bool MC_CLEAR;
    bool RTM_ALWAYS_ABORT;
    bool TSX_FORCE_ABORT_MSR;
    bool SERIALIZE;
    bool HYBRID;
    bool TSXLDTRK;
    bool PCONFIG;
    bool LBR;
    bool CETIBT;
    bool AMX_BF16;
    bool AVX512_FP16;
    bool AMX_TILE;
    bool AMX_INT8;
    bool IBRS_IBPB_SPEC_CTRL;
    bool STIBP;
    bool L1D_FLUSH;
    bool IA32_ARCH_CAPABILITIES;
    bool IA32_CORE_CAPABILITIES_MSR;
    bool SSBD;
    #pragma endregion
    
    // EXTENDED FEATURES 2
    #pragma region EXTENDED_FEATURES_2
    bool RAO_INT;
    bool AVX_VNNI;
    bool AVX512_BF16;
    bool LASS;
    bool CMPCCXADD;
    bool ARCHPERFMONEXT;
    bool FAST_ZERO_REP_MOVSB;
    bool FAST_SHORT_REP_STOSB;
    bool FAST_SHORT_REP_CMPSB_SCASB;
    bool FRED;
    bool LKGS;
    bool WRMSRNS;
    bool AMX_FP16;
    bool HRESET;
    bool AVX_IFMA;
    bool LAM;
    bool MSRLIST;

    bool IA32_PPIN_IA32_PPIN_CTL_MSR;
    
    bool AVX_VNN_INT8;
    bool AVX_NE_CONVERT;
    bool PREFETCHITI;
    bool CET_SSS;
    
    #pragma endregion

};

bool getHardwareAccelerationSettings(HardwareAccelerationSettings* updatable)
{
    // Constants
    unsigned int vals[32] = {
        (0x0001<<0),
        (0x0001<<1),
        (0x0001<<2),
        (0x0001<<3),
        (0x0001<<4),
        (0x0001<<5),
        (0x0001<<6),
        (0x0001<<7),
        (0x0001<<8),
        (0x0001<<9),
        (0x0001<<10),
        (0x0001<<11),
        (0x0001<<12),
        (0x0001<<13),
        (0x0001<<14),
        (0x0001<<15),
        (0x0001<<16),
        (0x0001<<17),
        (0x0001<<18),
        (0x0001<<19),
        (0x0001<<20),
        (0x0001<<21),
        (0x0001<<22),
        (0x0001<<23),
        (0x0001<<24),
        (0x0001<<25),
        (0x0001<<26),
        (0x0001<<27),
        (0x0001<<28),
        (0x0001<<29),
        (0x0001<<30),
        ((unsigned int)(0x0001<<31)),
    };


    // Data we're gathering
    //--------------------------------------------------
    int vendor[3];
    int func=0;

    unsigned int model_family_additional[4];
    bool model_family_additional_parsed[2][32];

    unsigned int extended[2][3];
    bool extended_parsed[6][32];

    // CPUID Data requests
    //--------------------------------------------------
    #pragma region data_requests

    // Get CPU Name
    __asm__("mov $0, %eax\n\t");
    __asm__("cpuid\n\t");

    __asm__("mov %%ebx, %0\n\t":"=r" (vendor[0]));
    __asm__("mov %%edx, %0\n\t":"=r" (vendor[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (vendor[2]));

    // Get CPU Func Supported
    __asm__("mov %%eax, %0\n\t":"=r" (func));

    // Model & Family
    __asm__("mov $1, %eax\n\t");
    __asm__("cpuid\n\t");
    
    __asm__("mov %%eax, %0\n\t":"=r" (model_family_additional[0])); 
    __asm__("mov %%ebx, %0\n\t":"=r" (model_family_additional[1]));
    __asm__("mov %%ecx, %0\n\t":"=r" (model_family_additional[2]));
    __asm__("mov %%edx, %0\n\t":"=r" (model_family_additional[3]));

    //  Extended Features
    
    __asm__("mov $7, %eax\n\t");
    __asm__("mov $0, %ecx\n\t");
    __asm__("cpuid\n\t");
    
    __asm__("mov %%ebx, %0\n\t":"=r" (extended[0][0]));
    __asm__("mov %%ecx, %0\n\t":"=r" (extended[0][1]));
    __asm__("mov %%edx, %0\n\t":"=r" (extended[0][2]));

    // Extended features set 2
    __asm__("mov $7, %eax\n\t");
    __asm__("mov $1, %ecx\n\t");
    __asm__("cpuid\n\t");

    __asm__("mov %%ebx, %0\n\t":"=r" (extended[1][0]));
    __asm__("mov %%ecx, %0\n\t":"=r" (extended[1][1]));
    __asm__("mov %%edx, %0\n\t":"=r" (extended[1][2]));

    #pragma endregion

    // Data Parsing
    //--------------------------------------------------
    #pragma region asm_data_parsing
    
    updatable->CPUName=((char*)vendor); 
    updatable->CPUName.resize(12);

    for(int i=0; i<32; i++)
    {
        //cout<<(vals[i] & model_family_additional[2])<<endl;
        model_family_additional_parsed[0][i]= ((vals[i]&model_family_additional[2]) > 0);
        model_family_additional_parsed[1][i]= ((vals[i]&model_family_additional[3]) > 0);
    }

    int i=0;
    int j=0;
    
    //  Feature Flags ECX & EDX Breakout
    //
    //  ECX FEATURE FLAG BREAKOUT
    //--------------------------------------------------
    updatable->SSE3 = model_family_additional_parsed[i][j]; j++;                //  0   sse3
    updatable->PCLMULQDQ = model_family_additional_parsed[i][j]; j++;           //  1   pclmulqdq
    updatable->DTES64 = model_family_additional_parsed[i][j]; j++;              //  2   dtes64
    updatable->MONITOR = model_family_additional_parsed[i][j]; j++;             //  3   monitor
    updatable->DS_CPI = model_family_additional_parsed[i][j]; j++;              //  4   ds-cpi
    updatable->VMX = model_family_additional_parsed[i][j]; j++;                 //  5   vmx
    updatable->SMX = model_family_additional_parsed[i][j]; j++;                 //  6   smx
    updatable->EST = model_family_additional_parsed[i][j]; j++;                 //  7   est
    updatable->TM2 = model_family_additional_parsed[i][j]; j++;                 //  8   tm2
    updatable->SSSE3 = model_family_additional_parsed[i][j]; j++;               //  9   ssse3
    updatable->CNXT_ID = model_family_additional_parsed[i][j]; j++;             //  10  cnxt-id
    updatable->SDBG = model_family_additional_parsed[i][j]; j++;                //  11  sdbg
    updatable->FMA = model_family_additional_parsed[i][j]; j++;                 //  12  fma
    updatable->CX16 = model_family_additional_parsed[i][j]; j++;                //  13  cx16
    updatable->XTPR = model_family_additional_parsed[i][j]; j++;                //  14  xtpr
    updatable->PDCM = model_family_additional_parsed[i][j]; j++;                //  15  pdcm
    j++;//updatable->RESERVED1 = model_family_additional_parsed[i][j]; j++;           //  16  Reserved
    updatable->PCID = model_family_additional_parsed[i][j]; j++;                //  17  pcid
    updatable->DCA = model_family_additional_parsed[i][j]; j++;                 //  18  dca
    updatable->SSE4_1 = model_family_additional_parsed[i][j]; j++;              //  19  sse4.1
    updatable->SSE4_2 = model_family_additional_parsed[i][j]; j++;              //  20  sse4.2
    updatable->X2APIC = model_family_additional_parsed[i][j]; j++;              //  21  x2apic
    updatable->MOVBE = model_family_additional_parsed[i][j]; j++;               //  22  movbe
    updatable->POPCNT = model_family_additional_parsed[i][j]; j++;              //  23  popcnt
    updatable->TSC_DEADLINE = model_family_additional_parsed[i][j]; j++;        //  24  tsc-deadline
    updatable->AES = model_family_additional_parsed[i][j]; j++;                 //  25  aes
    updatable->XSAVE = model_family_additional_parsed[i][j]; j++;               //  26  xsave
    updatable->OSXSAVE = model_family_additional_parsed[i][j]; j++;             //  27  osxsave
    updatable->AVX = model_family_additional_parsed[i][j]; j++;                 //  28  avx
    updatable->F16C = model_family_additional_parsed[i][j]; j++;                //  29  f16c
    updatable->RDRND = model_family_additional_parsed[i][j]; j++;               //  30  rdrnd
    updatable->HYPERVISOR = model_family_additional_parsed[i][j]; j=0; i++;     //  31  hypervisor
    //
    //  EDX FEATURE FLAG BREAKOUT
    //--------------------------------------------------
    updatable->FPU = model_family_additional_parsed[i][j]; j++;                 //  0   fpu
    updatable->VME = model_family_additional_parsed[i][j]; j++;                 //  1   vme
    updatable->DE = model_family_additional_parsed[i][j]; j++;                  //  2   de
    updatable->PSE = model_family_additional_parsed[i][j]; j++;                 //  3   pse
    updatable->TSC = model_family_additional_parsed[i][j]; j++;                 //  4   tsc
    updatable->MSR = model_family_additional_parsed[i][j]; j++;                 //  5   msr
    updatable->PAE = model_family_additional_parsed[i][j]; j++;                 //  6   pae
    updatable->MCE = model_family_additional_parsed[i][j]; j++;                 //  7   mce
    updatable->CX6 = model_family_additional_parsed[i][j]; j++;                 //  8   cx8
    updatable->APIC = model_family_additional_parsed[i][j]; j++;                //  9   apic
    j++;//updatable->RESERVED2 = model_family_additional_parsed[i][j]; j++;           //  10  RESERVED
    updatable->SEP = model_family_additional_parsed[i][j]; j++;                 //  11  sep
    updatable->MTRR = model_family_additional_parsed[i][j]; j++;                //  12  mtrr
    updatable->PGE = model_family_additional_parsed[i][j]; j++;                 //  13  pge
    updatable->MCA = model_family_additional_parsed[i][j]; j++;                 //  14  mca
    updatable->CMOV = model_family_additional_parsed[i][j]; j++;                //  15  cmov
    updatable->PAT = model_family_additional_parsed[i][j]; j++;                 //  16  pat
    updatable->PSE_36 = model_family_additional_parsed[i][j]; j++;              //  17  pse-36
    updatable->PSN = model_family_additional_parsed[i][j]; j++;                 //  18  psn
    updatable->CLFSH = model_family_additional_parsed[i][j]; j++;               //  19  clfsh
    j++;//updatable->RESERVED3 = model_family_additional_parsed[i][j]; j++;           //  20  RESERVED
    updatable->DS = model_family_additional_parsed[i][j]; j++;                  //  21  ds
    updatable->ACPI = model_family_additional_parsed[i][j]; j++;                //  22  acpi   
    updatable->MMX = model_family_additional_parsed[i][j]; j++;                 //  23  mmx
    updatable->FXSR = model_family_additional_parsed[i][j]; j++;                //  24  fxsr
    updatable->SSE = model_family_additional_parsed[i][j]; j++;                 //  25  sse
    updatable->SSE2 = model_family_additional_parsed[i][j]; j++;                //  26  sse2
    updatable->SS = model_family_additional_parsed[i][j]; j++;                  //  27  ss
    updatable->HTT = model_family_additional_parsed[i][j]; j++;                 //  28  htt
    updatable->TM = model_family_additional_parsed[i][j]; j++;                  //  29  tm
    updatable->IA64 = model_family_additional_parsed[i][j]; j++;                //  30  ia64
    updatable->PBE = model_family_additional_parsed[i][j]; j++;                 //  31  pbe


    for(int i=0; i<32; i++)
    {
        extended_parsed[0][i]= ((vals[i]&extended[0][0]) > 0);
        extended_parsed[1][i]= ((vals[i]&extended[0][1]) > 0);
        extended_parsed[2][i]= ((vals[i]&extended[0][2]) > 0);

        extended_parsed[3][i]= ((vals[i]&extended[1][0]) > 0);
        extended_parsed[4][i]= ((vals[i]&extended[1][1]) > 0);
        extended_parsed[5][i]= ((vals[i]&extended[1][2]) > 0);
    }

    i=0;
    j=0;

    //updatable-> = extended_parsed[i][j]; j++;
    updatable->FSGSBASE = extended_parsed[i][j]; j++;
    updatable->IA32_TSC_ADJUST_MSR = extended_parsed[i][j]; j++;
    updatable->SGX = extended_parsed[i][j]; j++;
    updatable->BMI1 = extended_parsed[i][j]; j++;
    updatable->HLE = extended_parsed[i][j]; j++;
    updatable->AVX2 = extended_parsed[i][j]; j++;
    updatable->FDP_EXCPTN_ONLY = extended_parsed[i][j]; j++;
    updatable->SMEP = extended_parsed[i][j]; j++;
    updatable->BMI2 = extended_parsed[i][j]; j++;
    updatable->ERMS = extended_parsed[i][j]; j++;
    updatable->INVPCID = extended_parsed[i][j]; j++;
    updatable->RTM = extended_parsed[i][j]; j++;
    updatable->RDT_M_PQM = extended_parsed[i][j]; j++;
    updatable->FPU_CS_FPU_DS_DEP = extended_parsed[i][j]; j++;
    updatable->MPX = extended_parsed[i][j]; j++;
    updatable->RDT_A_PQE = extended_parsed[i][j]; j++;
    updatable->AVX512_F = extended_parsed[i][j]; j++;
    updatable->AVX512_DQ = extended_parsed[i][j]; j++;
    updatable->RDSEED = extended_parsed[i][j]; j++;
    updatable->ADX = extended_parsed[i][j]; j++;
    updatable->SMAP = extended_parsed[i][j]; j++;
    updatable->AVX512_IFMA = extended_parsed[i][j]; j++;
    j++;//updatable-> = extended_parsed[i][j]; j++;
    updatable->CLFLUSHOPT = extended_parsed[i][j]; j++;
    updatable->CLWB = extended_parsed[i][j]; j++;
    updatable->PT = extended_parsed[i][j]; j++;
    updatable->AVX512_PF = extended_parsed[i][j]; j++;
    updatable->AVX512_ER = extended_parsed[i][j]; j++;
    updatable->AVX512_CD = extended_parsed[i][j]; j++;
    updatable->SHA = extended_parsed[i][j]; j++;
    updatable->AVX512_BW = extended_parsed[i][j]; j++;
    updatable->AVX512_VL = extended_parsed[i][j]; j++;

    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;

    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;



    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;

    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;

    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;
    updatable-> = extended_parsed[i][j]; j++;


    #pragma endregion


    return true;
}
*/