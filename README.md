# covid19-test-unit
It is a simulation and solution with mutex and semaphores for a covid-19 test unit. This system has these requirements:
● Each waiting room has 3 people.
● People will come to the unit in continuous and random periods.
● The hospital has 8 units and also 8 healthcare staff. So, each unit contains one staff.
● The states for each unit:
○ Entry free state: Staff will announce to the people his remaining places to get in the
waiting room, if it has one or more people in the room. 
○ Idle (Empty) state: If there are no people in the room, the staff will ventilate the room.
Being idle is forbidden for the staff. If any people came, they should open the room.
No people should wait.
○ Full and busy state: If there are 3 people in the room, the room will be in a busy state.
● The states for each people:
○ Waiting in room: When a person comes at room, she/he gets in the nearly full
capacity room. If there are no people in the room, the first person alerts the staff if she/he
is ventilating. And the people prepare for the test and fill out the form until the
room’s full. After the room is full, the test process will be applied to them and
together they empty the room.
○ Waiting at hospital: If there is no empty room, she/he's gonna wait for a room at the
outdoor waiting hole of the hospital.
● Starvation should be avoided.
