# KTH-tic-tac-toe

This repository contains code for a hardware Tic Tac Toe-client with built in AI.

## Directories

* `ui-client` <br>
Contains the code for the interaction of the game.
* `ai-client`<br>
Contains the code for the AI the player will compete against.
* `shared`<br>
Contains the code that will be shared between the clients (ie game board and protocol).

## GPIO Mapping for Chipkit UNO32
### J5
* 2: D: `0b0100000000`
* 3: D: `0b1`
* 4: F: `0b10`
* 5: D: `0b10`
* 6: D: `0b100`
* 7: D: `0b1000000000`
* 26: E: `0b1`
* 27: E: `0b10`
* 28: E: `0b100`
* 29: E: `0b1000`
* 30: E: `0b10000`
* 31: E: `0b100000`
* 32: E: `0b1000000`
* 33: E: `0b10000000`

### J6
* 8: D: `0b10000000000`
* 9: D: `0b1000`
* 10: G: `0b1000000000`
* 11: G: `0b100000000`
* 12: G: `0b10000000`
* 13: G: `0b1000000`
* A: B: `0b1`
* 34: D: `0b100000`
* 35: D: `0b100000000000`
* 36: D: `0b1000000`
* 37: D: `0b10000000`
* 38: F: `0b1000000`
* 39: F: `0b10000`
* 40: F: `0b100000`
* 41: B: `0b10`

### J7 (Analog)
* A0: B: `0b100`
* A1: B: `0b10000`
* A2: B: `0b100000000`
* A6: B: `0b1000`
* A7: B: `0b100000`
* A8: B: `0b1000000000`
* A11: B: `0b1000000000000000`
