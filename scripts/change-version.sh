#!/usr/bin/env bash

sed -i "" 's/\"version\": \"[0-9]*.[0-9]*.[0-9]*/&-'"$OS"'/' "$PACKAGE_JSON"
