# DES Command line tool

A robust and (hopefully) pleasant to use command line tool for DES.

## TO DO

- [x] Key generation
- [x] Key scheduling
- [x] DES
- [ ] Modes of operation
    - [x] ECB
    - [ ] CBC
    - [ ] OFB
    - [ ] ...
- [ ] 3DES
- [ ] Proper padding scheme

## ISSUES

Padding follows PKCS #5, but has not been implemented properly. If the given message equals 0 mod 64, then the final padding bit (0x8 * 8) is not written. This will have to be rewritten.
