import subprocess
import threading

class FluentLogic(object):
	"""
	Uses fluents to represent the wumpus world
	"""
	def __init__(self,size):
		self.axioms = "formulas(sos).Adjacent([x, y], [u, v]) <-> ((x = u & (Succ(y,v) | Succ(v,y))) | (y = v & (Succ(x, u) | Succ(u, x)))).-Breezy(x) & Adjacent(x,y) -> -Pit(y).-Smelly(x) & Adjacent(x,y) -> -Wumpus(y).all x (exists y (Adjacent(x,y) & Pit(y)) <-> Breezy(x)).all x (exists y (Adjacent(x,y) & Wumpus(y)) <-> Smelly(x)).Visited(x) <-> Agent(x).Agent(x) -> -Pit(x).Agent(x) -> -Wumpus(x).OK(x) <-> (-Pit(x) & -(Wumpus(x) & WumpusAlive(1))) | Visited(x)."

		# all x (exists y (Adjacent(x,y) & Pit(y)) <-> Breezy(x))

		# all x all y (Breezy([x,y]) <-> exists z ((Pit([x,z]) & (z = y+ -1 | z = y+ 1)) | (Pit([z,y]) & (z = x+ -1 | z = x+ 1)) )   )


		# Adjacent([x, y], [u, v]) <-> ((x = u & (Succ(y,v) | Succ(v,y))) | (y = v & (Succ(x, u) | Succ(u, x)))).
		
		for x in xrange(size-1):
			self.axioms += "-Pit([-1,"+str(x)+"])."
			self.axioms += "-Pit(["+str(x)+",-1])."
			self.axioms += "-Pit(["+str(size)+","+str(x)+"])."
			self.axioms += "-Pit(["+str(x)+","+str(size)+"])."
			self.axioms += "-Wumpus([-1,"+str(x)+"])."
			self.axioms += "-Wumpus(["+str(x)+",-1])."
			self.axioms += "-Wumpus(["+str(size)+","+str(x)+"])."
			self.axioms += "-Wumpus(["+str(x)+","+str(size)+"])."
		# 	for y in xrange(size-1):
		# 		if y != x+1:
		# 			self.axioms += "-Succ("+str(y)+","+str(x)+")."
		# 		else:
		# 			self.axioms += "Succ("+str(x+1)+","+str(x)+")."

		# self.axioms += "-Succ("+str(size)+","+str(size-1)+")."
		# self.axioms += "-Succ(0,-1)."

		for x in xrange(size-1):
			for y in xrange(size-1):
				if x+1 != size: self.axioms += "Adjacent(["+str(x)+","+str(y)+"], ["+str(x+1)+","+str(y)+"])."
				if x != 0: self.axioms += "Adjacent(["+str(x)+","+str(y)+"], ["+str(x-1)+","+str(y)+"])."
				if y+1 != size: self.axioms += "Adjacent(["+str(x)+","+str(y)+"], ["+str(x)+","+str(y+1)+"])."
				if y != 0: self.axioms += "Adjacent(["+str(x)+","+str(y)+"], ["+str(x)+","+str(y-1)+"])."

		self.knowledge = ""
		self.WumpusAlive = "WumpusAlive(1)."
		self.agent = {}
		self.count = 0
		self.safe = []

	def tellPrecepts(self,precepts,x,y,t):
		x = str(x)
		y = str(y)
		t = str(t)
		if (x,y) not in self.agent:
			self.agent[(x,y)] = precepts
			self.tell("Agent(["+x+","+y+"]).")
			if precepts[0]:
				self.tell("Breezy(["+x+","+y+"]).")
			else:
				self.tell("-Breezy(["+x+","+y+"]).")
			if precepts[1]:
				self.tell("Smelly(["+x+","+y+"]).")
			else:
				self.tell("-Smelly(["+x+","+y+"]).")
			self.WumpusAlive = not precepts[3]
			if precepts[3]:
				self.WumpusAlive = "-WumpusAlive(1)."
			else:
				self.WumpusAlive = "WumpusAlive(1)."

	def askOK(self, x,y,t):
		x = str(x)
		y = str(y)
		if (x,y) in self.agent:
			return True
		if (x,y) in self.safe:
			return True
		r = self.query("OK(["+x+","+y+"]).")
		if r:
			self.safe.append((x,y))
		return r

	def askVisited(self, x,y,t):
		x = str(x)
		y = str(y)
		if (x,y) in self.agent:
			return True
		return self.query("Visited(["+x+","+y+"]).")

	def askNotWumpus(self,x,y,t):
		x = str(x)
		y = str(y)
		if (x,y) in self.agent:
			return True
		return self.query("-Wumpus(["+x+","+y+"]).")

	def askNotOK(self,x,y,t):
		x = str(x)
		y = str(y)
		return self.query("-OK(["+x+","+y+"]).")

	def tell(self, query):
		self.knowledge += query

	def ask(self, query):
		return self.query(query)
	
	def printKb(self):

		closing = "end_of_list."
		return self.axioms + self.knowledge+ self.WumpusAlive + closing

	def query(self, query):
		self.count += 1
		"""
		Runs the given query, returning true if the query is true
		"""

		def target():
		#start the process
			self.process = subprocess.Popen("./prover9", stdin=subprocess.PIPE,\
			stdout=subprocess.PIPE, stderr=subprocess.PIPE)
			


			closing = "end_of_list."
			goal = "formulas(goals)."
			# print query
			# print self.axioms + self.knowledge+ self.WumpusAlive + closing + goal + query + closing
			# give the process the body of logical rules and facts
			self.process.communicate(self.axioms + self.knowledge+ self.WumpusAlive + closing + goal + query + closing)
			# process.communicate(query)
		
		thread = threading.Thread(target=target)
		thread.start()

		thread.join(5)
		if thread.is_alive():
			self.process.terminate()
			thread.join()
			return False

		if self.process.returncode == 1:
			print query
			raise Exception("syntax error!")
		
		#prover 9 only exits with status 0 when the query was found to be true
		return self.process.returncode == 0

