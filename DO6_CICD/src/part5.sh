#!/bin/bash

USER="edr" 
HOST="192.10.10.2"
DIR="/usr/local/bin"

scp ./cat/s21_cat "$USER@$HOST:$DIR"
if [ $? -ne 0 ]; then
echo "Ошибка"
exit 1
fi

scp ./grep/s21_grep "$USER@$HOST:$DIR"
if [ $? -ne 0 ]; then
echo "Ошибка"
exit 1
fi
