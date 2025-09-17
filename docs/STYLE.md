# Blockforge C++ Style Guide

This repository follows a pragmatic, performance-focused style:

* **Language Standard**: C++23. Prefer modern facilities (`std::expected`, `std::span`, `std::ranges`).
* **Error Handling**: Hot paths return `tl::expected`/`StatusOr`-like results instead of exceptions. Only throw during initialization failures.
* **Naming**:
  * Types and namespaces: `PascalCase`.
  * Functions and variables: `camelCase`.
  * Constants and macros: `SCREAMING_SNAKE_CASE`.
* **Headers**: Keep headers self-contained. Use forward declarations to minimize includes.
* **Ownership**: Favor smart pointers. Document lifetimes for raw pointers explicitly.
* **Logging**: Initialize logging through `bf::log::initialize` and use `spdlog` macros via subsystem-specific loggers
  (`[DX12]`, `[WORLD]`, etc.).
* **Threading**: Clearly annotate thread affinity and synchronization primitives.
* **Testing**: Every subsystem exposes a diagnostic self-test runnable via Catch2.
* **Formatting**: Enforced via `.clang-format` and `.editorconfig`.

When in doubt, document deviations in the affected source file and update this guide.
