# Hedera™ C++ SDK

![](https://img.shields.io/badge/c++-17-blue)
![](https://img.shields.io/badge/cmake-3.24-blue)
[![](https://github.com/hashgraph/hedera-sdk-cpp/actions/workflows/flow-pull-request-checks.yaml/badge.svg)](https://github.com/hashgraph/hedera-sdk-cpp/actions/workflows/flow-pull-request-checks.yaml)

> The SDK for interacting with [Hedera Hashgraph]: the official distributed
> consensus platform built using the hashgraph consensus algorithm for fast,
> fair and secure transactions. Hedera enables and empowers developers to
> build an entirely new class of decentralized applications.

[Hedera Hashgraph]: https://hedera.com

## Prerequisites

For MacOS and Linux users:

- ninja
    - MacOS: `brew install ninja`
    - Linux: `apt-get install ninja`
- pkg-config
    - MacOS: `brew install pkg-config`
    - Linux: `apt-get install pkg-config`
- cmake
    - MacOS: `brew install cmake`
    - Linux: `apt-get install cmake`

📣 **Note**: Ensure you install all three `ninja`, `pkg-config`, and `cmake` to avoid errors in subsequent steps. The
installations might take a few minutes.

For Windows users:

- Visual Studio 2019/2022 Community/Pro with Universal Windows Platform Development Tools
- [Perl](http://strawberryperl.com/) (`perl.exe` must be added to `%PATH%`)
- [NASM](https://www.nasm.us) (`nasm.exe` must be added to `%PATH%`)

## Build

This project features CMake Presets which simplify the setup of vcpkg based dependencies. The below commands are
typically all that is required.

1. Clone the project

```sh
git clone https://github.com/hashgraph/hedera-sdk-cpp.git
```

2. CD to the project directory

```sh
cd hedera-sdk-cpp
```

3. Complete the following tasks within your project directory for the build you want

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

To run all SDK tests (for Release or Debug builds):

```sh
ctest -C [Release|Debug] --test-dir build/<PRESET>
```

To run all SDK unit tests and test vectors:

```sh
ctest -C [Release|Debug] --test-dir build/<PRESET> -R "TestVectors|UnitTests"
```

To run all SDK integration tests:

```sh
ctest -C [Release|Debug] --test-dir build/<PRESET> -R "IntegrationTests"
```

To run a specific test:

```sh
ctest -C [Release|Debug] --test-dir build/<PRESET> -R <NAME OF TEST>
```

### Running Integration Tests

To run the integration tests it's necessary to have a
running [Hedera Local Node](https://github.com/hashgraph/hedera-local-node). If the local node is already running, check
the configuration JSON file for the network settings. Ensure the values for `network` tag contains a valid `AccountId`
and a valid `IP address` for an operational node.

Example config file:

```JSON
{
  "network": {
    "0.0.3": "127.0.0.1:50211"
  },
  "mirrorNetwork": [
    "127.0.0.1:5600"
  ],
  "operator": {
    "accountId": "0.0.2",
    "privateKey": "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"
  }
}
```

(Source: [config/local_node.json](https://github.com/hashgraph/hedera-sdk-cpp/blob/main/config/local_node.json))

## Examples

Examples must be run from the root directory in order to correctly access the address book and configuration files
located in the `addressbook/` and `config/` directories. Make sure your `.env` file is populated with:

- `OPERATOR_ID`: The ID of the operator account.
- `OPERATOR_KEY`: The DER-encoded hex private key of the operator account.
- `HEDERA_NETWORK`: The Hedera network name. Must be one of `mainnet`, `testnet`, or `previewnet`.
- `PASSPHRASE`: Optional variable used by `hedera-sdk-cpp-generate-private-key-from-mnemonic-example` to generate a
  private key from a mnemonic with a passphrase.

The command to run an example looks like:

```shell
build/<PRESET>/sdk/examples/[Release|Debug]/<EXAMPLE_NAME>
```

- `<PRESET>`: the preset that was used to build in Step 3 under [Build](#build).
- `[Release|Debug]`: `Release` is you built in `Release` mode, otherwise `Debug`.
- `<EXAMPLE_NAME>`: The name of the example you are trying to run.

If you're trying to run an example from the release artifacts, you must first `cd` into the architecture folder of the
OS on which you are trying to run the example. For example, if you are running an `x86_64` architecture on `Linux`:

```shell
cd [Release|Debug]/Linux/x86_64
```

From there, you can run:

```shell
examples/<EXAMPLE_NAME>
```

__NOTE:__ Make sure you copy your `.env` file with your environment variables into this folder as well.

## Contributing to this Project

We welcome participation from all developers!

For instructions on how to contribute to this repo, please
review the [Contributing Guide for C++](CONTRIBUTING.md).

More instructions for contribution can be found in the
[Global Contributing Guide](https://github.com/hashgraph/.github/blob/main/CONTRIBUTING.md).

## Code of Conduct

This project is governed by the
[Contributor Covenant Code of Conduct](https://github.com/hashgraph/.github/blob/main/CODE_OF_CONDUCT.md). By
participating, you are expected to uphold this code of conduct.

If you need to prepare a new SDK release, then see
the [Step-by-Step](https://github.com/hashgraph/hedera-sdk-cpp/blob/00222-step-by-step-guide-to-distribute-a-new-release/NewReleaseStepByStep.md)
guide how to do it.

## License

[Apache License 2.0](LICENSE)
![](data:image/jpeg;base64,IyBIZWRlcmHihKIgQysrIFNESwoKPiBUaGUgU0RLIGZvciBpbnRlcmFjdGluZyB3aXRoIFtIZWRlcmEgSGFzaGdyYXBoXTogdGhlIG9mZmljaWFsIGRpc3RyaWJ1dGVkCj4gY29uc2Vuc3VzIHBsYXRmb3JtIGJ1aWx0IHVzaW5nIHRoZSBoYXNoZ3JhcGggY29uc2Vuc3VzIGFsZ29yaXRobSBmb3IgZmFzdCwKPiBmYWlyIGFuZCBzZWN1cmUgdHJhbnNhY3Rpb25zLiBIZWRlcmEgZW5hYmxlcyBhbmQgZW1wb3dlcnMgZGV2ZWxvcGVycyB0bwo+IGJ1aWxkIGFuIGVudGlyZWx5IG5ldyBjbGFzcyBvZiBkZWNlbnRyYWxpemVkIGFwcGxpY2F0aW9ucy4KCltIZWRlcmEgSGFzaGdyYXBoXTogaHR0cHM6Ly9oZWRlcmEuY29tCgojIyBTREsgSVMgTk9UIFJFQURZIEZPUiBQUk9EVUNUSU9OIFVTRS4gSVQgSVMgQ1VSUkVOVExZIFNUSUxMIFVOREVSIERFVkVMT1BNRU5ULgoKIyMgUHJlcmVxdWlzaXRlcwoKRm9yIE1hY09TIGFuZCBMaW51eCB1c2VycyAodGhlc2UgY2FuIGJlIGluc3RhbGxlZCB3aXRoIGBicmV3IGluc3RhbGxgIG9yIGBhcHQtZ2V0IGluc3RhbGxgLCByZXNwZWN0aXZlbHkpOgoKLSBuaW5qYQotIHBrZy1jb25maWcKCkZvciBXaW5kb3dzIHVzZXJzOgoKLSBWaXN1YWwgU3R1ZGlvIDIwMTkvMjAyMiBDb21tdW5pdHkvUHJvIHdpdGggVW5pdmVyc2FsIFdpbmRvd3MgUGxhdGZvcm0gRGV2ZWxvcG1lbnQgVG9vbHMKLSBbUGVybF0oaHR0cDovL3N0cmF3YmVycnlwZXJsLmNvbS8pIChgcGVybC5leGVgIG11c3QgYmUgYWRkZWQgdG8gYCVQQVRIJWApCi0gW05BU01dKGh0dHBzOi8vd3d3Lm5hc20udXMpIChgbmFzbS5leGVgIG11c3QgYmUgYWRkZWQgdG8gYCVQQVRIJWApCgojIyBQcmVwYXJpbmcgdGhlIExvY2FsIE5vZGUKClRvIHJ1biB0aGUgSW50ZWdyYXRpb24gVGVzdHMgbG9jYWxseSwgZmlyc3QgeW91IHdpbGwgbmVlZCB0byBpbnN0YWxsIHRoZSBbSGVkZXJhIExvY2FsIE5vZGVdKGh0dHBzOi8vZ2l0aHViLmNvbS9oYXNoZ3JhcGgvaGVkZXJhLWxvY2FsLW5vZGUpLiBGb2xsb3cgdGhlIGluc3RydWN0aW9ucyBnaXZlbiBpbiB0aGUgW1JlYWRtZS5tZF0oaHR0cHM6Ly9naXRodWIuY29tL2hhc2hncmFwaC9oZWRlcmEtbG9jYWwtbm9kZSNyZWFkbWUpLgoKIyMgQnVpbGQKClRoaXMgcHJvamVjdCBmZWF0dXJlcyBDTWFrZSBQcmVzZXRzIHdoaWNoIHNpbXBsaWZ5IHRoZSBzZXR1cCBvZiB2Y3BrZyBiYXNlZCBkZXBlbmRlbmNpZXMuIFRoZSBiZWxvdyBjb21tYW5kcyBhcmUKdHlwaWNhbGx5IGFsbCB0aGF0IGlzIHJlcXVpcmVkLgoKYGBgc2gKIyBFbnN1cmUgdGhlIFZDUGtnIFN1Ym1vZHVsZSBpcyBpbml0aWFsaXplZApnaXQgc3VibW9kdWxlIHVwZGF0ZSAtLWluaXQKCiMgV2luZG93cyAoeDY0KSBCdWlsZApjbWFrZSAtLXByZXNldCB3aW5kb3dzLXg2NC1yZWxlYXNlCmNtYWtlIC0tYnVpbGQgLS1wcmVzZXQgd2luZG93cy14NjQtcmVsZWFzZQoKIyBMaW51eCAoeDY0KSBCdWlsZApjbWFrZSAtLXByZXNldCBsaW51eC14NjQtcmVsZWFzZQpjbWFrZSAtLWJ1aWxkIC0tcHJlc2V0IGxpbnV4LXg2NC1yZWxlYXNlCgojIE1hY09TIChJbnRlbCB4NjQpIEJ1aWxkCmNtYWtlIC0tcHJlc2V0IG1hY29zLXg2NC1yZWxlYXNlCmNtYWtlIC0tYnVpbGQgLS1wcmVzZXQgbWFjb3MteDY0LXJlbGVhc2UKCiMgTWFjT1MgKEFhcmNoNjQpIEJ1aWxkCmNtYWtlIC0tcHJlc2V0IG1hY29zLWFybTY0LXJlbGVhc2UKY21ha2UgLS1idWlsZCAtLXByZXNldCBtYWNvcy1hcm02NC1yZWxlYXNlCmBgYAoKIyMgVGVzdGluZwoKVG8gcnVuIGFsbCBTREsgdGVzdHM6CgpgYGBzaApjdGVzdCAtQyBSZWxlYXNlIC0tdGVzdC1kaXIgYnVpbGQvPFBSRVNFVD4KYGBgCgpUbyBydW4gYSBzcGVjaWZpYyB0ZXN0OgoKYGBgc2gKY3Rlc3QgLUMgUmVsZWFzZSAtLXRlc3QtZGlyIGJ1aWxkLzxQUkVTRVQ+IC1SIDxOQU1FIE9GIFRFU1Q+CmBgYAoKIyMjIFJ1bm5pbmcgSW50ZWdyYXRpb24gVGVzdHMKClRvIHJ1biB0aGUgaW50ZWdyYXRpb24gdGVzdHMgaXQncyBuZWNlc3NhcnkgdG8gaGF2ZSBhIHJ1bm5pbmcgW0hlZGVyYSBMb2NhbCBOb2RlXShodHRwczovL2dpdGh1Yi5jb20vaGFzaGdyYXBoL2hlZGVyYS1sb2NhbC1ub2RlKS4gSWYgdGhlIGxvY2FsIG5vZGUgaXMgYWxyZWFkeSBydW5uaW5nLCBjaGVjayB0aGUgY29uZmlndXJhdGlvbiBKU09OIGZpbGUgZm9yIHRoZSBuZXR3b3JrIHNldHRpbmdzLiBFbnN1cmUgdGhlIHZhbHVlcyBmb3IgYG5ldHdvcmtgIHRhZyBjb250YWlucyBhIHZhbGlkIGBBY2NvdW50SWRgIGFuZCBhIHZhbGlkIGBJUCBhZGRyZXNzYCBmb3IgYW4gb3BlcmF0aW9uYWwgbm9kZS4KCkV4YW1wbGUgY29uZmlnIGZpbGU6CgpgYGAKewogICAgIm5ldHdvcmsiOiB7CiAgICAgICAgIjAuMC4zIjogIjEyNy4wLjAuMTo1MDIxMSIKICAgIH0sCiAgICAgCiAgICAib3BlcmF0b3IiOiB7CiAgICAgICAgImFjY291bnRJZCI6ICIwLjAuMiIsCiAgICAgICAgInByaXZhdGVLZXkiOiAiMzAyZTAyMDEwMDMwMDUwNjAzMmI2NTcwMDQyMjA0MjA5MTEzMjE3OGU3MjA1N2ExZDc1MjgwMjU5NTZmZTM5YjBiODQ3ZjIwMGFiNTliMmZkZDM2NzAxN2YzMDg3MTM3IgogICAgfQp9CmBgYAoKKFNvdXJjZTogWy9jb25maWcvbG9jYWxfbm9kZS5qc29uXShodHRwczovL2dpdGh1Yi5jb20vaGFzaGdyYXBoL2hlZGVyYS1zZGstY3BwL2Jsb2IvbWFpbi9jb25maWcvbG9jYWxfbm9kZS5qc29uKSkKClRvIGV4ZWN1dGUgYSBzcGVjaWZpYyBpbnRlZ3JhdGlvbiB0ZXN0LCB5b3UgY2FuIHVzZSB0aGUgY29tbWFuZCBmcm9tIGFib3ZlOgoKYGBgc2gKY3Rlc3QgLUMgUmVsZWFzZSAtLXRlc3QtZGlyIGJ1aWxkLzxQUkVTRVQ+IC1SIDxOQU1FIE9GIFRFU1Q+CmBgYAoKIyMgQ29udHJpYnV0aW5nCgpDb250cmlidXRpb25zIGFyZSB3ZWxjb21lLiBQbGVhc2Ugc2VlIHRoZQpbY29udHJpYnV0aW5nIGd1aWRlXShodHRwczovL2dpdGh1Yi5jb20vaGFzaGdyYXBoLy5naXRodWIvYmxvYi9tYWluL0NPTlRSSUJVVElORy5tZCkKdG8gc2VlIGhvdyB5b3UgY2FuIGdldCBpbnZvbHZlZC4KCiMjIENvZGUgb2YgQ29uZHVjdAoKVGhpcyBwcm9qZWN0IGlzIGdvdmVybmVkIGJ5IHRoZQpbQ29udHJpYnV0b3IgQ292ZW5hbnQgQ29kZSBvZiBDb25kdWN0XShodHRwczovL2dpdGh1Yi5jb20vaGFzaGdyYXBoLy5naXRodWIvYmxvYi9tYWluL0NPREVfT0ZfQ09ORFVDVC5tZCkuIEJ5CnBhcnRpY2lwYXRpbmcsIHlvdSBhcmUgZXhwZWN0ZWQgdG8gdXBob2xkIHRoaXMgY29kZSBvZiBjb25kdWN0LgoKSWYgeW91IG5lZWQgdG8gcHJlcGFyZSBhIG5ldyBTREsgcmVsZWFzZSwgdGhlbiBzZWUgdGhlIFtTdGVwLWJ5LVN0ZXBdKGh0dHBzOi8vZ2l0aHViLmNvbS9oYXNoZ3JhcGgvaGVkZXJhLXNkay1jcHAvYmxvYi8wMDIyMi1zdGVwLWJ5LXN0ZXAtZ3VpZGUtdG8tZGlzdHJpYnV0ZS1hLW5ldy1yZWxlYXNlL05ld1JlbGVhc2VTdGVwQnlTdGVwLm1kKSBndWlkZSBob3cgdG8gZG8gaXQuCgoKIyMgTGljZW5zZQoKW0FwYWNoZSBMaWNlbnNlIDIuMF0oTElDRU5TRSkK)
