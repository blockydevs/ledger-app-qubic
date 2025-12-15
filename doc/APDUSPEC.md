# Qubic Ledger App - APDU Command Specification

This document describes the APDU interface implemented by this application, based on the source code in `src/`. It covers both the standard Ledger command to retrieve the app and version, and the app‑specific commands for name, public key derivation, and transaction signing.

Unless noted otherwise, APDUs follow the BOLOS/ISO7816-4 framing described in `doc/APDU.md`:
- `Lc` is exactly 1 byte (0–255)
- No `Le` field is used in commands
- Max command size is 260 bytes (5 header + 255 data)
- Max response size is 260 bytes (up to 258 data + 2 status bytes)

Common header fields:
- `CLA` - Instruction class
- `INS` - Instruction code
- `P1`, `P2` - Instruction parameters
- `Lc` - Byte length of `CData`
- `CData` - Command data

Status words (subset): see `src/sw.h` for the full list.
- `0x9000` `SW_OK` - Success
- `0x6985` `SW_DENY` - Denied by user
- `0x6A86` `SW_WRONG_P1P2` - Incorrect P1 or P2
- `0x6A87` `SW_WRONG_DATA_LENGTH` - Incorrect data length
- `0x6D00` `SW_INS_NOT_SUPPORTED` - Unknown INS
- `0x6E00` `SW_CLA_NOT_SUPPORTED` - Unknown CLA
- `0xB004` `SW_WRONG_TX_LENGTH` - Transaction too long
- `0xB005` `SW_TX_PARSING_FAIL` - Transaction parsing failed
- `0xC001` `SW_WRONG_BIP32_PATH_LENGTH` - Invalid derivation path length

CLA values:
- Standard Ledger “get app and version”: `CLA = 0xB0`
- App‑specific commands: `CLA = 0xE0` (see `src/constants.h`)


## 1) Get App And Version (standard)
- CLA: `0xB0`
- INS: `0x01`
- P1: `0x00`
- P2: `0x00`
- Lc: `0x00` (no `CData`)

Response data format (from Ledger standard):
```
formatId (1) || appNameLen (1) || appName (appNameLen) ||
appVersionLen (1) || appVersion (appVersionLen) || appFlagsLen (1) || appFlags (appFlagsLen)
```
Example (from logs):
```
=> b0 01 00 00 00
<= 01 05 51 75 62 69 63 05 30 2e 31 2e 35 01 00 90 00
   ^  ^  Q  u  b  i  c  ^  0  .  1  .  5  ^  ^  SW
   |  |                |                  |  |
   |  |                |                  |  appFlags (0x00), appFlagsLen (0x01)
   |  |                appVersion "0.1.5" (len 0x05)
   |  appName "Qubic" (len 0x05)
   formatId = 0x01
```


## 2) Get App Name
- CLA: `0xE0`
- INS: `0x04` (`GET_APP_NAME`)
- P1: `0x00`
- P2: `0x00`
- CData: empty (`Lc = 0`)

Response data:
- ASCII app name bytes (`APPNAME` from the build)

Example:
```
=> e0 04 00 00 00
<= 51 75 62 69 63 90 00              # "Qubic" + SW_OK
```


## 3) Get Public Key
- CLA: `0xE0`
- INS: `0x05` (`GET_PUBLIC_KEY`)
- P1: display flag - `0x00` = don’t display, `0x01` = display address on device
- P2: `0x00`
- CData: BIP32 path

BIP32 path encoding (`src/handler/get_public_key.c`):
```
pathLen (1) || index0 (4 BE) || index1 (4 BE) || ... || index{pathLen-1} (4 BE)
```
Notes:
- `pathLen` is the number of 32‑bit indices in the path
- Each index is big‑endian; hardened indices are typically `0x80000000 | i`
- Minimum valid `pathLen` is 1; zero length yields `SW_WRONG_DATA_LENGTH`

Response data (`src/helper/send_reponse.c`):
- 32‑byte raw public key

Examples:
```
=> e0 05 01 00 15 05 80 00 00 2c 80 01 45 5d 80 00 00 00 00 00 00 00 00 00 00 00
                 ^  ^  hardened 44'  ^ coin type '83357'   ^ account 0  ^ change 0  ^ address 0
   P1=0x01 (display)
<= 9f 8c 8f 91 0a a2 bb 32 af fc 3e 6f 07 78 49 0c d3 72 2f 5d 90 99 59 90 91 57 90 c7 19 93 85 2c 90 00
   ^ 32‑byte public key                                                                            ^ SW_OK
```

