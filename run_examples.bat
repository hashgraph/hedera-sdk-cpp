@echo off

REM Script should be executed from project root
REM Directory containing executable files
set EXECUTABLES_DIRECTORY=build_folder_with_exec_binaries

if exist %EXECUTABLES_DIRECTORY% (
  echo %EXECUTABLES_DIRECTORY% does exist.

  REM Iterate over all executables in the directory
  for %%f in (%EXECUTABLES_DIRECTORY%\*.exe) do (
    if exist "%%f" (
      echo Running %%f...
      
      REM Execute the binary executable without additional arguments
      "%%f"
      
      echo Done
    )
  )
) else (
  echo %EXECUTABLES_DIRECTORY% does not exist.
)

pause
