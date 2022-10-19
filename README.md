# CMSC421: Project 1 - Eventual Consistency
- Based on https://www.youtube.com/watch?v=RY_2gElt3SA&t=4s&ab_channel=TomScott
- Tasked to create 10 Like Servers using forked processes
- These servers will provide a random number of likes that will then
  get add onto the Primary Like server
- Eventually, all of these processes will provide the total amount of likes garnered within
  an amount of time

**Constraints and Conditions**
- Each like server is only up for 300 seconds
- Each like server will transmit a random-generated number from 0 - 42 after an interval of time
- From the previous condition, each interval should be randomly-generated as well from 1 - 5
- IPC method between the parent process and the like servers should be bidirectional
- Log every successful/failed processes in their respective .log files

