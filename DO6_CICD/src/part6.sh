#!/bin/bash
TOKEN="8170816945:AAFmX7kkjAyVdEfSL09EgClJpQeaERQDE2Q"
ID="506202294"

sleep 2
if [ "$CI_JOB_STATUS" == "success" ]; then
  MESSAGE="✅ $CI_JOB_NAME SUCCESS ✅"
else
  MESSAGE="❌ $CI_JOB_NAME FAIL ❌"
fi
curl -X POST "https://api.telegram.org/bot$TOKEN/sendMessage" -d chat_id=$ID -d text="$MESSAGE"