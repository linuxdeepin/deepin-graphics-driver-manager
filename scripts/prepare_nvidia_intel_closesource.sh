#!/bin/sh

COMMANDS=(
    "true"
    "apt update"
    "ls -la"
)

for cmd in "${COMMANDS[@]}"
do
    command ${cmd};

    if [ $? != 0 ]; then
        echo "### Failed in command '${cmd}'"
        exit -1;
    fi
done