/**
* Code in this file is migrated from the Qubic repository (https://github.com/qubic/qubic-cli/blob/main/K12AndKeyUtil.h)
*/


#ifndef LEDGER_APP_QUBIC_K12_H
#define LEDGER_APP_QUBIC_K12_H
#include <stdint.h>   // uint*_t


#define ROL64(a, offset) ((((unsigned long long)a) << offset) ^ (((unsigned long long)a) >> (64 - offset)))

#define KeccakF1600RoundConstant0 0x000000008000808bULL
#define KeccakF1600RoundConstant1 0x800000000000008bULL
#define KeccakF1600RoundConstant2 0x8000000000008089ULL
#define KeccakF1600RoundConstant3 0x8000000000008003ULL
#define KeccakF1600RoundConstant4 0x8000000000008002ULL
#define KeccakF1600RoundConstant5 0x8000000000000080ULL
#define KeccakF1600RoundConstant6 0x000000000000800aULL
#define KeccakF1600RoundConstant7 0x800000008000000aULL
#define KeccakF1600RoundConstant8 0x8000000080008081ULL
#define KeccakF1600RoundConstant9 0x8000000000008080ULL
#define KeccakF1600RoundConstant10 0x0000000080000001ULL

#define declareABCDE                            \
    unsigned long long Aba, Abe, Abi, Abo, Abu; \
    unsigned long long Aga, Age, Agi, Ago, Agu; \
    unsigned long long Aka, Ake, Aki, Ako, Aku; \
    unsigned long long Ama, Ame, Ami, Amo, Amu; \
    unsigned long long Asa, Ase, Asi, Aso, Asu; \
    unsigned long long Bba, Bbe, Bbi, Bbo, Bbu; \
    unsigned long long Bga, Bge, Bgi, Bgo, Bgu; \
    unsigned long long Bka, Bke, Bki, Bko, Bku; \
    unsigned long long Bma, Bme, Bmi, Bmo, Bmu; \
    unsigned long long Bsa, Bse, Bsi, Bso, Bsu; \
    unsigned long long Ca, Ce, Ci, Co, Cu;      \
    unsigned long long Da, De, Di, Do, Du;      \
    unsigned long long Eba, Ebe, Ebi, Ebo, Ebu; \
    unsigned long long Ega, Ege, Egi, Ego, Egu; \
    unsigned long long Eka, Eke, Eki, Eko, Eku; \
    unsigned long long Ema, Eme, Emi, Emo, Emu; \
    unsigned long long Esa, Ese, Esi, Eso, Esu;


#define copyFromState(state) \
    Aba = state[0];          \
    Abe = state[1];          \
    Abi = state[2];          \
    Abo = state[3];          \
    Abu = state[4];          \
    Aga = state[5];          \
    Age = state[6];          \
    Agi = state[7];          \
    Ago = state[8];          \
    Agu = state[9];          \
    Aka = state[10];         \
    Ake = state[11];         \
    Aki = state[12];         \
    Ako = state[13];         \
    Aku = state[14];         \
    Ama = state[15];         \
    Ame = state[16];         \
    Ami = state[17];         \
    Amo = state[18];         \
    Amu = state[19];         \
    Asa = state[20];         \
    Ase = state[21];         \
    Asi = state[22];         \
    Aso = state[23];         \
    Asu = state[24];

#define copyToState(state) \
    state[0] = Aba;        \
    state[1] = Abe;        \
    state[2] = Abi;        \
    state[3] = Abo;        \
    state[4] = Abu;        \
    state[5] = Aga;        \
    state[6] = Age;        \
    state[7] = Agi;        \
    state[8] = Ago;        \
    state[9] = Agu;        \
    state[10] = Aka;       \
    state[11] = Ake;       \
    state[12] = Aki;       \
    state[13] = Ako;       \
    state[14] = Aku;       \
    state[15] = Ama;       \
    state[16] = Ame;       \
    state[17] = Ami;       \
    state[18] = Amo;       \
    state[19] = Amu;       \
    state[20] = Asa;       \
    state[21] = Ase;       \
    state[22] = Asi;       \
    state[23] = Aso;       \
    state[24] = Asu;


