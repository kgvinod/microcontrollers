#!/bin/bash

# Run gpsim with your script
gpsim sim_macro.stc | tee sim_output.txt

# Check if the test passed or failed
if grep -q "FAIL" sim_output.txt; then
    echo "Test failed, check gpsim_output.txt for details"
    exit 1
else
    echo "Test passed"
    exit 0
fi

