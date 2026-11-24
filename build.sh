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

case "${1:-}" in
    "")
        "$compiler" "${flags[@]}" "${sources[@]}" app/source.cpp ${libraries[@]+"${libraries[@]}"} -o build/lab_3
        ;;
    --test)
        "$compiler" "${flags[@]}" "${sources[@]}" tests/lab_3_test.cpp ${libraries[@]+"${libraries[@]}"} -o build/lab_3_test
        ./build/lab_3_test
        ;;
    *)
        echo "Использование: bash build.sh [--test]" >&2
        exit 1
        ;;
esac
