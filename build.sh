#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"
mkdir -p build

compiler="${CXX:-clang++}"
flags=(-std=c++20 -Wall -Wextra -Wpedantic -Werror)
sources=(model/student.cpp model/group.cpp model/faculty.cpp app/university.cpp infrastructure/console.cpp)
libraries=()

if [[ "$(uname -s)" != Darwin ]]; then
    libraries+=(-luuid)
fi