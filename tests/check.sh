#!/bin/bash

set -e
shopt -s dotglob


base_dir="$(realpath "$(dirname "$0")"/../)"
mybt="${base_dir}"/my-bittorrent
base_tests="${base_dir}/tests/"
cd "${base_tests}"

load () {
  local script_path="$test_path/$1"
  if [ -f "$script_path" ]; then
      . "$script_path"
  fi
}

check_stderr () {
  local len=$(cat "$file_errfile" | wc -l)
  (( len == $(cat "$my_errfile" | wc -l) ))
}

red () {
  echo -e "\033[31;01m$@\033[00m"
}

blue () {
  echo -e "\033[32;01m$@\033[00m"
}

run_test () {
  (
    test_path="$(realpath "$test_path")"
    file_outfile="$(mktemp)"
    file_errfile="$(mktemp)"
    my_outfile="$(mktemp)"
    my_errfile="$(mktemp)"

    success=0
    load args

    if [ -z ${args_ref+x} ]; then
        declare -n args_ref=args
    fi

    tmpdir="$(mktemp -d)"
    cd "$tmpdir"
    load init

    "${args_ref[@]}" > "$file_outfile" 2> "$file_errfile"
    torrent_retcode="$?"

    rm -rf "$tmpdir"/*

    cd "$tmpdir"
    load init

    "$mybt" "${args[@]}" > "$my_outfile" 2> "$my_errfile"
    my_retcode="$?"

    rm -rf "$tmpdir"

    if (( torrent_retcode != my_retcode )); then
        red "mismatched retcode: expected $torrent_retcode, got $my_retcode"
        success=1
    fi
    if ! diff "$file_outfile" "$my_outfile"; then
        red "invalid stdout:"
        red ">> REFERENCE STDOUT"
        cat -e "$file_outfile"
        red ">> MYTORRENT STDOUT"
        cat -e "$my_outfile"
        success=1
    fi
    if ! check_stderr; then
        red "invalid stderr:"
        red ">> REFERENCE ERR"
        cat -e "$file_errfile"
        red ">> MYTORRENT ERR"
        cat -e "$my_errfile"
        success=1
    fi
    rm -rf "$file_outfile" "$file_errfile" \
          "$my_outfile" "$my_errfile"
    return "$success"
  )
}


in_list () {
  [[ -z "$1" ]] || [[ "$1" =~ (^|:)"$2"($|:) ]]
}


for suite_path in suites/*; do
  suite_name="${suite_path##*/}"
  in_list "$SUITES" "$suite_name" || continue
  for test_path in "$suite_path"/*; do
    [ -d "$test_path" ] || continue
    test_name="${test_path##*/}"
    in_list "$TESTS" "$test_name" || continue
    test_id="$suite_name: $test_name"
    if run_test; then
      test_status="$(blue OK)"
    else
      test_status="$(red KO)"
    fi
    echo -e "[$test_status] $test_id"
  done
done
