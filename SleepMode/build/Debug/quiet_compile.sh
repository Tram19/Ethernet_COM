#!/bin/bash
OUTPUT=$("$@" 2>&1)
STATUS=$?
if [ -n "$VERBOSE_BUILD" ]; then
    echo "$OUTPUT"
elif [ $STATUS -ne 0 ]; then
    echo "$OUTPUT" | grep -B3 -A10 "error:"
fi
exit $STATUS
