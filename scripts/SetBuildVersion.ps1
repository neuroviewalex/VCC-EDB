# Synchronize VCC-EDB Windows resources and startup version with the release tag.
# A tag build uses GITHUB_REF_NAME (vX.Y.Z.W); local/branch builds use VERSION.txt.
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$versionFile = Join-Path $root 'VERSION.txt'
$version = (Get-Content -LiteralPath $versionFile -Raw).Trim()
if ($env:GITHUB_REF_TYPE -eq 'tag') {
    $version = $env:GITHUB_REF_NAME
}
$version = $version.TrimStart('v')
if ($version -notmatch '^([0-9]+)\.([0-9]+)\.([0-9]+)\.([0-9]+)$') {
    throw "Invalid VCC-EDB version '$version'; expected X.Y.Z.W"
}
$numbers = $version.Replace('.', ',')
$title = "VCC-EDB v$version"

function Replace-Exactly {
    param([string]$Text, [string]$Pattern, [string]$Replacement, [string]$Description)
    $regex = [regex]::new($Pattern, [System.Text.RegularExpressions.RegexOptions]::Multiline)
    $count = $regex.Matches($Text).Count
    if ($count -ne 1) { throw "$Description: expected one occurrence, found $count" }
    return $regex.Replace($Text, { param($match) $match.Result($Replacement) }, 1)
}

$rcPath = Join-Path $root 'Vcc.rc'
$rc = [System.IO.File]::ReadAllText($rcPath)
$rc = Replace-Exactly $rc '(?m)^(\s*FILEVERSION\s+)\d+,\d+,\d+,\d+' ('${1}' + $numbers) 'FILEVERSION'
$rc = Replace-Exactly $rc '(?m)^(\s*PRODUCTVERSION\s+)\d+,\d+,\d+,\d+' ('${1}' + $numbers) 'PRODUCTVERSION'
$rc = Replace-Exactly $rc '(?m)^(\s*VALUE "FileVersion",\s*")\d+\.\d+\.\d+\.\d+(")' ('${1}' + $version + '${2}') 'FileVersion string'
$rc = Replace-Exactly $rc '(?m)^(\s*VALUE "ProductVersion",\s*")\d+\.\d+\.\d+\.\d+(")' ('${1}' + $version + '${2}') 'ProductVersion string'
$rc = Replace-Exactly $rc '(?m)^(\s*IDS_APP_TITLE\s+")VCC-EDB v\d+\.\d+\.\d+\.\d+(")' ('${1}' + $title + '${2}') 'window title'
$rc = Replace-Exactly $rc '(?m)^(\s*LTEXT\s+")VCC-EDB v\d+\.\d+\.\d+\.\d+(",IDC_STATIC)' ('${1}' + $title + '${2}') 'About dialog'
[System.IO.File]::WriteAllText($rcPath, $rc, [System.Text.UTF8Encoding]::new($false))

$configPath = Join-Path $root 'config.cpp'
$config = [System.IO.File]::ReadAllText($configPath)
$config = Replace-Exactly $config '(?m)^([ \t]*"VCC )\d+\.\d+\.\d+\.\d+( Tandy Color Computer 3 Emulator")' ('${1}' + $version + '${2}') 'INI version'
[System.IO.File]::WriteAllText($configPath, $config, [System.Text.UTF8Encoding]::new($false))
Write-Host "VCC-EDB build version: $title"
