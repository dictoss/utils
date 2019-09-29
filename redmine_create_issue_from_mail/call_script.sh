#!/bin/bash

BIN_PYTHON=/usr/bin/python3
RM_URL=http://192.168.xx.xxx/redmine
RM_KEY=your_api_key
RM_PROJNAME=yourprojectname

${BIN_PYTHON} ./redmine_create_issue_from_mail.py "${RM_URL}" "${RM_KEY}" "${RM_PROJNAME}" $*

