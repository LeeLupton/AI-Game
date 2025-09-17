# Project Kanban — “Blockforge C++” (Win11 x64)

> **Usage**
>
> * Move items between columns (`Backlog → In Progress → Review/Debug → Polish → Done`).
> * Check subtasks as you complete them.
> * Add error messages next to bullets like: `→ on failure: "DX12: CreateDevice failed: …"`
> * When you touch code, link the PR/commit under the card.

---

## 🗂️ Backlog (Plan & Setup)

* **Project Skeleton & Toolchain**

  * [x] Initialize repo with `CMake` presets (`Debug`, `RelWithDebInfo`), MSVC v143, C++23.
  * [x] Integrate **vcpkg** manifest (`vcpkg.json`) for dependencies.
  * [x] Continuous integration: GitHub Actions (build + unit tests), artifact upload.
  * [x] Coding standards: `.editorconfig`, ClangFormat, Clang-Tidy config.
  * [x] Decide API: **DirectX 12** (primary) with optional abstraction layer for future Vulkan.
  * [x] Logging & errors: **spdlog** + `fmt`; centralized `Error.h/.cpp` helpers.
  * [x] Standard error shape: `Category | Code | Human message | Context`.
  * [x] Use `HRESULT` translators (Windows `FormatMessage`) for DX failures.
  * [ ] Logging & errors: **spdlog** + `fmt`; centralized `Error.h/.cpp` helpers.

  * [ ] Standard error shape: `Category | Code | Human message | Context`.
  * [ ] Use `HRESULT` translators (Windows `FormatMessage`) for DX failures.
* **Dependencies (via vcpkg)**

  * [ ] `spdlog`, `fmt`, `glm`, `stb`, `entt` (ECS), `fastnoise2`, `cgltf` (optional), `tracy` (profiler), `rxcpp` (optional), `catch2` (tests).
* **Windows Platform Layer**

  * [ ] Windowing & input via **SDL2** or **GLFW** (choose one; SDL2 recommended for controller support).
  * [ ] High-DPI support, resize events, correct swapchain handling.
  * [ ] Gamepad + keyboard/mouse input mapping (Minecraft-like binds).
* **Build/Run UX**

  * [ ] `tools/` scripts: one-click setup, shader compile, asset packer.
  * [ ] Crash dumps via `DbgHelp` (minidumps), with symbol store config.

---

## 🔨 In Progress (Core Engine)

* **Rendering Baseline (DX12)**

  * [ ] Device/adapter selection, debug layer on in `Debug`, GPU validation toggle.

  * → on failure: `"DX12: CreateDevice failed (0x%08X): %s"`
  * [ ] Command queues, swapchain, descriptor heaps, fence sync, frame graph.
  * [ ] Shader toolchain: HLSL (`dxc`) + reflection; hot-reload in dev builds.
  * [ ] **Texture atlas** loader (PNG via `stb_image`) with exact **MC Java UV layout**.
  * [ ] **Block meshing** (greedy meshing) with per-face culling & correct UVs.
  * [ ] **Fixed-function look**: nearest filtering, no mip blur on pixel art, gamma-correct sRGB.
* **Lighting & The “MC Java” Look**

  * [ ] Vertex AO (Minecraft-style ambient occlusion) baked during meshing.
  * [ ] Simple directional sun light + fog; **sky gradient** and **cube-map skybox**.
  * [ ] Day/Night cycle (celestial angle), color grading to match MC vibe.
* **World & Chunks**

  * [ ] Chunk system: **16×16×256** columns; sectioned storage (16×16×16).
  * [ ] Generation: **FastNoise2** (OpenSimplex/Perlin) for terrain heightmaps/biomes.
  * [ ] Async meshing & streaming threads; job system; main-thread render submission.
  * [ ] Save format: simple **region files** (Anvil-like layout) with compression.
  * [ ] Configurable seed; deterministic regen.
* **Player, Camera, Physics**

  * [ ] FPS camera with MC-like sensitivity & smoothing.
  * [ ] Player **AABB** collisions (stairs/edges), step height, gravity, jump.
  * [ ] Block placement/breaking with raycast; reach distance & cooldown.
* **UI & UX**

  * [ ] Minimal MC-like HUD: crosshair, hotbar, hearts/food (placeholders allowed).
  * [ ] Inventory: grid UI, item stacks, tooltips, drag/drop.
  * [ ] Pause menu (Resume, Options, Controls, Quit).
* **Audio**

  * [ ] Load/stream OGG/WAV; footsteps, break/place, ambient.
  * [ ] Volume groups (SFX, ambient, UI).

---

## 🔎 Review/Debug (Testing, Profiling, Errors)

* **Error Handling (Every Subsystem)**

  * [ ] **Never** swallow failures. All returns checked. Clear user-facing error text.
  * [ ] DX12 wrappers: `CHECK_HR(hr, "DX12: Resource creation failed: %s")`.
  * [ ] File I/O: show path, mode, errno/Win32 error; suggest remediation if obvious.
  * [ ] Shader compile: print entry, profile, full `dxc` log path.
  * [ ] Asset load: validate dimensions/format; show atlas coords on mismatch.
