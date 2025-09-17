# Blockforge

An experimental C++23 voxel sandbox inspired by Minecraft Java Edition. The goal is to build a faithful DirectX 12 renderer and supporting systems following the roadmap captured in `TASKS.md`.

## Build (Windows)

```powershell
# Configure and build with CMake presets
./tools/build.ps1 -Config Debug

# Run the prototype executable
./tools/run.ps1 -Config Debug
```

The project uses a vcpkg manifest for dependencies. Continuous integration targets MSVC v143 on Windows 11.

## Repository Layout

```
CMakeLists.txt         # Root build definition
CMakePresets.json      # Toolchain and configuration presets
src/                   # Application sources
include/               # Future public headers
tools/                 # PowerShell helpers for build/run/shader compilation
.github/workflows/     # GitHub Actions CI pipeline
```

Refer to `docs/STYLE.md` and `docs/ERRORS.md` for engineering guidelines and diagnostics.
