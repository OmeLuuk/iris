# The Iris Ecosystem

## Overview

The Iris Ecosystem is a holistic repository embodying a series of interconnected C++ projects including the core Iris messaging server, along with common utilities and components shared across projects. Designed with a monorepo architecture, it simplifies dependency management and promotes modular design across all encompassed projects.

## Architecture

### Monorepo Management

The repository consolidates common code into a universal directory, streamlining dependency management and enhancing modularity.

### Projects

- **Iris**: A low latency, high-performance messaging server.
- **Prisma**: A broadcast messaging system leveraging Iris and TCP streams.

### Dependencies
To build and run, install the following:
```sudo apt-get update```
```sudo apt install cmake```
```sudo apt-get install qtbase5-dev```
```sudo apt-get install qtwayland5```

Upon getting the error
```
~/dev/iris/build$ ./bin/iris_chat Luuk
QSocketNotifier: Can only be used with threads started with QThread
./bin/iris_chat: symbol lookup error: /snap/core20/current/lib/x86_64-linux-gnu/libpthread.so.0: undefined symbol: __libc_pthread_init, version GLIBC_PRIVATE
```
unset your path variable like so
```unset GTK_PATH```

## Common Utilities

### Customizable Logging System

The common directory incorporates a detailed logging system that can be utilized across various projects. It provides an assortment of logging levels to facilitate effective debugging and monitoring, and allows for compile-time log level management to ensure zero-overhead in production builds. Keep in mind that while the debug messages can be evaluated in compile-time in the ```log()``` method and make the body zero-overhead in Release mode, a function call to ```log()``` will still be made if you do not put the function call itself in a ```constexpr``` as well. Hopefully some day this will be changed to be dealt with more effectively.

Usage Example:
```cpp
log(LL::DEBUG, "Debug message");
log(LL::INFO, "Informational message");
log(LL::ERROR, "Error message");
