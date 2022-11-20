
@echo off
SetLocal EnableDelayedExpansion

pushd src/
set sourceFiles=
for /R %%f in (*.c) do (
    set sourceFiles=!sourceFiles! %%f
)
popd

set includeDirs=-Isrc/ 
set linkingFlags=-luser32
set compilerFlags=-fuse-ld=llvm-lib -g -shared -Wvarargs -Wall -o ../bin/cengine.lib
set preDefinedMacros=-D_DEBUG -D_CRT_SECURE_NO_WARNINGS

echo Starting Cengine build process...

clang %sourceFiles% %includeDirs% %linkingFlags% %compilerFlags% %preDefinedMacros%