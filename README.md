# Hedera™ C++ SDK

> The SDK for interacting with [Hedera Hashgraph]: the official distributed
> consensus platform built using the hashgraph consensus algorithm for fast,
> fair and secure transactions. Hedera enables and empowers developers to
> build an entirely new class of decentralized applications.

[Hedera Hashgraph]: https://hedera.com

## SDK IS NOT READY FOR PRODUCTION USE. IT IS CURRENTLY STILL UNDER DEVELOPMENT.

## Prerequisites

For MacOS and Linux users (these can be installed with `brew install` or `apt-get install`, respectively):

- ninja
- pkg-config

For Windows users:

- Visual Studio 2019/2022 Community/Pro with Universal Windows Platform Development Tools
- [Perl](http://strawberryperl.com/) (`perl.exe` must be added to `%PATH%`)
- [NASM](https://www.nasm.us) (`nasm.exe` must be added to `%PATH%`)

## Preparing the Local Node

To run the Integration Tests locally, first you will need to install the [Hedera Local Node](https://github.com/hashgraph/hedera-local-node). Follow the instructions given in the [Readme.md](https://github.com/hashgraph/hedera-local-node#readme).

## Build

This project features CMake Presets which simplify the setup of vcpkg based dependencies. The below commands are
typically all that is required.

```sh
# Ensure the VCPkg Submodule is initialized
git submodule update --init

# Windows (x64) Build
cmake --preset windows-x64-release
cmake --build --preset windows-x64-release

# Linux (x64) Build
cmake --preset linux-x64-release
cmake --build --preset linux-x64-release

# MacOS (Intel x64) Build
cmake --preset macos-x64-release
cmake --build --preset macos-x64-release

# MacOS (Aarch64) Build
cmake --preset macos-arm64-release
cmake --build --preset macos-arm64-release
```

## Testing

To run all SDK tests:

```sh
ctest -C Release --test-dir build/<PRESET>
```

To run a specific test:

```sh
ctest -C Release --test-dir build/<PRESET> -R <NAME OF TEST>
```

### Running Integration Tests

To run the integration tests it's necessary to have a running [Hedera Local Node](https://github.com/hashgraph/hedera-local-node). If the local node is already running, check the configuration JSON file for the network settings. Ensure the values for `network` tag contains a valid `AccountId` and a valid `IP address` for an operational node.

Example config file:

```
{
    "network": {
        "0.0.3": "127.0.0.1:50211"
    },
     
    "operator": {
        "accountId": "0.0.2",
        "privateKey": "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"
    }
}
```

(Source: [/config/local_node.json](https://github.com/hashgraph/hedera-sdk-cpp/blob/main/config/local_node.json))

To execute a specific integration test, you can use the command from above:

```sh
ctest -C Release --test-dir build/<PRESET> -R <NAME OF TEST>
```

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
