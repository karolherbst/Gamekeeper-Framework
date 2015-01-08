#!/bin/sh
astyle -A1 --indent=tab=4 --indent-namespaces --lineend=linux --mode=c -n -Q --exclude=third_party -R "*.h" "*.cpp"
