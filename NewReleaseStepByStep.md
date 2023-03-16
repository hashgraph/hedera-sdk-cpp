# Step 1

Prepare a new git `tag` and push it to `origin`.

Add a new git `tag` using the following command in terminal where `v0.3.0` is the name of the newly added tag:
> git tag -s v0.3.0

Then execute:
> git push origin v0.3.0

It will populate the newly added tag in `origin`. 

# Step 2
Open [Hedera C++ SDK](https://github.com/hashgraph/hedera-sdk-cpp) and search for `Create a new release` link.
Select tag `v0.3.0` from the dropdown list. Enter the Release title to be same as the release number which is been prepared (for example: `v0.3.0`).

# Step 3 - Enter Notes

It's a good decision to tap direclty on button `Generate release notes` to get an automatically generated notes which can be used as a basis.

## What's Changed

In this section should be filled all of the issues which have been completed and merged into `main` branch. Use only the title of each issue and add a prefix with ➡️.

## Examples

In this section should be listed all of the newly added examples. Each of them should represent a link to the implementation (for example: [TransferTokensExample.cc](TransferTokensExample.cc)). It's a good idea also to add prefix with ➡️ before each line.

## Notes
Section to write any kind of important notes. For example, if there are any files which are not included in the build and should not be used, then it's better all of them to be listed here.

## Added
Here should be listed all of the Pull Requests (PRs) which are part of the release. Use the following format
> (pull-request-title) by (@author) in (#link)