Errors:
- Wrong `P1`/`P2`: `SW_WRONG_P1P2`
- Bad/short `CData` (path too short): `SW_WRONG_DATA_LENGTH`


## 4) Sign Transfer Transaction
- CLA: `0xE0`
- INS: `0x06` (`SIGN_TX`)
- P1: chunk index - `0x00` for first chunk, up to `0x03` (`P1_MAX`)
- P2: chunk continuation flag - `0x80` (`P2_MORE`) if more chunks follow, `0x00` (`P2_LAST`) for the last chunk

Chunking and `CData` layout (`src/handler/sign_tx.c`):
- First chunk (`P1=0x00`):
  - `CData =` BIP32 path (same encoding as Get Public Key) followed immediately by the first bytes of the serialized transaction body
- Subsequent chunks (`P1=0x01`.., `P2=0x80` while more data):
  - `CData =` next bytes of the serialized transaction body
- The app accumulates transaction bytes up to 510 bytes (`MAX_TX_LEN`)
- On non‑final chunks (`P2=0x80`), the app replies with `SW_OK` to acknowledge receipt
- On the final chunk (`P2=0x00`), the app:
  - Parses the transaction (`src/transaction/deserialize.c`), expecting exactly the structure below
  - Computes the KangarooTwelve hash of the serialized transaction
  - Shows a confirmation UI and, if accepted, returns the signature

Transaction structure (`src/transaction/types.h`, sizes are fixed):
```
source_public_key      32 bytes
destination_public_key 32 bytes
amount                 8 bytes (uint64, little‑endian as serialized in the struct)
tick                   4 bytes (uint32)
inputType              2 bytes (uint16)
inputSize              2 bytes (uint16)
```
Parsing checks:
- Both source and destination public keys must be non‑zero
- `amount` must be non‑zero
- Total body length must match the struct size (510 bytes max bound is enforced separately)
- If parsing fails and blind signing is disabled, `SW_TX_PARSING_FAIL` is returned; if blind signing is enabled in settings, a “blind sign” confirmation UI is shown instead

Response data on approval (`src/helper/send_reponse.c`):
```
sigLen (1) || signature (sigLen)
```
where `signature` is a 64‑byte signature for this app (`sigLen` is typically `0x40`).

Example (single‑chunk sign request):
```
=> e0 06 00 00 65 05 80 00 00 2c 80 01 45 5d 80 00 00 00 00 00 00 00 00 00 00 00
                         9f 8c 8f 91 0a a2 bb 32 af fc 3e 6f 07 78 49 0c d3 72 2f 5d 90 99 59 90 91 57 90 c7 19 93 85 2c
                         04 4b fe c5 7c 7c fb 51 52 d6 48 44 9e 90 f0 fc 6f 8b 3b cf 9b 75 c6 ef b1 ea 81 00 00 00 00 00 7b
                         00 00 00 00 00 00 00 39 30 00 00 01 00 00 01
   [path (as above)] [transaction body bytes]
<= 40 ee 0c 7c a4 38 19 28 8c 31 91 90 52 80 7e cf 6e 42 5d a2 74 00 a0 ab d6 b3 df 50 cb 4a 2b a7 e7 7d 33 9a d8 9b 15 c2 45 28 4b 4f a3 a4 3a d8 e1 92 25 6a eb 4d 0c 24 4a 4a 3d aa 99 c9 43 06 00 90 00
   ^^ 64‑byte signature ……………………………………………………………………………………                       ^^
```

Errors (non‑exhaustive):
- Bad `P1` or `P2`: `SW_WRONG_P1P2`
- Missing/short data: `SW_WRONG_DATA_LENGTH`
- Path length zero: `SW_WRONG_BIP32_PATH_LENGTH`
- Transaction too long: `SW_WRONG_TX_LENGTH`
- Parsing failure: `SW_TX_PARSING_FAIL` (unless blind signing enabled, see settings)
- User refused in UI: `SW_DENY`


## Notes
- The app prints debug information when built with debug flags; this does not affect APDU formats.
- The app’s UI may require user confirmation for displaying addresses (`GET_PUBLIC_KEY` with `P1=0x01`) and for signing transactions.
- Coin type and exact BIP32 path are application‑specific; examples above use: `m/44'/83357'/0'/0/0`.
