# Call J-Flash hidden and stream the raw log to the terminal
param(
    [string]$JFlashExe,
    [string]$Project,
    [string]$Hex,
    [string]$LogFile
)

# Remove the old log to avoid mixing runs
if (Test-Path -LiteralPath $LogFile) {
    Remove-Item -LiteralPath $LogFile -Force
}

# J-Flash options must be joined to their paths: -openprj<file>, -open<file>
$arguments = "-hide -openprj`"$Project`" -open`"$Hex`" -auto -startapp -exit -jflashlog`"$LogFile`""

# Start J-Flash hidden and keep the process object
$process = Start-Process -FilePath $JFlashExe -ArgumentList $arguments -PassThru -WindowStyle Hidden

# Tail jflash.log in the background and print every line unchanged
$tailJob = Start-Job -ScriptBlock {
    param($path)
    Get-Content -LiteralPath $path -Wait -Tail 0
} -ArgumentList $LogFile

# Print new log lines while J-Flash is running
while (-not $process.HasExited) {
    Receive-Job $tailJob
    Start-Sleep -Milliseconds 200
}

# Read the final log lines and stop the background job
Receive-Job $tailJob
Stop-Job $tailJob
Remove-Job $tailJob -Force

# Return J-Flash's exit code to CMake
$process.WaitForExit()
exit $process.ExitCode
