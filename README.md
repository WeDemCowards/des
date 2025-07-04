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

Padding in des_ecb not implemented properly. Instead of zero's, the padding described in PKCS #5 should be used. I also noticed that the padding is not quite at the end of a message. e.g. 

```
$ diff plain.html decrypted.html

   1   │ 437c437
   2   │ < </body></html>
   3   │ \ No newline at end of file
   4   │ ---
   5   │ > </body></html^@^@^@^@^@^@^@>
   6   │ \ No newline at end of file

```

After this has been fixed, detection and removal of padding after decryption should be implemented.
