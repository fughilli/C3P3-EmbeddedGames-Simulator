#!/bin/bash

CNCJS_PORT=8001

GRBL_SIM_LOCATION=$HOME/Projects/git_checkouts/grbl/grbl/grbl-sim
JOG_SERVER_LOCATION=$HOME/Projects/host_workspace
JOG_SERVER_TARGET=//jog_controller_server:jog_controller_server
JOG_SERVER_ARGS="--cncjs_host=127.0.0.1 --cncjs_port=$CNCJS_PORT --cncjs_serial_port=/dev/ttyS0 --cncjs_serial_baud=115200"

sudo echo

function echoerr() {
  echo -e "***** " "$@" 1>&2
}

function ctrl_c() {
  echoerr "Caught Ctrl-C; cleaning up."
  kill -s INT $GRBL_SIM_PID
  kill -s INT $CNC_PID
  kill -s INT $JOG_SERVER_PID
}

trap ctrl_c INT

function run_grbl_sim() {
  cd $GRBL_SIM_LOCATION
  sudo ./run_sim.sh &
  GRBL_SIM_PID=$!
}

function run_cncjs() {
  cncjs -p $CNCJS_PORT &
  CNC_PID=$!
}

function run_jog_server() {
  cd $JOG_SERVER_LOCATION
  bazelisk run $JOG_SERVER_TARGET -- $JOG_SERVER_ARGS &
  JOG_SERVER_PID=$!
}

echo "Run GRBL Simulator..."
run_grbl_sim
echo "Run cncjs..."
run_cncjs
sleep 2
echo "Run Jog Controller Server..."
run_jog_server

wait $GRBL_SIM_PID
wait $CNC_PID
wait $JOG_SERVER_PID
