[CmdletBinding(SupportsShouldProcess, ConfirmImpact = 'High')]
param()

$ErrorActionPreference = 'Stop'

# The script lives in the directory that is mirrored to the SD card.
$sourcePath = (Resolve-Path -LiteralPath $PSScriptRoot).Path
$sourceDrive = [System.IO.Path]::GetPathRoot($sourcePath).TrimEnd('\')

$removableDrives = @(
	Get-CimInstance -ClassName Win32_LogicalDisk |
		Where-Object { $_.DriveType -eq 2 -and $_.DeviceID -ne $sourceDrive }
)

if ($removableDrives.Count -eq 0) {
	throw 'Keine SD-Karte bzw. kein Wechseldatenträger gefunden.'
}

if ($removableDrives.Count -gt 1) {
	$drives = $removableDrives.DeviceID -join ', '
	throw "Mehrere Wechseldatenträger gefunden ($drives). Bitte nur die gewünschte SD-Karte angeschlossen lassen."
}

$destinationPath = "$($removableDrives[0].DeviceID)\"

Write-Host "Quelle:  $sourcePath"
Write-Host "Ziel:    $destinationPath"
Write-Host 'Die SD-Karte wird gespiegelt. Dateien, die nur auf der SD-Karte existieren, werden gelöscht.' -ForegroundColor Yellow

if (-not $PSCmdlet.ShouldProcess($destinationPath, 'SD-Karte spiegeln')) {
	Write-Host 'Abgebrochen.'
	exit 0
}

$robocopyArguments = @(
	$sourcePath
	$destinationPath
	'/MIR'
	'/COPY:DAT'
	'/DCOPY:DAT'
	'/R:2'
	'/W:2'
	'/XJ'
	'/NP'
	'/TEE'
)

& robocopy.exe @robocopyArguments
$robocopyExitCode = $LASTEXITCODE

# Robocopy uses exit codes 0-7 for success and success with differences.
if ($robocopyExitCode -gt 7) {
	throw "Robocopy ist mit Fehlercode $robocopyExitCode fehlgeschlagen."
}

Write-Host "Spiegelung abgeschlossen (Robocopy-Code $robocopyExitCode)." -ForegroundColor Green
