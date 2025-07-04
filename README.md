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

- Padding is now implemented properly for encryption. The next step is to update the decryption function to detect and remove it.
