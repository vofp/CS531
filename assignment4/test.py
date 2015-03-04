from copy import deepcopy
import logic



def main():
	kb = logic.FluentLogic()
	# print kb.query("At(Agent, [0,0], 0).")
	print kb.query("formulas(sos).man(x) -> mortal(x).man(george).end_of_list.formulas(goals).mortal(george).end_of_list.")

if __name__ == "__main__":
	main();