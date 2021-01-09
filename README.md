# ToyScopy

[![CircleCI](https://circleci.com/gh/negibokken/toyscopy/tree/master.svg?style=svg)](https://circleci.com/gh/negibokken/toyscopy/tree/master)
[![codecov](https://codecov.io/gh/negibokken/toyscopy/branch/master/graph/badge.svg)](https://codecov.io/gh/negibokken/toyscopy)
[![](http://img.shields.io/badge/license-MIT-blue.svg)](./LICENSE)

ToyScopy is a toy browser written in C++.

![](./samples/browser.gif)

## development

1. install gtk+3.0, gtkmm3 and adwaita-icon-theme

   ```sh
   brew install gtk+3.0 gtkmm3 adwaita-icon-theme
   ```
2. install libcurl
    - [ref](https://stackoverflow.com/questions/41580504/how-to-install-libcurl-under-macos10-12-and-use-for-xcode)
3. execute cmake under src directory

   ```sh
   cd src
   cmake .
   ```

4. make binary

   ```sh
   make
   ```

5. execute ToyScopy

   ```sh
   ./main
   ```

   - for checking debug log, execute with `DEBUG=true` like below:

   ```sh
   DEBUG=true ./main
   ```

## license

MIT
