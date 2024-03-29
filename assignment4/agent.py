import random
import env
from copy import deepcopy

NORTH = 0
EAST = 1
SOUTH = 2
WEST = 3

NOOP = 0
MOVE = 1
SHOOT = 2
GRAB = 3
CLIMB = 4

class RandomAgent(object):
	"""
	A hybrid agent that uses logic and a planner to work its way through the
	wumpus world
	"""
	
	def __init__(self):
		"""
		Initializes the agent
		"""
		self.logic = None
		self.environ = None
		self.actions = 0
		self.hasArrow = True
		
	def search(self, environment, logicEngine):
		"""
		Executes a random search for the gold
		"""
		self.environ = environment
		self.logic = logicEngine
		self.actions = 0
		self.hasArrow = True
		success = False
		dead = False
		goldFound = False
		x, y = (0,0)
	
		#keep looking for the gold stupidly
		while not goldFound and not dead:

			(breeze, stench, gilter) = self.environ.sense(x,y)

			dead = self.environ.isDeadly(x,y) 

			if not dead:
				
				if gilter:
					goldFound = True
			
				else:
					
					#get a list of adjacent cells
					adjacentCells = self.environ.proxy(x,y)

					#here is a good place to ask a query to the logic engine....
					#self.logic.query( .... )

					#pick one randomly
					(x, y) = random.choice(adjacentCells)

				#increment the action counter
				self.actions += 1

		return (success, dead, self.actions, self.hasArrow)
	
class domainAgent:
	def __init__(self):
		self.logic = None
		self.environ = None
		self.actions = 0
		self.hasArrow = True
		# 0:noop 1:move 2:shoot
		self.lastAction = 0
		self.direction = NORTH
		self.success = False
		self.scream = False
		self.plan = [(0,None)]
		# self.position = (0,0)
		self.x = 0
		self.y = 0
		self.searchCells = []

	def takeAction(self):
		# plan is a list of actions
		# action is a tuple of a (int,int) that represent (action type, direction)
		if len(self.plan) == 0:
			self.lastAction = 0
			return False
		nextAction, direction = self.plan.pop(0)
		# print "Action: ", nextAction, direction
		self.lastAction = nextAction
		if(nextAction == MOVE): # move
			x = self.x
			y = self.y
			if(direction == NORTH):
				y += 1 
			elif(direction == WEST):
				x -= 1 
			elif(direction == EAST):
				x += 1 
			elif(direction == SOUTH):
				y -= 1
			if(self.environ.isDeadly(x,y)):
				self.actions += 1
				return False
			if(self.environ.canMove(x,y)):
				self.x = x
				self.y = y
				self.actions += 1
				return True 
			return True
		elif(nextAction == SHOOT): # shoot arrow
			if(self.hasArrow):
				self.hasArrow = False
				self.actions += 1
				return self.environ.shootArrow(self.x,self.y,direction)
			return False
		elif(nextAction == GRAB): # grab gold
			self.actions += 1
			return self.environ.map[(self.x,self.y)][2]
		elif(nextAction == CLIMB): # climb out
			self.actions += 1
			return self.x == 1 and self.y == 1
		return False

	def sense(self):
		(pit, wumpus, gold) = self.environ.sense(self.x,self.y)
		return [pit, wumpus, gold,self.scream]


class SmarterAgent(object):
	"""
	A hybrid agent that uses logic and a planner to work its way through the
	wumpus world
	"""
	
	def __init__(self):
		"""
		Initializes the agent
		"""
		self.agent = domainAgent()
		
		
	def search(self, environment, logicEngine):
		"""
		Executes a random search for the gold
		"""
		self.agent = domainAgent()
		self.agent.environ = environment
		self.agent.logic = logicEngine
		self.agent.actions = 0
		self.agent.hasArrow = True
		self.agent.success = False
		dead = False
		goldFound = False

		r = self.step()
		while not r:
			print self.agent.actions
			r = self.step()

		return r
	
	def step(self):
		agent = self.agent
		r = agent.takeAction()
		agent.scream = False
		if(agent.lastAction == MOVE and not r):
			return (False, True, agent.actions, agent.hasArrow)
		elif(agent.lastAction == SHOOT):
			agent.scream = r
		elif(agent.lastAction == CLIMB and r):
			return(True,False,agent.actions,agent.hasArrow)
		elif(agent.lastAction == GRAB and r): # grab gold
			agent.environ.map[(agent.x,agent.y)][2] = False
		percepts = agent.sense()
		self.actionPlan(percepts)
		return False		

	def actionPlan(self,percepts):
		agent = self.agent
		l = agent.logic
		size = agent.environ.size
		t = agent.actions
		l.tellPrecepts(percepts,agent.x,agent.y,t)
		# tellPhysics(t)
		# get all safe tiles
		safe = [(x,y) for x in xrange(size) for y in xrange(size) if l.askOK(x,y,t)]
		if percepts[2]:
			agent.plan = []
			agent.plan.append((GRAB,None))
			agent.plan.extend(planRoute((agent.x,agent.y),[(1,1)],safe))
			agent.plan.append((CLIMB,None))
		unvisited = []
		# move to next unvisited cell 
		if len(agent.plan) == 0:
			unvisited = [(x,y) for x in xrange(size) for y in xrange(size) if not l.askVisited(x,y,t)]
			safeUnvisited = list(set(unvisited) & set(safe))
			agent.plan.extend(planRoute((agent.x,agent.y),safeUnvisited,safe))
		# shoot a wumpus
		if len(agent.plan) == 0 and agent.hasArrow:
			possibleWumpus = [(x,y) for x in xrange(size) for y in xrange(size) if not l.askNotWumpus(x,y,t)]
			agent.plan.extend(planShot((agent.x,agent.y),possibleWumpus,safe))
		# move to a cell that might be unsafe
		if len(agent.plan) == 0:
			notUnsafe = [(x,y) for x in xrange(size) for y in xrange(size) if not l.askNotOK(x,y,t)]
			notUnsafeUnvisited = list(set(unvisited) & set(safe))
			agent.plan.extend(planRoute((agent.x,agent.y),notUnsafeUnvisited,safe))
		# nothing left to do, climb out 
		if len(agent.plan) == 0:
			agent.plan.extend(planRoute((agent.x,agent.y),[(1,1)],safe))
			agent.plan.append((CLIMB,None))

