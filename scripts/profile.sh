#!/bin/bash
# CPU Profiler for BasicRenderer using DTrace

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
OUTPUT_DIR="$PROJECT_DIR/output"
BINARY="$PROJECT_DIR/build/bin/BasicRenderer"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
OUTPUT_FILE="$OUTPUT_DIR/dtrace_profile_$TIMESTAMP.txt"

echo "=== BasicRenderer CPU Profiler ==="
echo "Binary: $BINARY"
echo "Output: $OUTPUT_FILE"
echo ""

if [ ! -f "$BINARY" ]; then
    echo "Error: Binary not found at $BINARY"
    echo "Please build the project first."
    exit 1
fi

mkdir -p "$OUTPUT_DIR"

echo "Running DTrace profiler (requires sudo password)..."
echo ""

sudo dtrace -x ustackframes=100 \
    -n 'profile-997 /execname == "BasicRenderer"/ { @[ustack()] = count(); }' \
    -c "$BINARY" \
    > "$OUTPUT_FILE" 2>&1

if [ $? -eq 0 ]; then
    echo ""
    echo "Profile saved to: $OUTPUT_FILE"
    echo ""
    echo "Top functions by sample count:"
    echo "------------------------------"
    tail -50 "$OUTPUT_FILE"
else
    echo "Profiling failed. Check if SIP allows DTrace."
    echo "You may need to partially disable SIP for DTrace to work."
fi
