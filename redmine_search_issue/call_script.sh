#!/bin/bash

BIN_PYTHON=/usr/bin/python3
RM_URL=http://192.168.22.108/redmine
RM_KEY=your_api_key
RM_PROJNAME=importmail

${BIN_PYTHON} ./redmine_search_issue.py "${RM_URL}" "${RM_KEY}" "${RM_PROJNAME}" 
