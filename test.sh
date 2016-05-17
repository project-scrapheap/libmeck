#!/bin/bash

set -e

script_path=`readlink -f "$0"`
script_dir=`dirname "${script_path}"`

cd "${script_dir}/test/"

./test_app

