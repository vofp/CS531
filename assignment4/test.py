from copy import deepcopy
import logic



def main():
	kb = logic.FluentLogic()
	print kb.query("formulas(assumptions).  man(george). end_of_list. formulas(goals). mortal(george). end_of_list.")

if __name__ == "__main__":
	main();