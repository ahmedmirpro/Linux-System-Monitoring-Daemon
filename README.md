# Linux System Monitoring Daemon

A lightweight Linux system monitoring application written in **C++**, designed as a hands-on system programming project and progressively evolving toward a robust, multi-threaded Linux daemon.

The project focuses on practical Linux internals, process monitoring, modern C++ design, concurrency, diagnostics, and long-running service architecture.

## Project Goals

The main objectives are to:

* Explore Linux process information through the `/proc` filesystem.
* Build reliable process monitoring capabilities.
* Practice Linux system programming.
* Apply modern C++ concepts in a real system-level project.
* Progressively evolve the application toward a maintainable multi-threaded daemon.
* Develop production-oriented practices around logging, error handling, lifecycle management, testing, and diagnostics.

## Current Features

The current implementation provides:

* Process existence detection through `/proc/<pid>`.
* Periodic monitoring with a configurable sampling interval.
* Process information retrieval from `/proc/<pid>/status`, including:

  * Process name
  * Process state
  * PID
  * Parent PID
  * Resident memory usage (`VmRSS`)
* Robust parsing of `/proc/<pid>/stat`, including process names containing spaces.
* CPU accounting information:

  * User CPU time (`utime`)
  * System CPU time (`stime`)
  * Process start time (`starttime`)
  * Total accumulated CPU ticks
* Conversion of accumulated CPU time where applicable.
* Timestamped monitoring output.
* File-based logging through `Monitor.log`.
* Optional debug logging.
* Command-line argument validation.
* Process disappearance detection.
* Graceful termination using Unix signal handling (`SIGINT`).
* Separation between CPU-stat collection and logging responsibilities using dedicated data structures and methods.

## Current Architecture

At the current development stage, the application follows a simple architecture:

```text
                 main()
                    |
                    v
              Monitor object
                    |
        +-----------+-----------+
        |           |           |
        v           v           v
 ProcessExists   /proc/status  /proc/<pid>/stat
                    |           |
                    v           v
              Process Info   CPU Statistics
                    |           |
                    +-----+-----+
                          |
                          v
                       Logging
```

The architecture is intentionally being evolved incrementally rather than introducing unnecessary abstractions prematurely.

## Example Usage

```bash
./monitor <pid> <interval_seconds>
```

Example:

```bash
./monitor 2919 2
```

The application periodically verifies that the process still exists and collects its monitoring information until the process terminates or the monitor receives an interrupt signal.

## Technologies

* C++
* Linux
* `/proc` virtual filesystem
* POSIX/Linux APIs
* C++ Standard Library
* Git

## Concepts Practiced So Far

### Linux

* `/proc` filesystem
* Process lifecycle
* Process status information
* Linux CPU accounting
* User-space vs kernel-space CPU time
* CPU ticks
* Unix signals
* Graceful shutdown

### C++

* Classes and encapsulation
* Structures for system data
* Standard file streams
* String parsing
* String streams
* References and const correctness
* Return-by-value
* Type conversions
* Error handling
* Separation of responsibilities

### Software Engineering

* Incremental refactoring
* Logging and debug facilities
* Input validation
* Error paths
* Separation between data collection and presentation
* Git-based development
* Progressive architecture design

## Development Status

The project is currently transitioning from a **single-threaded process monitor** toward a **multi-threaded Linux monitoring daemon**.

The next development phase focuses on:

```text
std::thread
    ↓
Thread lifecycle
    ↓
Shared state
    ↓
Race-condition management
    ↓
Mutex / RAII locking
    ↓
Condition variables
    ↓
Producer / Consumer architecture
```

The objective is to progressively separate monitoring, processing, and logging responsibilities into independent execution contexts.

## MVP Roadmap

The first portfolio-ready release is planned to include:

* Robust `/proc` monitoring
* Clean C++ architecture
* Multi-threaded execution
* Producer/consumer communication
* Thread-safe data exchange
* Structured logging
* Configurable monitoring
* Graceful `SIGINT` / `SIGTERM` shutdown
* CMake-based build
* Runtime robustness checks
* Memory/resource validation
* Technical documentation

More advanced capabilities such as IPC, persistence, advanced CPU-load calculation, systemd integration, and extended fault management are intentionally reserved for later versions.

## Project Philosophy

This project is primarily an engineering and learning exercise.

Features are implemented progressively, with emphasis on understanding the underlying Linux and C++ mechanisms rather than relying on ready-made abstractions.

The project therefore prioritizes:

**understanding → design → implementation → debugging → validation**

over feature quantity.

## Status

🚧 **Work in progress — MVP under active development**
