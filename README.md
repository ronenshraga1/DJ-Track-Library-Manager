# DJ Track Session Manager

## Project Overview

DJ Track Session Manager is a C++ command-line project for simulating a DJ library, controller cache, and mixing workflow. It is structured as a systems programming assignment focused on dynamic memory management, Rule of 5 implementation, ownership transfer, polymorphism, and a small service-oriented architecture.

The application can run built-in assignment checks or load a DJ session from `bin/dj_config.txt`, build playlists from configured track indices, cache tracks with an LRU policy, and simulate loading tracks onto alternating mixer decks.

## Features

- MP3 and WAV track models derived from a polymorphic `AudioTrack` base class.
- Rule of 5 coverage for dynamically allocated waveform data.
- Playlist management through a linked-list style `Playlist` abstraction.
- Custom move-only pointer wrapper used for ownership transfer.
- LRU controller cache with configurable capacity.
- DJ library service that builds a track library from configuration data.
- Mixing engine service that clones cached tracks into mixer decks, calls format-specific load/beatgrid analysis, and alternates decks.
- Config-driven playlists and library track definitions.
- Interactive playlist selection mode and automatic "play all playlists" mode.
- Makefile targets for normal, debug, release, quick run, and Valgrind leak checks.

## Tech Stack

- Language: C++11
- Build system: Make
- Compiler: `g++`
- Standard library containers and utilities
- Optional tooling: Valgrind and GDB

No package manager, external C++ dependency file, Dockerfile, CI workflow, or deployment configuration was found in this repository.

## Project Structure

```text
.
|-- include/                    # Header files
|   |-- AudioTrack.h
|   |-- MP3Track.h
|   |-- WAVTrack.h
|   |-- Playlist.h
|   |-- PointerWrapper.h
|   |-- LRUCache.h
|   |-- CacheSlot.h
|   |-- DJSession.h
|   |-- DJLibraryService.h
|   |-- DJControllerService.h
|   |-- MixingEngineService.h
|   |-- ConfigurationManager.h
|   `-- SessionFileParser.h
|-- src/                        # C++ implementation files
|-- bin/                        # Build output and runtime config location
|   |-- dj_manager              # Built executable, when present
|   `-- dj_config.txt           # Runtime configuration expected by DJSession
|-- input_2/
|   `-- dj_config.txt           # Sample/generated DJ configuration
|-- Makefile                    # Build, run, clean, and diagnostic targets
|-- test_output.txt             # Captured output artifact
`-- interactive_*_output*.txt   # Captured interactive output artifacts
```

## Architecture

The code is organized around a small service-oriented simulation:

- `AudioTrack` owns common track metadata and waveform memory. `MP3Track` and `WAVTrack` implement loading, beatgrid analysis, quality scoring, and polymorphic cloning.
- `Playlist` stores ordered track pointers and supports adding, removing, displaying, and traversal.
- `DJLibraryService` owns the configured library tracks and builds playlists from 1-based track indices.
- `DJControllerService` wraps `LRUCache` and handles controller memory behavior.
- `LRUCache` stores cloned tracks in `CacheSlot` instances and evicts the least recently used slot when full.
- `MixingEngineService` owns two mixer decks, clones cached tracks into decks, calls `load()` and `analyze_beatgrid()`, and switches the active deck.
- `DJSession` orchestrates configuration loading, playlist selection, cache loading, deck loading, and session summary statistics.
- `SessionFileParser` parses the main key/value configuration format used by `bin/dj_config.txt`.

## Installation

Install a C++ compiler and Make. On Ubuntu/Debian:

```bash
make install-deps
```

Or install the equivalent tools manually:

- `g++`
- `make`
- `valgrind` for leak checks, optional
- `gdb` for debugging, optional

## Build

Build the default executable:

```bash
make
```

Build with debug flags:

```bash
make debug
```

Build with release flags:

```bash
make release
```

The executable is written to:

```text
bin/dj_manager
```

## Configuration

`DJSession` loads configuration from:

```text
bin/dj_config.txt
```

If you want to use the provided sample config, copy or keep its contents in `bin/dj_config.txt`.

Supported configuration keys inferred from the parser:

```text
app_name=DJ Track Library Manager
version=1.0
library_track_1=MP3,title,{artist1;artist2;},duration_seconds,bpm,bitrate,has_tags
library_track_2=WAV,title,{artist1;artist2;},duration_seconds,bpm,sample_rate,bit_depth
controller_cache_size=8
bpm_tolerance=10
auto_sync=true
playlist_name=1,2,3
```

Notes:

- `library_track_N` entries define the available track library.
- Playlist entries map a playlist name to 1-based indices from the configured library.
- MP3 extra parameters are bitrate and whether ID3 tags exist.
- WAV extra parameters are sample rate and bit depth.
- `controller_cache_size`, `bpm_tolerance`, and `auto_sync` affect the session simulation.
- `default_crossfade_time` exists in `SessionConfig`, but current session behavior uses an instant-transition model.

No environment variables are required.

## Usage

Run the built-in assignment/demo checks:

```bash
./bin/dj_manager
```

Run the interactive DJ session and choose a playlist from the terminal menu:

```bash
./bin/dj_manager -I
```

Run the interactive DJ session in automatic "play all playlists" mode:

```bash
./bin/dj_manager -I -A
```

The session prints processing logs, cache status, deck status, and a final summary with processed tracks, cache hits/misses, evictions, deck loads, transitions, and errors.

## Scripts and Commands

```bash
make              # Build the program
make all          # Build the program
make debug        # Build with -DDEBUG
make release      # Build with -DNDEBUG
make test         # Build and run ./bin/dj_manager
make test-leaks   # Build debug target and run Valgrind
make clean        # Remove object files and executable from bin/
make install-deps # Install build-essential, g++, valgrind, and gdb on Ubuntu/Debian
make help         # Show available Makefile targets
make examination  # Runs ./test.sh, if that script exists
```


## Testing

The default executable mode runs assignment-oriented checks for:

- playlist memory behavior
- `AudioTrack` copy and move operations
- custom pointer wrapper move/reset/release behavior
- polymorphic `MP3Track`/`WAVTrack` behavior

Run:

```bash
make test
```

For memory diagnostics with Valgrind:

```bash
make test-leaks
```

## Deployment

There is no deployment pipeline in this repository. The project is intended to be built and run locally as a command-line program.

For a local release-style build:

```bash
make clean
make release
```

Then run:

```bash
./bin/dj_manager -I -A
```

## Contributing

1. Keep changes focused on the relevant class or service.
2. Rebuild with `make debug` after source changes.
3. Run `make test` to check the assignment/demo flow.
4. Run `make test-leaks` when changing ownership, destructors, copy/move logic, cache behavior, or deck loading.
5. Preserve the existing C++11 style and Makefile-based workflow unless the project requirements change.
