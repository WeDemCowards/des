# DES Command line tool

A robust and (hopefully) pleasant to use command line tool for DES. Try it out!

## USAGE

```
Usage: des FUNCTION [OPTIONS]

FUNCTION
    -e, --encrypt                       Encrypt the input data (Requires a keyfile)
    -d, --decrypt                       Decrypt the input data (Requires a keyfile)
    -g, --keygen                        Generate a des key
    -h, --help                          Print this help page

OPTIONS
    -i <filepath>, --input <filepath>   Specify the input for the cipher
    -o <filepath>, --output <filepath>  Specify the output for the cipher or keygen
    -k <filepath>, --key <filepath>     Specify the key for the cipher
    -m <mode>,     --mode <mode>        Specify blockcipher mode of operation.

MODES
    ecb     Electronic Codebook
    cbc     Cipher Block Chaining

DEFAULT BEHAVIOUR
    Specifying a FUNCTION is necessary. If no function is specified, then this
    help page will be printed.

    If keygen is selected, the only relevant option is -o --output. If this is
    not specified, the generated key will be written to stdout.

    If encryption or decryption are selected, a filepath for the key must be
    provided.

    If the following options are not provided, their default behaviour is as follows: 
        - mode:   ecb
        - input:  stdin
        - output: stdout

AUTHOR
    Aaron McCurdy
    mccurdya@proton.me
    https://github.com/WeDemCowards

```

## TO DO

- [ ] Modes of operation
    - [x] ECB
    - [ ] CBC
    - [ ] OFB
    - [ ] ...
- [ ] Weak key detection
- [ ] Robust test vector validation
- [ ] Interoperability (see issues)
- [ ] 3DES

## ISSUES:

The system fails to be interoperatable in the following ways:

- Endianess: The encryption and decryption process does not account for architectures with differing Endianess. You need to read the manual for `htobe64` and implement it where necessary.

- Portability: The key generation uses Linux's `<sys/getrandom.h>`, with no option for Windows. It wouldn't (shouldn't) be too hard to make the program portable.