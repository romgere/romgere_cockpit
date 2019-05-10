**WIP**

# Case of use

The `XXX` class goal is to be bind with an input data (or group of data) from X-Plane to turn on or off an LED plugged on your Arduino board.

Can be use for push button for command who should not be repeat.

# Arduino connection

Board view | Sketch view
---------- | -----------
![2 positions toggle switch connection (board view)](/resources/img/toggle_switch_bb.png?raw=true) | ![2 positions toggle switch connection (sketch view)](/resources/img/toggle_switch_sk.png?raw=true)


# Code sample

```cpp

void setup(){
  ...
}

```

# Options (constructor parameters)

Here is the definition of the xxx constructor :

**Multi arduino board OFF :**
`XXXX( ... );`

**Multi arduino board ON :**
`XXXX( ... );`

Arguments definition :
1. `...` : PIN n° ...
2. `...` (default: 1) : ...
3. `int boardAddress` (default: -1) : Address of slave board on which xxxx is plug (-1 = plug on main board). *Available only if you previously enable the "multi board mode"*
