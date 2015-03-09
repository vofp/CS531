import random
import env
from copy import deepcopy
from astar import Astar

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

	def takeAction(self):
		# plan is a list of actions
		# action is a truple of a (int,int) that represent (action type, direction)
		nextAction, direction = self.plan.pop(0)
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
			if(hasArrow):
				self.hasArrow = False
				self.actions += 1
				return self.environ.shootArrow(self.x,self,y,direction)
			return False
		elif(nextAction == GRAB): # grab gold
			return self.environ.map[(self.x,self.y)][2]
		elif(nextAction == CLIMB): # climb out
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

		return self.step()

		return (success, dead, self.actions, self.hasArrow)
	
	def step(self):
		agent = self.agent
		r = agent.takeAction()
		agent.scream = False
		if(self.nextAction == 1 and not r):
			return (False, True, agent.actions, agent.hasArrow)
		elif(self.nextAction == 2):
			agent.scream = r
		percepts = agent.sense()
		self.actionPlan(percepts)		

	def actionPlan(self,percepts):
		agent = self.agent
		l = agent.logic
		size = agent.environ.size
		t = agent.actions
		l.tellPrecepts(precepts,t)
		# tellPhysics(t)
		# get all safe tiles
		safe = [(x,y) for x in xrange(size) for y in xrange(size) if l.askOK(x,y,t)]
		if precepts[2]:
			agent.plan.append((GRAB,None))
			agent.plan.extend(planRoute((agent.x,agent.y),[(1,1)],safe))
			agent.plan.append((CLIMB,None))
		unvisited = []
		# move to unvisited
		if len(agent.plan) == 0:
			unvisited = [(x,y) for x in xrange(size) for y in xrange(size) if l.askVisited(x,y)]
			safeUnvisited = list(set(unvisited) & set(safe))
			agent.plan.extend(planRoute((agent.x,agent.y),safeUnvisited,safe))
		# shoot a wumpus
		if len(agent.plan) == 0 and agent.hasArrow:
			possibleWumpus = [(x,y) for x in xrange(size) for y in xrange(size) if not l.askNotWumpus(x,y)]
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


def planRoute(start, goals, nodes):
	pass
	# rewrite search algorithms (breath-first, a*)
	# start at starting position (start) 
	# take action (NORTH, SOUTH, WEST, EAST) leading to a valid node in nodes
	# return path as an array of [(MOVE,NORTH),(MOVE,EAST)...(MOVE,WEST)]
	# cost is 1 per move
        Q = set()
        v = set()
        Q.push(start)
        while Q is not empty:
            v = Q.pop()
            get_neighbors(v)
            for w in v:
                Q.push(w)
                


        graph, node astar.make_graph()
        paths = astar.AStarGrid(graph)
        paths.search(start, end)



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
			print self.agent.actions
			r = self.step()

		return r
	
	def step(self):
		agent = self.agent
		print "Take action ",
		print agent.plan[0],
		r = agent.takeAction()
		print " resulting in ",
		print r
		agent.scream = False
		if(agent.lastAction == MOVE and not r):
			return (False, True, agent.actions, agent.hasArrow)
		elif(agent.lastAction == SHOOT):
			agent.scream = r
		elif(agent.lastAction == CLIMB and r):
			return(True,False,agent.actions,agent.hasArrow)
		
		percepts = agent.sense()
		self.actionPlan(percepts)
		return False


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
		self.printMap()
		while True:
			r = raw_input("[a]ction/[q]uery/[e]val: ")
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
			elif r[0] == 'e':
				e = raw_input("eval: ")
				eval(e)

	def printMap(self):
		agent = self.agent
		env = agent.environ
		for y in range(env.size-1, -1, -1):
			for x in range(0, env.size):
			
				(pit,wumpus,gold) = (False, False, False)
				
				if (x,y) in env.map:
					(pit,wumpus,gold) = env.map[(x,y)]
				
				print ("%s%s%s%s|" % ( "p" if pit else " ", "w" if wumpus else " ", "g" if gold else " " ,"a" if (agent.x == x and agent.y == y) else " ")),
			
			print ""
			
			#print a row seperator
			print "____|_" * env.size

