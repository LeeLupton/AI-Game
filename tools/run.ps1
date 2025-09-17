param(
    [ValidateSet('Debug', 'RelWithDebInfo')]
    [string]$Config = 'Debug'
)

$ErrorActionPreference = 'Stop'

$Preset = if ($Config -eq 'Debug') { 'windows-debug' } else { 'windows-relwithdebinfo' }
$BinaryDir = Join-Path $PSScriptRoot "..\out\build\$Preset"
$Executable = Join-Path $BinaryDir "Blockforge.exe"

if (-not (Test-Path $Executable)) {
    Write-Host "[run] Executable not found. Building first..."
    & "$PSScriptRoot\build.ps1" -Config $Config
}

Write-Host "[run] Launching $Executable"
& $Executable
