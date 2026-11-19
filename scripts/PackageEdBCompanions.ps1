# Package the exact EDB ROM and keyboard assets from the established release archive.
# Do not silently substitute stock ROMs or synthesize a keymap.
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$destination = Join-Path $root '__bin/Win32/Release'
$archiveUrl = 'https://github.com/neuroviewalex/VCC-EDB/releases/download/v2.1.9.32/CoCo3_EDB.rar'
$expectedSha256 = 'cac381e869ffb4fb0af1d691ab29ce37b19334c46842a5389c8c782bca0caf80'
$required = @('coco3_201_edb.rom', 'disk_211_edb.rom', 'custom.keymap')
$sevenZip = Join-Path $env:ProgramFiles '7-Zip/7z.exe'
if (-not (Test-Path -LiteralPath $sevenZip)) { throw '7-Zip not found on Windows runner' }
if (-not (Test-Path -LiteralPath $destination)) { throw "Build output missing: $destination" }
$temp = Join-Path $env:RUNNER_TEMP 'vcc-edb-companions'
New-Item -ItemType Directory -Force -Path $temp | Out-Null
$archive = Join-Path $temp 'CoCo3_EDB.rar'
$extracted = Join-Path $temp 'extracted'
New-Item -ItemType Directory -Force -Path $extracted | Out-Null
Invoke-WebRequest -Uri $archiveUrl -OutFile $archive -MaximumRedirection 10
$actualSha256 = (Get-FileHash -LiteralPath $archive -Algorithm SHA256).Hash.ToLowerInvariant()
if ($actualSha256 -ne $expectedSha256) {
    throw "EDB source archive SHA-256 mismatch: $actualSha256"
}
& $sevenZip x "-o$extracted" -y $archive | Out-Null
if ($LASTEXITCODE -ne 0) { throw "Cannot extract verified EDB archive (7z exit $LASTEXITCODE)" }
foreach ($name in $required) {
    $matches = @(Get-ChildItem -LiteralPath $extracted -Recurse -File | Where-Object { $_.Name -ieq $name })
    if ($matches.Count -ne 1) {
        throw "Expected exactly one $name in verified EDB archive; found $($matches.Count). Release packaging stopped."
    }
    Copy-Item -LiteralPath $matches[0].FullName -Destination (Join-Path $destination $name) -Force
    if ((Get-Item -LiteralPath (Join-Path $destination $name)).Length -eq 0) {
        throw "Packaged file is empty: $name"
    }
    Write-Host "$name SHA256: $((Get-FileHash -LiteralPath (Join-Path $destination $name) -Algorithm SHA256).Hash)"
}
if (-not (Test-Path -LiteralPath (Join-Path $destination 'Vcc.exe'))) {
    throw 'Compiled Vcc.exe missing; refusing to publish incomplete portable package'
}
Write-Host 'EDB portable release package verified.'
