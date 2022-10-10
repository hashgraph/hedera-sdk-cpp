# Hedera™ C++ SDK

> The SDK for interacting with [Hedera Hashgraph]: the official distributed
> consensus platform built using the hashgraph consensus algorithm for fast,
> fair and secure transactions. Hedera enables and empowers developers to
> build an entirely new class of decentralized applications.

[Hedera Hashgraph]: https://hedera.com

## SDK IS NOT READY FOR PRODUCTION USE. IT IS CURRENTLY STILL UNDER DEVELOPEMENT.

## Generate the protobuf files

In order to get the C++ SDK to build, you must generate the protobuf files that
it uses. This can be done in a couple steps:

1. Follow the instructions at the [Google's Protobuf GitHub Page] to build the protobuf compiler and get the protobuf source files.
2. Pull down the [Hedera Protobufs] to get the proto files.
3. Follow the instructions at the [Google Protobuf Developer's Page] to then generate the C++ files.
4. Follow these steps to then copy the generated files into the correct locations in the SDK:

```sh
cd sdk
mkdir generated
cd generated
mkdir include
mkdir src
cp <FOLDER-CONTAINING-GENERATED-C++-FILES>/*.h include/
cp <FOLDER-CONTAINING-GENERATED-C++-FILES>/*.cc src/
```

[Google's Protobuf GitHub Page]: https://github.com/protocolbuffers/protobuf
[Hedera Protobufs]: https://github.com/hashgraph/hedera-protobufs
[Google Protobuf Developer's Page]: https://developers.google.com/protocol-buffers/docs/cpptutorial#compiling-your-protocol-buffers

## Build

```sh
mkdir build
cd build
cmake ..
cd ..
cmake --build build/ --parallel <number-of-processors>
```

## Testing

To run all SDK tests (this excludes tests generated by the protobuf library):
```sh
cd build
ctest -E '(lite|check)'
```

To run a specific test:
```sh
cd build
ctest -R <name-of-test>
```

## Recommendations

We suggest using [Visual Studio Code] to do development work. It provides a variety of different extensions to make development work easier, such as simplifying builds, testing, static analysis, and documentation generation. We recommend the following extensions:
 - [C/C++ Extension Pack]
 - [Better C++ Syntax]
 - [Doxygen Documentation Generator]
 - [SonarLint]

[Visual Studio Code]: https://code.visualstudio.com/
[C/C++ Extension Pack]: https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack
[Better C++ Syntax]: https://marketplace.visualstudio.com/items?itemName=jeff-hykin.better-cpp-syntax
[Doxygen Documentation Generator]: https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen
[SonarLint]: https://marketplace.visualstudio.com/items?itemName=SonarSource.sonarlint-vscode

## Contributing

Contributions are welcome. Please see the
[contributing guide](https://github.com/hashgraph/.github/blob/main/CONTRIBUTING.md)
to see how you can get involved.

## Code of Conduct

This project is governed by the
[Contributor Covenant Code of Conduct](https://github.com/hashgraph/.github/blob/main/CODE_OF_CONDUCT.md). By
participating, you are expected to uphold this code of conduct.

## License

[Apache License 2.0](LICENSE)
