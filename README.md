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
        - mode:   cbc
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
    - [x] CBC
    - [ ] OFB
    - [ ] ...
- [x] Weak key detection
- [ ] Robust test vector validation
- [ ] 3DES
- [ ] Make portible for Windows

### Modes of Operation
After CBC is implemeneted, I will consider the program *complete*. Other modes
of operation would be nice to include also, but there isn't too much educational
or functional benefit to this. This isn't meant to be a swiss army tool.

### 3DES
Would be easy enough to implement, with the only problem being cluttering up my
nice and realtively simple interface. Will consider down the line.

### Windows
Probably not hard, I would just have to read up on the Microsoft CRT. Will
require writing your own portable functions for `htobe`, etc.
