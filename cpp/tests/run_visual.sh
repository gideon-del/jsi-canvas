#!/bin/bash

set -e
cd "$(dirname "$0")"

INCLUDE="-I../vector-engine/math -I../vector-engine/debugcd"
FLAGS="-std=c++17 -Wall -Wextra"
SOURCES=$(find ../vector-engine -name "*.cpp")
OUTPUT="visual/output"

mkdir -p "$OUTPUT"

echo "Building and running visual tests..."
echo ""

for test_file in visual/test_*.cpp; do
    name=$(basename "$test_file" .cpp)
    echo "Building $name..."
    clang++ $FLAGS $INCLUDE $SOURCES "$test_file" -o "visual/$name"
    echo "Running $name..."
    "./visual/$name"
done

echo ""
echo "Opening output..."

if command -v open &> /dev/null; then
    open "$OUTPUT"/*.svg

elif command -v xdg-open &> /dev/null; then
    for f in "$OUTPUT"/*.svg; do
        xdg-open "$f" &
    done
fi

echo "Done."