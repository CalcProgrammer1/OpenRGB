# Contributing to OpenRGB

The OpenRGB project welcomes contributions from the community.  The project would not support the number of devices it does today without the amazing contributions from community developers.  If you want to add a new device, fix a bug, or add a feature, feel free to open a merge request on the OpenRGB GitLab (https://gitlab.com/CalcProgrammer1/OpenRGB).

## Creating a Merge Request

To create a merge request, log into GitLab and fork the OpenRGB project.  Push your changes to your fork and then use the Create Merge Request option.  Before opening a merge request, please review the following best practices to help your merge request get merged without excessive delay.

* Please keep the scope of each merge request limited to one functional area.  If you wish to add a new device controller, for instance, don't also do code cleanup in another controller as part of the same merge request.
* Explain what your merge request is changing, why you feel the change is necessary, and add any additional information you feel is needed to best understand your change in the merge request description.
* Mark your merge request as a Draft (start title with "Draft:") until it has been fully developed and ideally tested and verified working.  Remove Draft status when you are ready for the code to be reviewed and merged.  I typically do not look at or review Draft merge requests.
* Follow the Style Guidelines below when making your code changes.
* Avoid using `git merge` when updating your fork.  The OpenRGB project uses a linear git history, meaning all changes are rebased onto the tip of master.  By using `git rebase` to update your fork, you will make it easier to accept merge requests.  I also recommend squashing your commits, though GitLab will do this as part of the merge process if you don't.
* Do not submit a merge request using your fork's `master` branch as the source.  The `master` branch is protected by default when creating a fork and I cannot manually rebase a protected branch before merging.  If you submit a merge request with a protected `master` branch as the source, it may not get merged until you unprotect it.

### Pipelines, Shared Runners, Quotas, and Accepting Merge Requests

Since mid 2022, GitLab has severely limited the usage of shared runners to all free users.  With that in mind, most pipeline jobs are skipped on forked copies of the project.  Only the Windows 64bit and Linux AppImage amd64 jobs are run on forks, and only if you have permission and available CI minutes on your account.  Unfortunately, with the ever tightening restrictions GitLab has on free CI usage, even to public projects with an open source license, you likely will not be able to utilize the CI builds for your own fork unless you set up your own runners.  Because of this, I no longer require a successful CI run in order to accept a merge request as it is an unreasonable ask given the current CI situation.  If your merge request requires additional verification, I may choose to merge it into a branch instead of into `master` so that my own runners can run the full set of jobs and then I will pull it into master after any verification or changes.  This also means that any build breakages inadvertently caused by merging a merge request may result in the merge being backed out, in which case I will either fix it myself or leave a message on the merge request to rework and resubmit a fixed version.

Please refer to the following link for [further information relating to minutes and quotas](https://docs.gitlab.com/ee/ci/pipelines/cicd_minutes.html).

## Style Guidelines

OpenRGB is written in C++, uses the Qt framework for UI, and uses the QMake build system.  While OpenRGB does use C++, I am primarily a C programmer and prefer doing things "C Style" vs. C++ style.  C++'s object oriented programming features fit the needs of a program such as OpenRGB where there are many implementations of a single interface so I chose to write it in C++ over C.  Still, however, I prefer using C-style code where possible.  When making changes to existing code files, try to follow the existing style.  Merge requests that go out of their way to restyle code will not be accepted and will be sent back for rework.

### Functional Style

* Limit use of C++ std library data types
    * For fixed-length containers, use C arrays
    * For list constants, use const C arrays
    * For fixed-length strings, use C char buffers ("C strings"), though using std::string is acceptable if the value will be placed in a std::string
    * For basic types, prefer using the non-typedef'd basic types (char, short, int, etc) over typedef'd (uint8_t, uint16_t, uint32_t, etc)
        * Unless defined in an OpenRGB API or the file is implementing an existing API using these types (such as i2c_smbus)
    * For hard-coded values, use `#define`s over const variables unless you need a pointer to the value
    * For variable-length containers, use std::vector
    * For variable-length strings, use std::string
    * `struct` should not include any functions, only variables
    * `class` should only be used for objects containing functions
    * Other std types may be used sparingly if necessary
* Only use Qt libraries, types, and functionality in user interface files
    * Anything under the qt/ folder basically
    * Do not use QDebug, use our LogManager instead.  If you use QDebug during development, please remove it before submitting your merge request
* Use C-style indexed for loops when iterating through an array or vector
* Use C-style casts unless necessary
* Avoid the `auto` type.  It makes code more difficult to read
* Do not use `printf` or `cout` for debug messages, use LogManager instead
* Don't define namespaces unless necessary

### Non-functional Style

* Set your editor's tab settings to insert spaces, using 4 spaces per tab stop
* Always put opening and closing braces `{`, `}` on their own lines
* Indent code inside the braces, do not indent the braces themselves
* Capitalize hexadecimal literals (`0xFF` not `0xff`) except in the udev rules file (which must be lowercase to work)
* No space between keyword and open parenthesis (`if(TRUE)` not `if (TRUE)`).  There are some instances of this that haven't been cleaned up in the code.
* Generally, `snake_case` is used for variable names and `CamelCase` is used for function and class names
* Add a header comment to new files with the filename, description, original author, date, and SPDX license identifier.  You can copy one from an existing file to preserve formatting.
* Line up `=` on a tab stop when doing a lot of assignments together
* Use this comment box style when adding comments:
    * Try to start the comment box on the same column as the code directly beneath it and end on column 61 if possible.  See the example below:

```
/*---------------------------------------------------------*\
| This is a comment                                         |
\*---------------------------------------------------------*/
```

## Translating

Translation files are located in [`OpenRGB/qt/i18n/`](https://gitlab.com/CalcProgrammer1/OpenRGB/-/tree/master/qt/i18n), where languages are formatted using ISO 639-1 format: `OpenRGB_xx_XX.ts` — `xx_XX` representing the language code.
In order to translate a file, you need to [fork](https://gitlab.com/CalcProgrammer1/OpenRGB/-/forks/new) the project, create a new file for your language with `lupdate` (or edit an exisiting one), edit the file with `qtlinguist`, commit, push, and create a merge request.
