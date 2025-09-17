param(
    [ValidateSet('Debug', 'RelWithDebInfo')]
    [string]$Config = 'Debug'
)

$ErrorActionPreference = 'Stop'

$Preset = if ($Config -eq 'Debug') { 'windows-debug' } else { 'windows-relwithdebinfo' }

Write-Host "[build] Configuring preset '$Preset'"
cmake --preset $Preset

Write-Host "[build] Building"
cmake --build --preset $Preset
