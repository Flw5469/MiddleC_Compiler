@echo off
echo Step 1: Compiling cfg.cpp...
call build.bat
echo Build.bat exit code: %errorlevel%
if errorlevel 1 (
    echo Compilation failed!
    pause
    exit /b 1
)

echo Compilation successful! Running the flow
echo About to run: .\build\cfg.exe
.\build\cfg.exe
if errorlevel 1 (
    echo cfg.exe returned an error!
    pause
    exit /b 1
)

echo.
echo Step 2: Running make...
make
if errorlevel 1 (
    echo Make failed!
    pause
    exit /b 1
)

echo.
echo Step 3: Running output.exe...
.\build\output\output.exe
if errorlevel 1 (
    echo output.exe returned an error!
    pause
    exit /b 1
)

echo.
echo All steps completed successfully!
pause