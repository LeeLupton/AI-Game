# Blockforge Error Message Catalog

This document centralizes human-readable diagnostics. Every subsystem should emit errors using the canonical format. Use
`bf::formatError` from `Blockforge/Core/Error.h` to guarantee consistency when logging or surfacing errors to users.

```
Category | Code | Human message | Context
```

## DirectX 12 Device

* `DX12|CreateDevice|0x%08X|CreateDevice failed for adapter '%s'. Enable GPU debug layer or update drivers. %s`

Use `bf::makeDxError("CreateDevice", hr, context)` to construct errors that automatically translate the `HRESULT` into a human
-readable message on Windows builds.

## Shader Compilation

* `SHADER|Compile|<file>:<entry>/<profile>|Compile failed. See log at <path>. Common fixes: check define set, include dirs, target profile.`

## File I/O

* `IO|Open|%s|Failed to open file '%s' (%s). Check path/permissions/work dir.`

## Texture Atlas

* `ASSET|Atlas|%s|UV mismatch for tile '%s'. Expected %dx%d, got %dx%d. Adjust atlas.json.`

Additional subsystem-specific errors should extend this list with actionable remediation hints.
