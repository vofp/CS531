import os
import os.path

import env
import agent
import logic2 as logic
import time

#---------------------------------------MAIN--------------------------------------
if __name__ == "__main__":

	directory = "./maps/"

	#create the agent
	# player = agent.RandomAgent()
	player = agent.InteractiveAgent()
	# player = agent.SmarterAgent()

	print "file name, is solveable?, has good start?, map size, success?, died?, num actions, has arrow"

	#for each of the map files run a simulation
	for aFile in os.listdir(directory):

		#build the environment
		theMap = env.Environment(os.path.join(directory,aFile))
		print os.path.basename(theMap.fileName)
	
		#create the logic engine
		kb = logic.FluentLogic(theMap.size)
	
		#run the agent and get the results
		t0 = time.time()
		(success, died, actions, hasArrow, queries, wumpusAlive) = player.search(theMap, kb)
		timing = time.time() - t0
		#map, possible, can start?, size, success, died?, error?, actions, arrow
		print "%s, %s, %s, %i, %s, %s, %i, %s, %s, %s, %f" %\
		(os.path.basename(theMap.fileName), theMap.isSolveable(),\
		not theMap.noLogicalStart(), theMap.size, success, died,\
		actions, hasArrow, queries, wumpusAlive, timing)
		
