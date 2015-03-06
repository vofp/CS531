import subprocess

class FluentLogic(object):
	"""
	Uses fluents to represent the wumpus world
	"""
	def __init__(self):
		self.axioms = "formulas(sos).Succ(10,9).Succ(9,8).Succ(8,7).Succ(7,6).Succ(6,5).Succ(5,4).Succ(4,3).Succ(3,2).Succ(2,1).Succ(1,0).Adjacent([x, y], [u, v]) <-> x = u & (Succ(y,v) | Succ(v,y)) | y = v & (Succ(x, u) | Succ(u, y)).At(Agent,x,z) & Breeze(z)	-> Breezy(x).At(Agent,x,z) & -Breeze(z)	-> -Breezy(x).At(Agent,x,z) & Stench(z)	-> Smelly(x).At(Agent,x,z) & -Stench(z)	-> -Smelly(x).-Breezy(x) & Adjacent(x,y) -> -Pit(y).-Smelly(x) & Adjacent(x,y) -> -Wumpus(y).all x (exists y (Adjacent(x,y) & Pit(y)) <-> Breezy(x)).all x (exists y (Adjacent(x,y) & (Wumpus(y)) <-> Smelly(x)).(Visited(x,z) & t <= z) <-> (exists t At(Agent,x,t)).Unvisited(x) <-> -(exists t At(Agent,x,t)).OK(x,z) <-> (-Pit(x) & -Wumpus(x) & WumpusAlive(z)))."
		# self.knowledge = "At(Agent, [0,0], 0).At(Agent, [1,0], 1).At(Agent, [1,3], 2).-Breeze(0).-Stench(0).Breeze(1).-Stench(1).-Breeze(2).-Stench(2)."
		self.knowledge = "At(Agent, [0,0], 0)"

	def tellPrecepts(precepts,t):
		if precepts[0]:
			self.tell("Breeze("+t+").")
		else:
			self.tell("-Breeze("+t+").")
		if precepts[1]:
			self.tell("Stench("+t+").")
		else:
			self.tell("-Stench("+t+").")
		if precepts[3]:
			self.tell("-WumpusAlive("+t+").")
		else:
			self.tell("WumpusAlive("+t+").")

	def askOK(self, x,y,t):
		return self.query("OK(["+x+","+y+"],"+t+").")

	def askVisited(self, x,y,t):
		return self.query("Visited(["+x+","+y+"],"+t+").")

	def askNotWumpus(self,x,y):
		return self.query("-Wumpus(["+x+","+y+"]).")

	def askNotOK(self,x,y,t):
		return self.query("-OK(["+x+","+y+"],"+t+").")

	def tell(self, query):
		self.knowledge += query

	def ask(self, query):
		return self.query(query)
	
	def query(self, query):
		"""
		Runs the given query, returning true if the query is true
		"""
		#start the process
		process = subprocess.Popen("./prover9", stdin=subprocess.PIPE,\
		stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		


		closing = "end_of_list."
		goal = "formulas(goals)."
		# print axioms + knowledge + closing + goal + query + closing
		# give the process the body of logical rules and facts
		process.communicate(self.axioms + knowledge + closing + goal + query + closing)
		# process.communicate(query)
		
		if process.returncode == 1:
			print query
			raise Exception("syntax error!")
		
		#prover 9 only exits with status 0 when the query was found to be true
		return process.returncode == 0

