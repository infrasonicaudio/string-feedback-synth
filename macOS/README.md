# MacOS Build Target

### ⚠️ WARNING: This is experimental and meant for debugging only ⚠️

This directory contains a `cmake` configuration and alternate main source file for
building the synth engine as a simple command line executable for direct playback
on macOS.

## Prerequisites

* CMake
* Up to date compiler (Xcode tools should be fine)

## Building

The easiest way to build this is to use the VSCode workspace task definitions defined 
at the root of the repo:

* `build_macOS`
* `build_and_run_macOS`
* `clean_macOS`

If you don't want to use VSCode take a look at the command definitions inside
`.vscode/tasks.json` and run the same commands it defines, from within this directory.
