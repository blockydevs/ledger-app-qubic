/**
 * Code in this file is migrated from the Qubic repository (https://github.com/qubic/qubic-cli/blob/main/K12AndKeyUtil.h)
*/

#include "k12.h"
#include <string.h>   // memset, explicit_bzero

static void keccak_p1600_permute_12rounds(uint8_t *state) {
    declareABCDE
    unsigned long long *stateAsLanes = (unsigned long long *) state;
    copyFromState(stateAsLanes)
    rounds12
    copyToState(stateAsLanes)
}

static void kangaroo_twelve_f_absorb(KangarooTwelve_F *instance, const uint8_t *data, unsigned long long dataByteLen) {
    unsigned long long i = 0;
    while (i < dataByteLen) {
        if (!instance->byteIOIndex && dataByteLen >= i + K12_rateInBytes) {
            declareABCDE
            unsigned long long *stateAsLanes = (unsigned long long *) instance->state;
            copyFromState(stateAsLanes)
            unsigned long long modifiedDataByteLen = dataByteLen - i;
            while (modifiedDataByteLen >= K12_rateInBytes) {
                Aba ^= ((unsigned long long *) data)[0];
                Abe ^= ((unsigned long long *) data)[1];
                Abi ^= ((unsigned long long *) data)[2];
                Abo ^= ((unsigned long long *) data)[3];
                Abu ^= ((unsigned long long *) data)[4];
                Aga ^= ((unsigned long long *) data)[5];
                Age ^= ((unsigned long long *) data)[6];
                Agi ^= ((unsigned long long *) data)[7];
                Ago ^= ((unsigned long long *) data)[8];
                Agu ^= ((unsigned long long *) data)[9];
                Aka ^= ((unsigned long long *) data)[10];
                Ake ^= ((unsigned long long *) data)[11];
                Aki ^= ((unsigned long long *) data)[12];
                Ako ^= ((unsigned long long *) data)[13];
                Aku ^= ((unsigned long long *) data)[14];
                Ama ^= ((unsigned long long *) data)[15];
                Ame ^= ((unsigned long long *) data)[16];
                Ami ^= ((unsigned long long *) data)[17];
                Amo ^= ((unsigned long long *) data)[18];
                Amu ^= ((unsigned long long *) data)[19];
                Asa ^= ((unsigned long long *) data)[20];
                rounds12
                data += K12_rateInBytes;
                modifiedDataByteLen -= K12_rateInBytes;
            }
            copyToState(stateAsLanes)
            i = dataByteLen - modifiedDataByteLen;
        } else {
            uint8_t partialBlock;
            if ((dataByteLen - i) + instance->byteIOIndex > K12_rateInBytes) {
                partialBlock = K12_rateInBytes - instance->byteIOIndex;
            } else {
                partialBlock = (uint8_t) (dataByteLen - i);
            }
            i += partialBlock;

            if (!instance->byteIOIndex) {
                unsigned int j = 0;
                for (; (j + 8) <= (unsigned int) (partialBlock >> 3); j += 8) {
                    ((unsigned long long *) instance->state)[j + 0] ^= ((unsigned long long *) data)[j + 0];
                    ((unsigned long long *) instance->state)[j + 1] ^= ((unsigned long long *) data)[j + 1];
                    ((unsigned long long *) instance->state)[j + 2] ^= ((unsigned long long *) data)[j + 2];
                    ((unsigned long long *) instance->state)[j + 3] ^= ((unsigned long long *) data)[j + 3];
                    ((unsigned long long *) instance->state)[j + 4] ^= ((unsigned long long *) data)[j + 4];
                    ((unsigned long long *) instance->state)[j + 5] ^= ((unsigned long long *) data)[j + 5];
                    ((unsigned long long *) instance->state)[j + 6] ^= ((unsigned long long *) data)[j + 6];
                    ((unsigned long long *) instance->state)[j + 7] ^= ((unsigned long long *) data)[j + 7];
                }
                for (; (j + 4) <= (unsigned int) (partialBlock >> 3); j += 4) {
                    ((unsigned long long *) instance->state)[j + 0] ^= ((unsigned long long *) data)[j + 0];
                    ((unsigned long long *) instance->state)[j + 1] ^= ((unsigned long long *) data)[j + 1];
                    ((unsigned long long *) instance->state)[j + 2] ^= ((unsigned long long *) data)[j + 2];
                    ((unsigned long long *) instance->state)[j + 3] ^= ((unsigned long long *) data)[j + 3];
                }
                for (; (j + 2) <= (unsigned int) (partialBlock >> 3); j += 2) {
                    ((unsigned long long *) instance->state)[j + 0] ^= ((unsigned long long *) data)[j + 0];
                    ((unsigned long long *) instance->state)[j + 1] ^= ((unsigned long long *) data)[j + 1];
                }
                if (j < (unsigned int) (partialBlock >> 3)) {
                    ((unsigned long long *) instance->state)[j + 0] ^= ((unsigned long long *) data)[j + 0];
                }
                if (partialBlock & 7) {
                    unsigned long long lane = 0;
                    memcpy(&lane, data + (partialBlock & 0xFFFFFFF8), partialBlock & 7);
                    ((unsigned long long *) instance->state)[partialBlock >> 3] ^= lane;
                }
            } else {
                unsigned int _sizeLeft = partialBlock;
                unsigned int _lanePosition = instance->byteIOIndex >> 3;
                unsigned int _offsetInLane = instance->byteIOIndex & 7;
                const uint8_t *_curData = data;
                while (_sizeLeft > 0) {
                    unsigned int _bytesInLane = 8 - _offsetInLane;
                    if (_bytesInLane > _sizeLeft) {
                        _bytesInLane = _sizeLeft;
                    }
                    if (_bytesInLane) {
                        unsigned long long lane = 0;
                        memcpy(&lane, (void *) _curData, _bytesInLane);
                        ((unsigned long long *) instance->state)[_lanePosition] ^= (lane << (_offsetInLane << 3));
                    }
                    _sizeLeft -= _bytesInLane;
                    _lanePosition++;
                    _offsetInLane = 0;
                    _curData += _bytesInLane;
                }
            }

            data += partialBlock;
            instance->byteIOIndex += partialBlock;
            if (instance->byteIOIndex == K12_rateInBytes) {
                keccak_p1600_permute_12rounds(instance->state);
                instance->byteIOIndex = 0;
            }
        }
    }
}

