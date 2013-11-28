#!/bin/sh

CMD_SSH=/usr/bin/ssh
CMD_SSH_OPT="-C -N -f vpsport"
CMD_PS=/bin/ps
CMD_GREP=/usr/bin/grep

EXISTSTR=`${CMD_PS} ax | ${CMD_GREP} "${CMD_SSH}" | ${CMD_GREP} -v "grep"`
if [ -z "${EXISTSTR}" ]; then
  #connect
  ${CMD_SSH} ${CMD_SSH_OPT}
  if [ $? -eq 0 ]; then
    echo `date` " : ssh connect success."
    exit 0
  else
    echo `date` " : ERROR ssh connect."
    exit 1
  fi
else
  #echo "already ssh connection."
  exit 0
fi
