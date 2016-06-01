#!/bin/bash

set -e

script_path=`readlink -f "$0"`
script_dir=`dirname "${script_path}"`

cd "${script_dir}/test/"

runtest() {
	echo "$0: running test \"$1\""
	eval "./$1"
}

echo ""

runtest "test_app"

runtest "format"
runtest "point"
runtest "rect"