def directionf(x1, y1, x2, y2):
	"""
	Returns the direction (x2, y2) is relative to (x1, y1) or none if they
	are not in a straight line from each other
	"""
	if x1 == x2 and y2 > y1:
		return NORTH
	elif x1 == x2 and y2 < y1:
		return SOUTH
	elif y1 == y2 and x2> x1:
		return EAST
	elif y1 == y2 and x2 < x1:
		return WEST
	else:	
		return None

def planShot(start, goals, safe_nodes):
    idGoals = []
    for g in goals:
        idGoals.extend(get_neighbors(g))
    idGoals = list(set(goals) & set(safe_nodes))
    path = planRoute(start, idGoals, safe_nodes)
    d = [0, 0, 0, 0]
    for a in path:
        action, direction = a
        d[direction] += 1
    dx = d[0]-d[2]
    dy = d[1]-d[3]
    x, y = start
    x += dx
    y += dy 
    wumpus_loc = list(get_neighbors((x,y)) & set(goals))
    if len(wumpus_loc) == 0:
    	return []
    x2, y2 = wumpus_loc[0]
    shot_dir = directionf(x,y,x2,y2)
    path.append((2, shot_dir))
    return path

   
def planRoute(start, goals, safe_nodes):
    # print "start ", start
    # print "goals ", goals
    # print "nodes ", safe_nodes
    return branchbound(start, goals, safe_nodes)
	#pass
	# rewrite search algorithms (breath-first, a*)
	# start at starting position (start) 
	# take action (NORTH, SOUTH, WEST, EAST) leading to a valid node in nodes
	# return path as an array of [(MOVE,NORTH),(MOVE,EAST)...(MOVE,WEST)]
	# cost is 1 per move

def astar(start, goals, safe_nodes):
    pass
    


def branchbound(start, goals, safe_nodes):
    open_set = []
    closed_set = []
    path = {}
    open_set.append(start)
    path[start] = []
    while not len(open_set) == 0:
        test_node = open_set.pop(0)
        closed_set.append(test_node)
        for d in range(4):
            x,y = test_node
            if(d == NORTH):
                    y += 1 
            elif(d == WEST):
                    x -= 1 
            elif(d == EAST):
                    x += 1 
            elif(d == SOUTH):
                    y -= 1
            w = (x,y)
            if w not in safe_nodes:
                continue
            if w not in closed_set:
                open_set.append(w)
                test = deepcopy(path[test_node])
                path[w] = test
                path[w].append((1,d))
            # print w, path
            if w in goals:
                return path[w]
    return []

				
def get_neighbors(loc):
	x, y = loc
	return set([(x,y+1),(x+1,y),(x-1,y),(x,y-1)])

	#list(set(get_neighbors) & set(safe_nodes))
	#make a* and breadth-first functions


