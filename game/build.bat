
@echo off

SetLocal EnableDelayedExpansion

pushd src/
set sourceFiles=
for /R %%f in (*.c) do (
    set sourceFiles=!sourceFiles! %%f
)
popd

set includeDirs=-Isrc -I../engine/src -I../engine/vendor
set linkingFlags=-L../bin -lcengine
set compilerFlags=-g -Wvarargs -Wall -o ../bin/cengine-game.exe -O3 
set preDefinedMacros=-D_DEBUG -D_CRT_SECURE_NO_WARNINGS

echo Starting Cengine-Game build process...

clang %sourceFiles% %includeDirs% %linkingFlags% %compilerFlags% %preDefinedMacros%