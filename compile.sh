#!/bin/bash

BUILD_DIR="build"
SRC_DIR="src"
TESTS_DIR="tests"
BIN_DIR="bin"

MATH_FLAG="-lm -lbsd"
SDL_FLAG="-lSDL2"
PTHREAD_FLAG="-pthread"
LINK_FLAGS="$MATH_FLAG $PTHREAD_FLAG $SDL_FLAG"

mkdir -p $BUILD_DIR $BIN_DIR

build_main() {
  APP_NAME="exec"
  echo "--- Building Main Application: $APP_NAME ---"

  # Compile and link
  gcc $CFLAGS -I$SRC_DIR $SRC_DIR/main.c -o $BUILD_DIR/$APP_NAME $LINK_FLAGS

  if [ $? -eq 0 ]; then
    mv $BUILD_DIR/$APP_NAME $BIN_DIR/$APP_NAME
    echo "Main application build successful! Executable is in ./bin/$APP_NAME"
  else
    echo "Main application build failed!"
    exit 1
  fi
}

build_test() {
  APP_NAME="test"
  echo "--- Building Main Application: $APP_NAME ---"

  # Compile and link
  gcc $CFLAGS -I$TESTS_DIR $SRC_DIR/main.c -o $BUILD_DIR/$APP_NAME $LINK_FLAGS

  if [ $? -eq 0 ]; then
    mv $BUILD_DIR/$APP_NAME $BIN_DIR/$APP_NAME
    echo "Test application build successful! Executable is in ./bin/$APP_NAME"
  else
    echo "Test application build failed!"
    exit 1
  fi
}

case "$1:$2:$3" in
"":"":"")
  CFLAGS="-g -std=gnu99 -O3 -Wall -Wextra -Werror -fsanitize=address -fsanitize=undefined"
  build_main
  ;;
main:nowarn:"")
  CFLAGS="-g -std=gnu99 -O3"
  build_main
  ;;
main:nowarn:slow)
  CFLAGS="-g -std=gnu99 "
  build_main
  ;;
test:"":"")
  CFLAGS="-g -std=gnu99 -O3 -Wall -Wextra -Werror -fsanitize=address -fsanitize=undefined"
  build_main
  ;;
test:nowarn:"")
  CFLAGS="-g -std=gnu99 -O3"
  build_main
  ;;
test:nowarn:slow)
  CFLAGS="-g -std=gnu99 "
  build_main
  ;;
*)
  echo "Invalid Build Arguments! Valid Arguments Are:"
  echo "./build.sh main  (Builds the main application with strict warnings)"
  echo "./build.sh main nowarn  (Builds the main application with looser warnings)"
  exit 1
  ;;
esac