void kangaroo_twelve(const uint8_t *input, unsigned int inputByteLen, uint8_t *output, unsigned int outputByteLen) {
    KangarooTwelve_F queueNode;
    KangarooTwelve_F finalNode;
    unsigned int blockNumber, queueAbsorbedLen;

    memset(&finalNode, 0, sizeof(KangarooTwelve_F));
    const unsigned int len = inputByteLen ^ ((K12_chunkSize ^ inputByteLen) & -(K12_chunkSize < inputByteLen));
    kangaroo_twelve_f_absorb(&finalNode, input, len);
    input += len;
    inputByteLen -= len;
    if (len == K12_chunkSize && inputByteLen) {
        blockNumber = 1;
        queueAbsorbedLen = 0;
        finalNode.state[finalNode.byteIOIndex] ^= 0x03;
        if (++finalNode.byteIOIndex == K12_rateInBytes) {
            keccak_p1600_permute_12rounds(finalNode.state);
            finalNode.byteIOIndex = 0;
        } else {
            finalNode.byteIOIndex = (finalNode.byteIOIndex + 7) & ~7;
        }

        while (inputByteLen > 0) {
            const unsigned int len = K12_chunkSize ^ ((inputByteLen ^ K12_chunkSize) & -(inputByteLen < K12_chunkSize));
            memset(&queueNode, 0, sizeof(KangarooTwelve_F));
            kangaroo_twelve_f_absorb(&queueNode, input, len);
            input += len;
            inputByteLen -= len;
            if (len == K12_chunkSize) {
                ++blockNumber;
                queueNode.state[queueNode.byteIOIndex] ^= K12_suffixLeaf;
                queueNode.state[K12_rateInBytes - 1] ^= 0x80;
                keccak_p1600_permute_12rounds(queueNode.state);
                queueNode.byteIOIndex = K12_capacityInBytes;
                kangaroo_twelve_f_absorb(&finalNode, queueNode.state, K12_capacityInBytes);
            } else {
                queueAbsorbedLen = len;
            }
        }

        if (queueAbsorbedLen) {
            if (++queueNode.byteIOIndex == K12_rateInBytes) {
                keccak_p1600_permute_12rounds(queueNode.state);
                queueNode.byteIOIndex = 0;
            }
            if (++queueAbsorbedLen == K12_chunkSize) {
                ++blockNumber;
                queueAbsorbedLen = 0;
                queueNode.state[queueNode.byteIOIndex] ^= K12_suffixLeaf;
                queueNode.state[K12_rateInBytes - 1] ^= 0x80;
                keccak_p1600_permute_12rounds(queueNode.state);
                queueNode.byteIOIndex = K12_capacityInBytes;
                kangaroo_twelve_f_absorb(&finalNode, queueNode.state, K12_capacityInBytes);
            }
        } else {
            memset(queueNode.state, 0, sizeof(queueNode.state));
            queueNode.byteIOIndex = 1;
            queueAbsorbedLen = 1;
        }
    } else {
        if (len == K12_chunkSize) {
            blockNumber = 1;
            finalNode.state[finalNode.byteIOIndex] ^= 0x03;
            if (++finalNode.byteIOIndex == K12_rateInBytes) {
                keccak_p1600_permute_12rounds(finalNode.state);
                finalNode.byteIOIndex = 0;
            } else {
                finalNode.byteIOIndex = (finalNode.byteIOIndex + 7) & ~7;
            }

            memset(queueNode.state, 0, sizeof(queueNode.state));
            queueNode.byteIOIndex = 1;
            queueAbsorbedLen = 1;
        } else {
            blockNumber = 0;
            if (++finalNode.byteIOIndex == K12_rateInBytes) {
                keccak_p1600_permute_12rounds(finalNode.state);
                finalNode.state[0] ^= 0x07;
            } else {
                finalNode.state[finalNode.byteIOIndex] ^= 0x07;
            }
        }
    }

    if (blockNumber) {
        if (queueAbsorbedLen) {
            blockNumber++;
            queueNode.state[queueNode.byteIOIndex] ^= K12_suffixLeaf;
            queueNode.state[K12_rateInBytes - 1] ^= 0x80;
            keccak_p1600_permute_12rounds(queueNode.state);
            kangaroo_twelve_f_absorb(&finalNode, queueNode.state, K12_capacityInBytes);
        }
        unsigned int n = 0;
        for (unsigned long long v = --blockNumber; v && (n < sizeof(unsigned long long)); ++n, v >>= 8) {
        }
        uint8_t encbuf[sizeof(unsigned long long) + 1 + 2];
        for (unsigned int i = 1; i <= n; ++i) {
            encbuf[i - 1] = (uint8_t) (blockNumber >> (8 * (n - i)));
        }
        encbuf[n] = (uint8_t) n;
        encbuf[++n] = 0xFF;
        encbuf[++n] = 0xFF;
        kangaroo_twelve_f_absorb(&finalNode, encbuf, ++n);
        finalNode.state[finalNode.byteIOIndex] ^= 0x06;
    }
    finalNode.state[K12_rateInBytes - 1] ^= 0x80;
    keccak_p1600_permute_12rounds(finalNode.state);
    memcpy(output, finalNode.state, outputByteLen);
}
