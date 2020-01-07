# ToyScopy

[![CircleCI](https://circleci.com/gh/negibokken/toyscopy/tree/master.svg?style=svg)](https://circleci.com/gh/negibokken/toyscopy/tree/master)
[![codecov](https://codecov.io/gh/negibokken/toyscopy/branch/master/graph/badge.svg)](https://codecov.io/gh/negibokken/toyscopy)
[![](http://img.shields.io/badge/license-MIT-blue.svg)](./LICENSE)

ToyScopy is a toy browser written in C++.

![](./samples/browser.gif)

## development

1. install gtk+3.0

   ```sh
   brew install gtk+3.0 adwaita-icon-theme
   ```

2. execute cmake under src directory

   ```sh
   cd src
   cmake .
   ```

3. make binary

   ```sh
   make
   ```

4. execute ToyScopy

   ```sh
   ./main
   ```

   - for checking debug log, execute with `DEBUG=true` like below:

   ```sh
   DEBUG=true ./main
   ```

## license

MIT
