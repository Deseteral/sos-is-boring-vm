# SOS is boring VM
Simple virtual machine.

## Specification
The virtual machine is based on imaginary specification. The document with CPU
specification is located [here](docs/spec.txt).

## Building
Currently only Linux builds are supported.
Only required dependency is GCC with g++.

To build, execute Makefile found in project's root directory:
```
make
```

If you want to build only VM or Assembler:
```
make vm
make asm
```

## Testing
To run the tests execute `run_tests.sh` script:
```
./run_tests.sh
```

## License
The project is [MIT licensed](LICENSE).
