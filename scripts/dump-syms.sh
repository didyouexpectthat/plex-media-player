#!/bin/sh

DUMP_SYMS=$1
COMPRESS=$2
SOURCE=$3
TARGET=$4

echo $SOURCE

DSYM_ARG=""
DSYM_FILE=""
BASESOURCE=$(basename "$SOURCE")
if [ -e "$BASESOURCE.dSYM" ]; then
  DSYM_ARG="-g"
  DSYM_FILE="$BASESOURCE.dSYM"
fi

echo $DUMP_SYMS $DSYM_ARG $DSYM_FILE $SOURCE | $COMPRESS > $TARGET
$DUMP_SYMS $DSYM_ARG "$DSYM_FILE" "$SOURCE" | $COMPRESS > "$TARGET"