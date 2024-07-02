# Page replacement simulator
Written July 2024, Benjamin Cash

## Compile
gcc -o sim simulator.c

## Usage
./sim < input_file

## Expected Output
#### input_file
```
Optimal page faults: 0.330000
FIFO page faults: 0.520000
Second-Chance page faults: 0.480000
```
#### input_file2
```
Optimal page faults: 0.253906
FIFO page faults: 0.503906
Second-Chance page faults: 0.513672
```
#### input_file3
```
Optimal page faults: 0.638889
FIFO page faults: 0.842593
Second-Chance page faults: 0.851852
```
#### input_file4
```
Optimal page faults: 0.153320
FIFO page faults: 0.361328
Second-Chance page faults: 0.359375
```