#define thetaRhoPiChiIotaPrepareTheta(i, A, E) \
    Da = Cu ^ ROL64(Ce, 1);                    \
    De = Ca ^ ROL64(Ci, 1);                    \
    Di = Ce ^ ROL64(Co, 1);                    \
    Do = Ci ^ ROL64(Cu, 1);                    \
    Du = Co ^ ROL64(Ca, 1);                    \
    A##ba ^= Da;                               \
    Bba = A##ba;                               \
    A##ge ^= De;                               \
    Bbe = ROL64(A##ge, 44);                    \
    A##ki ^= Di;                               \
    Bbi = ROL64(A##ki, 43);                    \
    A##mo ^= Do;                               \
    Bbo = ROL64(A##mo, 21);                    \
    A##su ^= Du;                               \
    Bbu = ROL64(A##su, 14);                    \
    E##ba = Bba ^ ((~Bbe) & Bbi);              \
    E##ba ^= KeccakF1600RoundConstant##i;      \
    Ca = E##ba;                                \
    E##be = Bbe ^ ((~Bbi) & Bbo);              \
    Ce = E##be;                                \
    E##bi = Bbi ^ ((~Bbo) & Bbu);              \
    Ci = E##bi;                                \
    E##bo = Bbo ^ ((~Bbu) & Bba);              \
    Co = E##bo;                                \
    E##bu = Bbu ^ ((~Bba) & Bbe);              \
    Cu = E##bu;                                \
    A##bo ^= Do;                               \
    Bga = ROL64(A##bo, 28);                    \
    A##gu ^= Du;                               \
    Bge = ROL64(A##gu, 20);                    \
    A##ka ^= Da;                               \
    Bgi = ROL64(A##ka, 3);                     \
    A##me ^= De;                               \
    Bgo = ROL64(A##me, 45);                    \
    A##si ^= Di;                               \
    Bgu = ROL64(A##si, 61);                    \
    E##ga = Bga ^ ((~Bge) & Bgi);              \
    Ca ^= E##ga;                               \
    E##ge = Bge ^ ((~Bgi) & Bgo);              \
    Ce ^= E##ge;                               \
    E##gi = Bgi ^ ((~Bgo) & Bgu);              \
    Ci ^= E##gi;                               \
    E##go = Bgo ^ ((~Bgu) & Bga);              \
    Co ^= E##go;                               \
    E##gu = Bgu ^ ((~Bga) & Bge);              \
    Cu ^= E##gu;                               \
    A##be ^= De;                               \
    Bka = ROL64(A##be, 1);                     \
    A##gi ^= Di;                               \
    Bke = ROL64(A##gi, 6);                     \
    A##ko ^= Do;                               \
    Bki = ROL64(A##ko, 25);                    \
    A##mu ^= Du;                               \
    Bko = ROL64(A##mu, 8);                     \
    A##sa ^= Da;                               \
    Bku = ROL64(A##sa, 18);                    \
    E##ka = Bka ^ ((~Bke) & Bki);              \
    Ca ^= E##ka;                               \
    E##ke = Bke ^ ((~Bki) & Bko);              \
    Ce ^= E##ke;                               \
    E##ki = Bki ^ ((~Bko) & Bku);              \
    Ci ^= E##ki;                               \
    E##ko = Bko ^ ((~Bku) & Bka);              \
    Co ^= E##ko;                               \
    E##ku = Bku ^ ((~Bka) & Bke);              \
    Cu ^= E##ku;                               \
    A##bu ^= Du;                               \
    Bma = ROL64(A##bu, 27);                    \
    A##ga ^= Da;                               \
    Bme = ROL64(A##ga, 36);                    \
    A##ke ^= De;                               \
    Bmi = ROL64(A##ke, 10);                    \
    A##mi ^= Di;                               \
    Bmo = ROL64(A##mi, 15);                    \
    A##so ^= Do;                               \
    Bmu = ROL64(A##so, 56);                    \
    E##ma = Bma ^ ((~Bme) & Bmi);              \
    Ca ^= E##ma;                               \
    E##me = Bme ^ ((~Bmi) & Bmo);              \
    Ce ^= E##me;                               \
    E##mi = Bmi ^ ((~Bmo) & Bmu);              \
    Ci ^= E##mi;                               \
    E##mo = Bmo ^ ((~Bmu) & Bma);              \
    Co ^= E##mo;                               \
    E##mu = Bmu ^ ((~Bma) & Bme);              \
    Cu ^= E##mu;                               \
    A##bi ^= Di;                               \
    Bsa = ROL64(A##bi, 62);                    \
    A##go ^= Do;                               \
    Bse = ROL64(A##go, 55);                    \
    A##ku ^= Du;                               \
    Bsi = ROL64(A##ku, 39);                    \
    A##ma ^= Da;                               \
    Bso = ROL64(A##ma, 41);                    \
    A##se ^= De;                               \
    Bsu = ROL64(A##se, 2);                     \
    E##sa = Bsa ^ ((~Bse) & Bsi);              \
    Ca ^= E##sa;                               \
    E##se = Bse ^ ((~Bsi) & Bso);              \
    Ce ^= E##se;                               \
    E##si = Bsi ^ ((~Bso) & Bsu);              \
    Ci ^= E##si;                               \
    E##so = Bso ^ ((~Bsu) & Bsa);              \
    Co ^= E##so;                               \
    E##su = Bsu ^ ((~Bsa) & Bse);              \
    Cu ^= E##su;

#define rounds12                                                                    \
    Ca = Aba ^ Aga ^ Aka ^ Ama ^ Asa;                                               \
    Ce = Abe ^ Age ^ Ake ^ Ame ^ Ase;                                               \
    Ci = Abi ^ Agi ^ Aki ^ Ami ^ Asi;                                               \
    Co = Abo ^ Ago ^ Ako ^ Amo ^ Aso;                                               \
    Cu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;                                               \
    thetaRhoPiChiIotaPrepareTheta(0, A, E)                                          \
        thetaRhoPiChiIotaPrepareTheta(1, E, A)                                      \
            thetaRhoPiChiIotaPrepareTheta(2, A, E)                                  \
                thetaRhoPiChiIotaPrepareTheta(3, E, A)                              \
                    thetaRhoPiChiIotaPrepareTheta(4, A, E)                          \
                        thetaRhoPiChiIotaPrepareTheta(5, E, A)                      \
                            thetaRhoPiChiIotaPrepareTheta(6, A, E)                  \
                                thetaRhoPiChiIotaPrepareTheta(7, E, A)              \
                                    thetaRhoPiChiIotaPrepareTheta(8, A, E)          \
                                        thetaRhoPiChiIotaPrepareTheta(9, E, A)      \
                                            thetaRhoPiChiIotaPrepareTheta(10, A, E) \
                                                Da = Cu ^ ROL64(Ce, 1);             \
    De = Ca ^ ROL64(Ci, 1);                                                         \
    Di = Ce ^ ROL64(Co, 1);                                                         \
    Do = Ci ^ ROL64(Cu, 1);                                                         \
    Du = Co ^ ROL64(Ca, 1);                                                         \
    Eba ^= Da;                                                                      \
    Bba = Eba;                                                                      \
    Ege ^= De;                                                                      \
    Bbe = ROL64(Ege, 44);                                                           \
    Eki ^= Di;                                                                      \
    Bbi = ROL64(Eki, 43);                                                           \
    Emo ^= Do;                                                                      \
    Bbo = ROL64(Emo, 21);                                                           \
    Esu ^= Du;                                                                      \
    Bbu = ROL64(Esu, 14);                                                           \
    Aba = Bba ^ ((~Bbe) & Bbi);                                                     \
    Aba ^= 0x8000000080008008ULL;                                                   \
    Abe = Bbe ^ ((~Bbi) & Bbo);                                                     \
    Abi = Bbi ^ ((~Bbo) & Bbu);                                                     \
    Abo = Bbo ^ ((~Bbu) & Bba);                                                     \
    Abu = Bbu ^ ((~Bba) & Bbe);                                                     \
    Ebo ^= Do;                                                                      \
    Bga = ROL64(Ebo, 28);                                                           \
    Egu ^= Du;                                                                      \
    Bge = ROL64(Egu, 20);                                                           \
    Eka ^= Da;                                                                      \
    Bgi = ROL64(Eka, 3);                                                            \
    Eme ^= De;                                                                      \
    Bgo = ROL64(Eme, 45);                                                           \
    Esi ^= Di;                                                                      \
    Bgu = ROL64(Esi, 61);                                                           \
    Aga = Bga ^ ((~Bge) & Bgi);                                                     \
    Age = Bge ^ ((~Bgi) & Bgo);                                                     \
    Agi = Bgi ^ ((~Bgo) & Bgu);                                                     \
    Ago = Bgo ^ ((~Bgu) & Bga);                                                     \
    Agu = Bgu ^ ((~Bga) & Bge);                                                     \
    Ebe ^= De;                                                                      \
    Bka = ROL64(Ebe, 1);                                                            \
    Egi ^= Di;                                                                      \
    Bke = ROL64(Egi, 6);                                                            \
    Eko ^= Do;                                                                      \
    Bki = ROL64(Eko, 25);                                                           \
    Emu ^= Du;                                                                      \
    Bko = ROL64(Emu, 8);                                                            \
    Esa ^= Da;                                                                      \
    Bku = ROL64(Esa, 18);                                                           \
    Aka = Bka ^ ((~Bke) & Bki);                                                     \
    Ake = Bke ^ ((~Bki) & Bko);                                                     \
    Aki = Bki ^ ((~Bko) & Bku);                                                     \
    Ako = Bko ^ ((~Bku) & Bka);                                                     \
    Aku = Bku ^ ((~Bka) & Bke);                                                     \
    Ebu ^= Du;                                                                      \
    Bma = ROL64(Ebu, 27);                                                           \
    Ega ^= Da;                                                                      \
    Bme = ROL64(Ega, 36);                                                           \
    Eke ^= De;                                                                      \
    Bmi = ROL64(Eke, 10);                                                           \
    Emi ^= Di;                                                                      \
    Bmo = ROL64(Emi, 15);                                                           \
    Eso ^= Do;                                                                      \
    Bmu = ROL64(Eso, 56);                                                           \
    Ama = Bma ^ ((~Bme) & Bmi);                                                     \
    Ame = Bme ^ ((~Bmi) & Bmo);                                                     \
    Ami = Bmi ^ ((~Bmo) & Bmu);                                                     \
    Amo = Bmo ^ ((~Bmu) & Bma);                                                     \
    Amu = Bmu ^ ((~Bma) & Bme);                                                     \
    Ebi ^= Di;                                                                      \
    Bsa = ROL64(Ebi, 62);                                                           \
    Ego ^= Do;                                                                      \
    Bse = ROL64(Ego, 55);                                                           \
    Eku ^= Du;                                                                      \
    Bsi = ROL64(Eku, 39);                                                           \
    Ema ^= Da;                                                                      \
    Bso = ROL64(Ema, 41);                                                           \
    Ese ^= De;                                                                      \
    Bsu = ROL64(Ese, 2);                                                            \
    Asa = Bsa ^ ((~Bse) & Bsi);                                                     \
    Ase = Bse ^ ((~Bsi) & Bso);                                                     \
    Asi = Bsi ^ ((~Bso) & Bsu);                                                     \
    Aso = Bso ^ ((~Bsu) & Bsa);                                                     \
    Asu = Bsu ^ ((~Bsa) & Bse);

#define K12_security 128
#define K12_capacity (2 * K12_security)
#define K12_capacityInBytes (K12_capacity / 8)
#define K12_rateInBytes ((1600 - K12_capacity) / 8)
#define K12_chunkSize 8192
#define K12_suffixLeaf 0x0B

typedef struct {
    uint8_t state[200];
    uint8_t byteIOIndex;
} KangarooTwelve_F;

void kangaroo_twelve(const uint8_t *input, unsigned int inputByteLen, uint8_t *output, unsigned int outputByteLen);

#endif  // LEDGER_APP_QUBIC_K12_H
