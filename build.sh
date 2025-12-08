#!/bin/bash

# --- Configuration Variables ---
BUILD_DIR="build"
SRC_DIR="src"
TESTS_DIR="tests"
INCLUDE_DIR="include"
BIN_DIR="bin"

# Common GCC flags (warnings, debug info, include path)
# CFLAGS="-Wall -Wextra -g -I$INCLUDE_DIR"
CFLAGS="-g -I$INCLUDE_DIR"

# Linker Flags:
# Use pkg-config to get necessary SDL2 flags (cflags and libs)
SDL_FLAGS=$(pkg-config --cflags --libs sdl2)

# Add the math library linker flag
MATH_FLAG="-lm"

# Combine all linker flags
LDFLAGS="$SDL_FLAGS $MATH_FLAG"

# Ensure the build directory exists
mkdir -p $BUILD_DIR $BIN_DIR

# --- Build Functions ---

build_main() {
  APP_NAME="render"
  echo "--- Building Main Application: $APP_NAME ---"

  # Compile the main application source files
  APP_SRC_FILES=$(find $SRC_DIR -name "*.c" ! -name "*test*.c" ! -name "test_*.c")

  # Compile and link
  gcc $CFLAGS $APP_SRC_FILES -o $BUILD_DIR/$APP_NAME $LDFLAGS

  if [ $? -eq 0 ]; then
    mv $BUILD_DIR/$APP_NAME $BIN_DIR/$APP_NAME
    echo "Main application build successful! Executable is in ./bin/$APP_NAME"
  else
    echo "Main application build failed!"
    exit 1
  fi
}

build_tests() {
  TEST_EXEC="testexec"
  echo "--- Building Tests: $TEST_EXEC ---"
  LIB_SRC_FILES=$(find $SRC_DIR -name "*.c" ! -name "main.c")
  TEST_SRC_FILES=$(find $TESTS_DIR -name "*.c")

  # Compile and link the library code with the test runner's main() function
  gcc $CFLAGS $LIB_SRC_FILES $TEST_SRC_FILES -o $BUILD_DIR/$TEST_EXEC $LDFLAGS

  if [ $? -eq 0 ]; then
    echo "Tests build successful!"
    echo "To run: ./build/$TEST_EXEC"
  else
    echo "Tests build failed!"
    exit 1
  fi
}

# --- Command Line Argument Handling ---

case "$1" in
main)
  build_main
  ;;
tests)
  build_tests
  ;;
*)
  echo "Usage: ./build.sh [main|tests]"
  echo "Example: ./build.sh main  (Builds the main 'render' application)"
  echo "Example: ./build.sh tests (Builds the 'TestRunner' executable)"
  exit 1
  ;;
esac
