# Builds with cl.exe (assumes MSVC is in PATH or called from VS Developer environment)
param(
  [string]$Configuration = "Release"
)

$ErrorActionPreference = "Stop"

# Determine workspace root
$ws = $PSScriptRoot | Split-Path -Parent
if (-not $ws) { $ws = "${PWD}" }

# Output paths
$exeOut = Join-Path $ws "chefpp.exe"
$src = Join-Path $ws "main.cpp"

# Compiler flags
$std = "/std:c++17"
$warnings = "/W4 /EHsc"
$opt = if ($Configuration -eq 'Debug') { "/Od /MDd /Zi" } else { "/O2 /MD" }
$includes = "/I`"$ws`""

Write-Host "Building with cl.exe..." -ForegroundColor Cyan
Write-Host "Source: $src" -ForegroundColor Gray
Write-Host "Output: $exeOut" -ForegroundColor Gray

# Build command
$buildCmd = "cl.exe $std $warnings $opt $includes /Fe:`"$exeOut`" `"$src`" /link /INCREMENTAL:NO"

# Execute
Invoke-Expression $buildCmd

if ($LASTEXITCODE -eq 0) {
  Write-Host "`nBuild succeeded!" -ForegroundColor Green
  Write-Host "Executable: $exeOut" -ForegroundColor Green
} else {
  Write-Error "Build failed with exit code $LASTEXITCODE"
}

exit $LASTEXITCODE
