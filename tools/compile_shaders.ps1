param(
    [string]$ShaderDir = "$(Join-Path $PSScriptRoot '..\shaders')",
    [string]$OutputDir = "$(Join-Path $PSScriptRoot '..\out\shaders')"
)

$ErrorActionPreference = 'Stop'

if (-not (Test-Path $ShaderDir)) {
    Write-Warning "[shaders] Directory '$ShaderDir' not found."
    return
}

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

Get-ChildItem -Path $ShaderDir -Filter *.hlsl -Recurse | ForEach-Object {
    $InputPath = $_.FullName
    $RelativePath = Resolve-Path -Relative $_.FullName
    $OutputPath = Join-Path $OutputDir ($_.BaseName + '.cso')

    Write-Host "[shaders] Compiling $RelativePath"
    & dxc.exe -T ps_6_7 -E main -Fo $OutputPath -Qstrip_reflect -Qstrip_debug $InputPath

    if ($LASTEXITCODE -ne 0) {
        throw "Shader compilation failed for $RelativePath"
    }
}
