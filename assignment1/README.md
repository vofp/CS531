#PEAS#
##Performance Measurement##
It is evaluated by the total number of clean cells as a function of the number of actions taken. (Plot the number of clean cells vs. the number of actions taken). 

##Environment##
n X m empty rectangular room
p% chance of containing dirt

##Actuation##
go forward
turn right by 90 degrees
turn left by 90 degrees
suck up dirt
turn off

##Sensor##
a wall sensor = 1 if the machine has a wall right in the front and 0 otherwise
a dirt sensor = 1 if the square contains dirt
a home sensor = 1 if the agent is home (the starting location)

#Agent#
agent always starts in the bottom leftmost corner oriented upwards
first rule whose condition evaluates to TRUE will ``fire,'' which means the corresponding action is executed
If the agent program is deterministic then there is only one possible action selected at each step. 
If it is stochastic, then there are several actions, and each action will be chosen with some probability. 
For example, the rule "if wall then turn-left 0.5 turn-right 0.5" will choose turn-left and turn-right with equal probability if the wall sensor is ON.

##Type of Agents##
1 A simple memoryless deterministic reflex agent.
2 A randomized reflex agent that can choose actions randomly based on sensor readings.
3 A deterministic model-based reflex agent with a small amount (2 to 3 bits) of memory that represents the "state." When executing each action, the agent simultaneously updates the state by setting or resetting these bits according to how you specify them. The actions can be based on its current state bits as well as the current percepts.