class InteractiveAgent(object):
	"""
	A hybrid agent that uses logic and a planner to work its way through the
	wumpus world
	"""
	
	def __init__(self):
		"""
		Initializes the agent
		"""
		self.agent = domainAgent()
		
		
	def search(self, environment, logicEngine):
		"""
		Executes a random search for the gold
		"""
		self.agent = domainAgent()
		self.agent.environ = environment
		self.agent.logic = logicEngine
		self.agent.actions = 0
		self.agent.hasArrow = True
		self.agent.success = False
		dead = False
		goldFound = False

		r = self.step()
		while not r:
			# print self.agent.actions
			r = self.step()

		return r
	
	def step(self):
		agent = self.agent
		r = agent.takeAction()
		agent.scream = False
		if(agent.lastAction == MOVE and not r):
			return (False, True, agent.actions, agent.hasArrow,agent.logic.count,agent.logic.WumpusAlive)
		elif(agent.lastAction == SHOOT):
			agent.scream = r
		elif(agent.lastAction == CLIMB and r):
			return(True,False,agent.actions,agent.hasArrow,agent.logic.count,agent.logic.WumpusAlive)
		elif(agent.lastAction == GRAB and r): # grab gold
			m = agent.environ.map[(agent.x,agent.y)]
			agent.environ.map[(agent.x,agent.y)] = (m[0],m[1],False)
		percepts = agent.sense()
		r = self.actionPlan2(percepts)
		if not r:
			return (False, False, agent.actions, agent.hasArrow,agent.logic.count,agent.logic.WumpusAlive)
		return False

	def actionPlan2(self,percepts):
		agent = self.agent
		l = agent.logic
		size = agent.environ.size
		t = agent.actions
		l.tellPrecepts(percepts,agent.x,agent.y,t)
		self.printMap(0)
		print ""
		#l.tellPrecepts(precepts,t)
		# tellPhysics(t)
		# get all safe tiles
		agent.searchCells.extend(list(get_neighbors((agent.x,agent.y))))
		safe = []
		if percepts[2]:
			agent.plan = []
			agent.plan.append((GRAB,None))
			safe = [(x,y) for x in xrange(size) for y in xrange(size) if (x,y) in agent.searchCells if l.askOK(x,y,t)]
			safe.append((1,1))
			agent.plan.extend(planRoute((agent.x,agent.y),[(1,1)],safe))
			agent.plan.append((CLIMB,None))
			return agent.plan
		unvisited = []
		# move to next unvisited cell 
		if len(agent.plan) == 0:
			print 1
			safe = [(x,y) for x in xrange(size) for y in xrange(size) if (x,y) in agent.searchCells if l.askOK(x,y,t)]
			unvisited = [(x,y) for x in xrange(size) for y in xrange(size) if (x,y) in agent.searchCells if (x,y) in safe if not l.askVisited(x,y,t)]
			# safeUnvisited = list(set(unvisited) & set(safe))
			safeUnvisited = unvisited
			agent.plan.extend(planRoute((agent.x,agent.y),safeUnvisited,safe))
		# shoot a wumpus
		if len(agent.plan) == 0 and agent.hasArrow:
			print 2
			possibleWumpus = [(x,y) for x in xrange(size) for y in xrange(size) if not l.askNotWumpus(x,y,t)]
			agent.plan.extend(planShot((agent.x,agent.y),possibleWumpus,safe))
		# move to a cell that might be unsafe
		if len(agent.plan) == 0:
			print 3
			notUnsafe = [(x,y) for x in xrange(size) for y in xrange(size) if (x,y) in agent.searchCells if (x,y) in unvisited if not l.askNotOK(x,y,t)]
			notUnsafeUnvisited = list(set(unvisited) & set(safe))
			# notUnsafeUnvisited = notUnsafe
			agent.plan.extend(planRoute((agent.x,agent.y),notUnsafeUnvisited,safe))
		# nothing left to do, climb out 
		if len(agent.plan) == 0:
			print 4
			safe.append((1,1))
			agent.plan.extend(planRoute((agent.x,agent.y),[(1,1)],safe))
			if len(agent.plan) == 0:
				return False
			agent.plan.append((CLIMB,None))
		return agent.plan


	def actionPlan(self,percepts):
		agent = self.agent
		size = agent.environ.size
		l = agent.logic
		t = agent.actions
		l.tellPrecepts(percepts,agent.x,agent.y,t)
		# tellPhysics(t)
		# get all safe tiles
		print percepts,
		print " at ("+str(agent.x)+","+str(agent.y)+")"
		self.printMap(0)
		print self.actionPlan2(percepts)
		return

		r = raw_input("[a]ction/[q]uery/[e]val/[p]rint: ")
		print r
		if r[0] == 'a':
			a = int(raw_input("action int: "))
			d = int(raw_input("direction int: "))
			agent.plan.append((a,d))
			return
		elif r[0] == 'q':
			q = raw_input("query: ")
			try:
				print agent.logic.query(q)
			except Exception, e:
				print e
		elif r[0] == 'r':
			print self.actionPlan2(percepts)
		elif r[0] == 'e':
			e = raw_input("eval: ")
			try:
				print eval(e)
			except Exception, e2:
				print e2
		elif r[0] == 'p':
			self.printMap(1)

	def printMap(self,i):
		agent = self.agent
		env = agent.environ
		l = agent.logic
		for y in range(env.size-1, -1, -1):
			for x in range(0, env.size):
			
				(pit,wumpus,gold) = (False, False, False)
				
				if (x,y) in env.map:
					(pit,wumpus,gold) = env.map[(x,y)]
				
				print ("%s%s%s%s|" % ( "p" if pit else " ", "w" if wumpus else " ", "g" if gold else " " ,"a" if (agent.x == x and agent.y == y) else " ")),
			
			print ""
			if i == 1:
				for x in range(0, env.size):
				
					print ("%s%s%s%s|" % ( "O" if l.askOK(x,y,agent.actions) else " ", "N" if not l.askNotOK(x,y,agent.actions) else " ", "V" if not l.askVisited(x,y,agent.actions) else " " ,"w" if not l.askNotWumpus(x,y,agent.actions) else " ")),
				
				print ""
			
			#print a row seperator
			print "____|_" * env.size