* **Debugging Tooling**

  * [ ] **Visual Studio** Graphics Diagnostics / **PIX** / **RenderDoc** capture steps doc.
  * [ ] GPU markers (PIX events) per pass/chunk build.
  * [ ] CPU profiler: **Tracy** zones; frame budget targets (CPU < 16ms; GPU < 16ms).
  * [ ] Static analysis: Clang-Tidy; warnings as errors in CI.
  * [ ] Sanitizers (Clang on Windows): ASan/UBSan in dev config if feasible.
* **Unit & Integration Tests**

  * [ ] **Catch2** test suite for: greedy meshing, AO weights, noise determinism, save/load round-trip, AABB collision edge cases.
  * [ ] Headless renderer smoke test (device init + clear + teardown).
* **Performance Targets**

  * [ ] Chunks: mesh time budget per chunk < 2ms on i9/RTX 3060.
  * [ ] View distance tuning; frustum + occlusion hints (coarse).
  * [ ] Memory: per-chunk vertex/index buffers upper bound documented.

---

## 🎨 Polish (MC Java Faithfulness)

* **Visual Fidelity**

  * [ ] Texture atlas parity with MC Java (nearest sampling, correct tiling, no filtering bleed).
  * [ ] Fog color tables and skylight curve tuned to MC feel.
  * [ ] Block models for non-cubes (slabs, stairs, fences) — minimal initial set.
  * [ ] Particles: block break, placement puff, rain/snow.
* **Gameplay Feel**

  * [ ] FOV handling like MC (70 default), smooth sprint FOV change toggle.
  * [ ] Mouse sensitivity curve mimicking MC Java.
  * [ ] Sneak behavior (reduced speed, edge safety).
* **Content & Tools**

  * [ ] Simple **resource pack** loader for textures/UI (folder override).
  * [ ] In-game debug overlay (F3-like): FPS, chunk count, coords, light, seed.
  * [ ] Screenshot key w/ timestamped PNG to `Screenshots/`.

---

## ✅ Done (Move cards here when complete)

* Logging & errors foundation → Completed in this PR.

---

## 📦 Dependencies & Versions (pin in `vcpkg.json`)

* MSVC v143, C++23
* DirectX 12 Agility SDK (if used)
* SDL2 or GLFW (choose one)
* spdlog, fmt, glm, stb, EnTT, fastnoise2, catch2, tracy

---

## 🧭 Agent Operating Rules (put in `CONTRIBUTING.md` too)

* **Plan → Code → Verify → Explain** each change.
* Prefer **no exceptions** in hot paths; return `StatusOr<T>` (or `tl::expected`) with rich error messages. Throw only at app start for “cannot continue” failures.
* Every subsystem must:

  1. Validate inputs; fail fast with actionable messages.
  2. Log at `info/warn/error` with subsystem tags (`[DX12]`, `[IO]`, `[WORLD]`).
  3. Expose a small **diagnostic self-test** callable from a dev menu.
* All shaders and assets must compile in CI. If a shader fails: block merge.
* Ship **symbols** for RelWithDebInfo; generate **minidumps** on crash.
* Keep **frame pacing** sacred. Any PR that regresses frame time > 1ms must include a performance note and mitigation.

---

## 🧰 Concrete Commands & Files (create these now)

* [ ] `CMakePresets.json` with MSVC configs; `ENABLE_GPU_VALIDATION` option for Debug.
* [ ] `vcpkg.json` with deps listed above.
* [ ] `tools/build.ps1` and `tools/run.ps1` (accept `-Config Debug|RelWithDebInfo`).
* [ ] `tools/compile_shaders.ps1` (`dxc` batch with reflection JSON).
* [ ] `docs/ERRORS.md` — catalog canonical error messages & likely fixes.
* [ ] `docs/STYLE.md` — naming, headers, ownership, ECS usage.

---

## 🧪 Error Message Templates (copy into `docs/ERRORS.md`)

* **DX12 Device**

  * `DX12|CreateDevice|0x%08X|CreateDevice failed for adapter '%s'. Enable GPU debug layer or update drivers. %s`
* **Shader Compile**

  * `SHADER|Compile|<file>:<entry>/<profile>|Compile failed. See log at <path>. Common fixes: check define set, include dirs, target profile.`
* **File I/O**

  * `IO|Open|%s|Failed to open file '%s' (%s). Check path/permissions/work dir.`
* **Texture Atlas**

  * `ASSET|Atlas|%s|UV mismatch for tile '%s'. Expected %dx%d, got %dx%d. Adjust atlas.json.`

---

## 📓 Debug Playbook (short)

* If frame drops: open Tracy, check chunk meshing zones → reduce view distance or batch faces.
* If black textures: verify sRGB formats and sampler states; check atlas UVs.
* If crashes on resize: ensure swapchain/RTV/DSV recreate order & fence waits.
* If AO looks wrong: re-run unit tests; confirm neighbor sampling & quad orientation.

---

### Next Steps

1. Duplicate this board into `TASKS.md` at the repo root.
2. Start by completing **Project Skeleton & Toolchain** in Backlog.
3. Keep moving cards rightward and attach commits/PRs under each card.

Build a world out of math and pixels; keep the errors human.
