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
		self.nextAction = 0
		self.direction = NORTH
		self.success = False
		self.scream = False
		self.plan = []
		# self.position = (0,0)
		self.x = 0
		self.y = 0

	def takeAction(self):
		# plan is a list of actions
		# action is a truple of a (int,int) that represent (action type, direction)
		nextAction, direction = self.plan.pop(0)
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
			if(self.environ.canMove(x,y)):
				self.x = x
				self.y = y
				self.actions += 1
				return True 
			if(self.environ.isDeadly(x,y))
				self.actions += 1
				return False
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

	def sense():
		p = self.environ.sense(self.x,self.y)
		p.append(self.scream)
		return p


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
	# rewrite search algorithms (breath-first, a*)
	# start at starting position (start) 
	# take action (NORTH, SOUTH, WEST, EAST) leading to a valid node in nodes
	# return path as an array of [(MOVE,NORTH),(MOVE,EAST)...(MOVE,WEST)]
	# cost is 1 per move
	



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
		if(self.nextAction == MOVE and not r):
			return (False, True, agent.actions, agent.hasArrow)
		elif(self.nextAction == SHOOT):
			agent.scream = r
		elif(self.nextAction == CLIMB and r):
			return(True,False,agent.actions,agent.hasArrow)
		
		percepts = agent.sense()
		self.actionPlan(percepts)		

	def actionPlan(self,percepts):
		agent = self.agent
		size = agent.environ.size
		t = agent.actions
		tellPrecepts(precepts,x,y,t)
		# tellPhysics(t)
		# get all safe tiles
		while True:
			r = input("[a]ction/[q]uery/[e]val: ")
			if r[0] == 'a':
				a = int(input("action int: "))
				d = int(input("direction int: "))
				agent.plan.append((a,d))
				return
			elif r[0] == 'q':
				q = input("query: ")
				agent.logic.query(q)
			elif r[0] == 'e':
				e = input("eval: ")
				eval(e)



