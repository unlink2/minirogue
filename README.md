# mrg

## Table of content

- [Installation](#Installation)
- [Usage](#Usage)
- [License](#License)
- [Contributing](#Contributing)
- [TODO](#TODO)

## Installation

To build this program you will require a recent C compiler, premake, `libargtable2` and `libcmocka` for unit tests.

```sh
premake gmake # to build gnu make file 
make # to compile all targets 
```

## Usage

1) Rename `mrg` to the project's name in all files and folders 

## License

This program is distributed under the terms of the MIT License.

## Contributing

### Style 

- Most functions that can fail return -1 on failure 
- If a function returns a struct that can fail, the failure should be indicated by a struct member (e.g. `good`) which is then set to -1
    - If the struct has some sort of id or handle the id or handle should be set to -1 instead
- Never store pointers in structs, instead use handles that index into the apropriate tables 
