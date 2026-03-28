#!/bin/bash

echo "Starting Monitor Client (runs in background for 5 seconds)..."
# Start monitor client, subscribe for 5 seconds.
echo -e "6\n5\n0\n" | ./client_app 127.0.0.1 2222 1 > monitor_output.txt 2>&1 &

# Sleep for 1 second to make sure it registers
sleep 1

echo "Starting Action Client..."
cat << EOF > actions.txt
1
Alice
pass
1
500.0
4
Alice
1000
pass
3
Alice
1000
pass
1
250.0
1
Bob
pass
1
100.0
5
Alice
1000
pass
1001
200.0
2
Alice
1000
pass
0
EOF

./client_app 127.0.0.1 2222 1 < actions.txt > action_output.txt 2>&1

echo "Action Client finished."

# Wait for monitor to finish its 5 seconds
sleep 5

echo "=== Monitor Output ==="
cat monitor_output.txt

echo "=== Action Output ==="
cat action_output.txt
