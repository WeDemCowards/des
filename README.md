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
