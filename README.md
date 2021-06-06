# README

```
Alex Griffiths
18001525
```

This is my sudoku bot workspace.

My ASP solver is in the `my_sudoku_ASP_program.lp` file and is currently configured to solve a 9x9 sudoku puzzle. This is also the size of puzzle that my `tester_node` in the `tester` package generates and sends to the `interface_node`.

To change the size of the puzzle that the solver is configured for, you simply need to change line 2 of the program from

```
#const sg = 3
```

to whatever the square root of the total puzzle grid is. In the case of the 100x100 puzzle, this would be:

```
#const sg = 10
```

_This line corresponds to the size of the subgrid in the puzzle._

During testing, I started my nodes in the following order, however, all the really matters is that the node that sends the initial facts is started last after all the other nodes are running.

1. `roscore`
2. `rosrun interface interface_node`
3. `rosrun move_arm_joint arm_shoulder_pan_joint`
4. `rosrun move_arm_joint arm_elbow_flex_joint`
5. `rosrun move_arm_joint arm_wrist_flex_joint`
6. `rosrun move_arm_joint arm_shoulder_lift_joint`
7. `rosrun move_arm_joint gripper_joint`
8. `rosrun mover_client mover_client_node`
9. `rosrun tester tester_node` _This is an optional step if you already have another package in mind for sending the initial facts_


