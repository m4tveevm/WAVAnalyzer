# WAV Analyzer

## Table of Contents

- [Introduction](#introduction)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

WAV Analyzer is a command-line utility for analyzing WAV audio files. It provides a detailed histogram based on
different frequency components of the audio file for any given second.

## Installation

### Prerequisites

- Unix-like OS (Linux or macOS)
- CMake (minimum version 3.28)
- C++17 compliant compiler (GCC, Clang, MSVC)
- FFTW library

### Installing FFTW library

- On Ubuntu/Debian:
  ```bash
  sudo apt-get install libfftw3-dev
  ```
- On Fedora:
  ```bash
  sudo dnf install fftw-devel
  ```
- On macOS (using Homebrew):
  ```bash
  brew install fftw
  ```

### Building WAV Analyzer

1. Clone the repository:
   ```bash
   git clone https://github.com/m4tveevm/WAVAnalyzer.git
   ```
2. Change to the project directory:
   ```bash
   cd wav-analyzer
   ```
3. Generate the build system using CMake:
    ```bash
   cmake -S . -B build
    ```
4. Build the project:
   ```bash
   cmake --build build --config Release
   ```

## Usage

To analyze a WAV file, run:

```bash
./wav-analyzer path_to_your_file.wav
```

Navigate through the audio seconds using the LEFT and RIGHT arrow keys. Press `q` to quit the application.

## Contributing

Contributions are welcome! For major changes, please open an issue first to discuss what you would like to change.

## License

Distributed under the MIT License. See `LICENSE` for more information.
